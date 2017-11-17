#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <fcntl.h>
#include <unistd.h>

#include "cashka.h"
#include "options.h"
#include "server.h"

using std::string;

namespace cashka
{
	/**
	 * Конструктор
	 * 
	 * @param Options options
	 */
	Server::Server (Options & options) : options (options) {}
	
	/**
	 * Создать сокет
	 * 
	 * @return void
	 */
	void Server::start ()
	{
		if (this->options.get_unix_socket().empty())
		{
			this->socket_ip_create();
		}
		else
		{
			this->socket_unix_create();
		}
		
		this->socket_select();
	}
	
	/**
	 * Закрыть сокет
	 * 
	 * @return void
	 */
	void Server::stop ()
	{
		
	}
	
	/**
	 * Информация по сокету
	 * 
	 * @return string
	 */
	string Server::socket_ip_info ()
	{
		string info;
		
		if (this->addrinfo == nullptr)
		{
			err ("Невозможно получить сведения по сети.");
		}
		
		struct addrinfo * ai; 
		int iface_num = 1;
		
		for (ai = this->addrinfo; ai != NULL; ai = ai->ai_next)
		{
			if (ai != this->addrinfo)
			{
				info += "\n";
			}

			info += "Интерфейс " + std::to_string(iface_num) + ":";

			void * addr;
			char ip_str[INET6_ADDRSTRLEN];

			if (ai->ai_family == AF_INET)
			{
				info += "\n - Версия протокола: «IPv4»";

				struct sockaddr_in *ipv4 = (struct sockaddr_in *)ai->ai_addr;
				addr = &(ipv4->sin_addr);
			}
			else if (ai->ai_family == AF_INET6)
			{
				info += "\n - Версия протокола: «IPv6»";

				struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)ai->ai_addr;
				addr = &(ipv6->sin6_addr);
			}

			inet_ntop(ai->ai_family, addr, ip_str, sizeof ip_str);
			info += "\n - Адрес: «" + (string)ip_str + "»";

			info += "\n - Порт: «" + std::to_string(this->options.get_port()) + "»";
		}

		
		return info;
	}
	
	/**
	 * Создать сокет 
	 * 
	 * @return void
	 */
	void Server::socket_ip_create ()
	{
		struct addrinfo hints;
		
		/* Создаём addinfo */
		memset (&hints, 0, sizeof (hints));	
		hints.ai_family = AF_UNSPEC;			/* IPv4 или IPv6 */
		hints.ai_socktype = SOCK_STREAM;		/* TCP */
		hints.ai_flags = AI_PASSIVE;			/* Если хост не указан выбрать автоматически */
		
		const char * host = this->options.get_host().c_str();
		const char * port = std::to_string(this->options.get_port()).c_str();
		
		int getaddrinfo_status;
		if ((getaddrinfo_status = getaddrinfo (host, port, &hints, &this->addrinfo)) != 0)
		{
			err ((string)"Невозможно получить сведения по сети. Подробно: " + gai_strerror(getaddrinfo_status));
		}
		
		/* Определяем кол-во мастер сокетов */
		struct addrinfo * ai; 
		for (ai = this->addrinfo; ai != nullptr; ai = ai->ai_next)
		{
			this->master_count ++;
		}
		this->master = new int [this->master_count];
		
		/* Создаём мастер сокеты */
		int i = 0;
		for (ai = this->addrinfo; ai != nullptr; ai = ai->ai_next)
		{
			/* Создаём основной сокет */
			if ((this->master[i] = socket (ai->ai_family, ai->ai_socktype, ai->ai_protocol)) == -1)
			{
				err ((string)"Не удалось создать соединение. Подробнее: " + strerror(errno));
			}

			/* Избегаем ошибки: Адрес уже занят  */
			int yes = 1;
			if (setsockopt(this->master[i], SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) 
			{
				err ((string)"Не удалось создать соединение. Подробнее: " + strerror(errno));
			}

			/* bind, listen */
			if ((bind (this->master[i], ai->ai_addr, ai->ai_addrlen)) == -1)
			{
				err ((string)"Не удалось создать соединение. Подробнее: " + strerror(errno));
			}

			if ((listen (this->master[i], 20)) == -1)
			{
				err ((string)"Не удалось создать соединение. Подробнее: " + strerror(errno));
			}

			/* Делаем сокет неблокирующим */
			fcntl (this->master[i], F_SETFL, O_NONBLOCK);
			
			/* Следующий сокет */
			i++;
		}
	}
	
	/**
	 * Создать unix-сокет
	 * 
	 * @return void
	 */
	void Server::socket_unix_create ()
	{
		struct sockaddr_un hints;
		memset (&hints, 0, sizeof (struct sockaddr_un));
		hints.sun_family = AF_LOCAL;
		strncpy (hints.sun_path, this->options.get_unix_socket().c_str(), sizeof (hints.sun_path) - 1);

		this->master_count = 1;
		this->master = new int [1];
		
		/* Создаём основной сокет */
		if ((this->master[0] = socket (AF_UNIX, SOCK_STREAM, 0)) == -1)
		{
			err ((string)"Не удалось создать соединение. Подробнее: " + strerror(errno));
		}

		/* Избегаем ошибки: Адрес уже занят  */
		int yes = 1;
		if (setsockopt(this->master[0], SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) 
		{
			err ((string)"Не удалось создать соединение. Подробнее: " + strerror(errno));
		}

		/* bind, listen */
		if ((bind (this->master[0], (const struct sockaddr *) &hints, sizeof(struct sockaddr_un))) == -1)
		{
			err ((string)"Не удалось создать соединение. Подробнее: " + strerror(errno));
		}

		if ((listen (this->master[0], 20)) == -1)
		{
			err ((string)"Не удалось создать соединение. Подробнее: " + strerror(errno));
		}
		
		/* Делаем сокет неблокирующим */
		fcntl (this->master[0], F_SETFL, O_NONBLOCK);
	}
	
	/**
	 * Отлавливать соединения через select
	 * 
	 * @return void
	 */
	void Server::socket_select ()
	{
		/* Очищаем множества и добавляем мастер сокеты */
		FD_ZERO (&this->fd_read);
		FD_ZERO (&this->fd_all);
		for (int i = 0; i < this->master_count; i++)
		{
			FD_SET (this->master[i], &this->fd_all);
		}
		
		/* Определяем fd_max */
		for (int i = 0; i < this->master_count; i++)
		{
			if (this->fd_max < this->master[i])
			{
				this->fd_max = this->master[i];
			}
		}
		
		do
		{
			/* Добавляем сокеты в read */
			this->fd_read = this->fd_all;

			/* Таймаут преопределяем (на некоторых системах сбрасываются) */
			this->select_timeout.tv_sec = this->select_timeout_sec;
			this->select_timeout.tv_usec = this->select_timeout_usec;
			
			int select_result = select (this->fd_max + 1, &fd_read, NULL, NULL, &this->select_timeout);
			
			/* Ошибка */
			if (select_result == -1)
			{
				err ((string)"Ошибка сети. Этап «select». Подробнее: " + strerror(errno));
			}

			/* Данные не пришли на сокеты */
			if (select_result < 1)
			{
				continue;
			}

			for (int i = 0; i <= this->fd_max; i++)
			{
				int socket = i;

				/* Доп. проверка, что данные не пришли на сокеты */
				if (!FD_ISSET (socket, &fd_read))
				{
					continue;
				}

				/* Пришли данные на мастер-сокет */
				bool is_master = false;
				for (int i = 0; i < this->master_count; i++)
				{
					if (socket == this->master[i])
					{
						is_master = true;
						break;
					}
				}
				
				/* Создаём новое соединение из мастера */
				if (is_master)
				{
					this->client_add (socket);
					
				}
				/* Выдаём сообщение */
				else
				{
					this->client_message_read (socket);
				}
			}
		}
		while (true);
	}
	
	/**
	 * Добавить нового клиента
	 * 
	 * @param int master
	 * @return void
	 */
	void Server::client_add (int master)
	{
		int socket_new;
		struct sockaddr_storage their_addr;
		socklen_t addr_size = sizeof their_addr;

		if ((socket_new = accept (master, (struct sockaddr *)&their_addr, &addr_size)) == -1)
		{
			err ((string)"Ошибка сети. Этап «accept». Подробнее: " + strerror(errno));
		}

		if (socket_new > this->fd_max)
		{
			this->fd_max = socket_new;
		}

		int send_size = send (socket_new, this->msg_hello, strlen (this->msg_hello) + 1, 0);
		if (send_size == -1)
		{
			err ((string)"Ошибка сети. Этап «send_hello». Подробнее: " + strerror(errno));
		}

		FD_SET (socket_new, &this->fd_all);
	}
	
	/**
	 * Отключить клиента
	 * 
	 * @param int socket
	 * @return void
	 */
	void Server::client_close (int socket)
	{
		FD_CLR (socket, &this->fd_all);
		close (socket);
	}
	
	/**
	 * Прочитать сообщение от клиента
	 * 
	 * @param int socket
	 * @return void
	 */
	void Server::client_message_read (int socket)
	{
		memset (&this->buffer, 0, sizeof this->buffer);
		int recv_size = recv (socket, this->buffer, sizeof this->buffer, 0);

		/* Ошибка при чтении данных с сокета */
		if (recv_size == -1)
		{
//			err ((string)"Ошибка сети. Этап «read». Подробнее: " + strerror(errno));
			this->client_close (socket);
			return;
		}

		/* Закрыть соединение */
		if (recv_size == 0)
		{
			this->client_close (socket);
			return;
		}

		/* Читаем сообщение и возвращаем ответ. */
		string message = (string)"Держи ответку: " + buffer;
		int send_size = send (socket, message.c_str(), message.length() + 1, 0);
		if (send_size == -1)
		{
			err ((string)"Ошибка сети. Этап «send_answer». Подробнее: " + strerror(errno));
		}
	}
}