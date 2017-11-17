#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <fcntl.h>
#include <unistd.h>

#include "options.h"
#include "client.h"
#include "cashka-cli.h"

using std::cout;
using std::endl;
using std::string;


namespace cashka_cli
{
	/**
	 * Конструктор
	 */
	Client::Client (Options & options) : options (options) {}

	/**
	 * Деструктор
	 */
	Client::~Client ()
	{
		this->close ();
	}

	/**
	 * Ожидание команд от клавиатуры и клиентского сокета
	 */
	void Client::on ()
	{
		this->_connect_info ();

		FD_ZERO (&this->fd_read);
		FD_ZERO (&this->fd_all);

		/* Слушаем клавиатуру */
		FD_SET (STDIN_FILENO, &fd_all);

		do
		{
			/* Добавляем сокеты в read */
			this->fd_read = this->fd_all;

			/* Таймаут преопределяем (на некоторых системах сбрасываются) */
			this->select_timeout.tv_sec = this->select_timeout_sec;
			this->select_timeout.tv_usec = this->select_timeout_usec;

			/* Могучий SELECT */
			int select_result = select (this->fd_max, &fd_read, nullptr, nullptr, &this->select_timeout);

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
				int fd = i;

				/* Доп. проверка, что данные не пришли на сокеты */
				if (!FD_ISSET (fd, &fd_read))
				{
					continue;
				}

				try
				{
					/* Данные с клавиатуры */
					if (fd == STDIN_FILENO)
					{
						this->read_stdin ();
					}
					/* Данные с сокета */
					else if (fd == this->socket)
					{
						this->read_socket ();
					}
				}
				catch (string & message)
				{
					cout << "Ошибка: " << message << endl;
				}
			}

			this->_connect_info ();

		}
		while (true);
	}

	/**
	 * Показать текст ошибки
	 *
	 * @param const char * message
	 */
	void Client::err (string message)
	{
		throw message;
	}

	/**
	 * Чтение данные с клавиатуры
	 */
	void Client::read_stdin ()
	{
		char buffer[512] = { 0 };
		int read_size = read (STDIN_FILENO, buffer, 512);

		if (read_size == -1)
		{
			this->err ("Невозможно прочитать данные с клавиатуры.");
		}

		buffer[strlen (buffer)-1] = '\0';

		/* Выход */
		if ((string)buffer == "exit" || (string)buffer == "quit")
		{
			if (this->is_connect)
			{
				this->close ();
			}

			exit (EXIT_SUCCESS);
		}

		/* Парсим и выполняем команду */
		this->parse (buffer);
	}

	/**
	 * Читаем данные с сокета
	 */
	void Client::read_socket ()
	{
		char buffer[512] = {0};
		int recv_size = recv (this->socket, buffer, 512, 0);
		if (recv_size > 0)
		{
			cout << "Данные с сервера: " << buffer << endl;
		}
	}

	/**
	 * Спарсить команду
	 *
	 * @param const char * command
	 */
	void Client::parse (const char * buffer)
	{
		char * str = strdup (buffer);

		char * command = strtok (str, " ");
		char * param = strtok (nullptr, " ");

		if (command == nullptr)
		{
			this->err ("Не задана команда.");
		}

		if ((string)command == "connect")
		{
			this->check_param_connect (param);
			this->connect (this->host.c_str(), this->port.c_str());
		}
		else if ((string)command == "connect-unix")
		{
			this->check_param_connect_unix (param);
			this->connect_unix (this->unix_socket.c_str ());
		}
		else if ((string)command == "close")
		{
			this->close ();
		}
		else if ((string)command == "send")
		{
			this->send (buffer + strlen (command) + 1);
		}
		else
		{
			this->err ("Неизвестная команда.");
		}
	}

	/**
	 * Соединится с сервером по протоколу IPv4/IPv6
	 *
	 * @param const char * host
	 * @param const char * port
	 */
	void Client::connect (const char * host, const char * port)
	{
		struct addrinfo hints, * addrinfo;

		memset (&hints, 0, sizeof (hints));
		hints.ai_family = AF_UNSPEC;			/* IPv4 или IPv6 */
		hints.ai_socktype = SOCK_STREAM;		/* TCP */
		hints.ai_flags = AI_PASSIVE;			/* Если хост не указан выбрать автоматически */

		int getaddrinfo_status;
		if ((getaddrinfo_status = getaddrinfo (host, port, &hints, &addrinfo)) != 0)
		{
			this->err ((string)"Хост или порт указан неверно. Подробно: " + gai_strerror (getaddrinfo_status));
		}

		if ((this->socket = ::socket (addrinfo->ai_family, addrinfo->ai_socktype, addrinfo->ai_protocol)) == -1)
		{
			this->err ((string)"Не удалось подключиться. Подробнее: " + strerror(errno));
		}

		if (::connect (this->socket, addrinfo->ai_addr, addrinfo->ai_addrlen) == -1)
		{
			this->err ((string)"Не удалось подключиться. Подробнее: " + strerror(errno));
		}

		this->is_connect = true;
		this->connect_type = "ip";

		FD_SET (this->socket, &this->fd_all);
		this->fd_max = this->socket + 1;

		cout << "Соединение установлено." << endl;
	}

	/**
	 * Соединится по UNIX-сокету
	 *
	 * @param const char * unix_socket
	 */
	void Client::connect_unix (const char * unix_socket)
	{
		struct sockaddr_un hints;
		memset (&hints, 0, sizeof (struct sockaddr_un));
		hints.sun_family = AF_LOCAL;
		strncpy (hints.sun_path, unix_socket, sizeof (hints.sun_path) - 1);

		if ((this->socket = ::socket (AF_UNIX, SOCK_STREAM, 0)) == -1)
		{
			this->err ((string)"Не удалось подключиться. Подробнее: " + strerror(errno));
		}

		if (::connect (this->socket, (struct sockaddr *)&hints, sizeof(struct sockaddr_un)) == -1)
		{
			this->err ((string)"Не удалось подключиться. Подробнее: " + strerror(errno));
		}

		this->unix_socket_basename = (string)basename (unix_socket);

		this->is_connect = true;
		this->connect_type = "unix";

		FD_SET (this->socket, &this->fd_all);
		this->fd_max = this->socket + 1;

		cout << "Соединение установлено." << endl;
	}

	/**
	 * Закрыть соединение
	 */
	void Client::close ()
	{
		/* Соединение уже закрыто */
		if (!this->is_connect)
		{
			cout << "Соединение уже закрыто." << endl;
			return;
		}

		/* Закрыть сокет */
		if (::shutdown (this->socket, SHUT_RDWR) == -1)
		{
			this->err ((string)"Не удалось закрыть соединение. Подробнее: " + strerror(errno));
		}

		/* Удаляем из множества (для select) */
		FD_CLR (this->socket, &this->fd_all);

		/* Обзначаем закрытие соединения */
		this->is_connect = false;
		if (this->connect_type == "ip")
		{
			this->host.clear ();
			this->port.clear ();
		}
		else if (this->connect_type == "unix")
		{
			this->unix_socket.clear ();
			this->unix_socket_basename.clear ();
		}

		cout << "Соединение закрыто." << endl;
	}

	/**
	 * Отправить сообщение
	 *
	 * @param const char * message
	 */
	void Client::send (const char * message)
	{
		if (!this->is_connect)
		{
			this->err ("Вам необходимо сначала подключиться к серверу, чтобы отправлять сообщение.");
		}

		int send_size = ::send (this->socket, message, strlen(message) + 1, 0);
		if (send_size == -1)
		{
			err ((string)"Не удалось отправить сообщение. Подробнее: " + strerror(errno));
		}

		cout << "Сообщение отправлено" << endl;
	}

	/**
	 * Проверка параметров команды «connect»
	 *
	 * @param const char * param
	 */
	void Client::check_param_connect (const char * param)
	{
		if (param == nullptr)
		{
			this->err ("Необходимо указать хост и порт. Пример: «127.0.0.1:3000» или «::1:3000»");
		}

		if (strtok (nullptr, " ") != nullptr)
		{
			this->err ("Параметры заданы неверно.");
		}

		string ip_port = param;
		int pos = ip_port.rfind (':');
		if (pos == string::npos)
		{
			this->err ("IP адрес и порт заданы неверно.");
		}

		this->host = ip_port.substr (0, pos);
		this->port = ip_port.substr (pos + 1);
	}

	/**
	 * Проверка параметров команды «connect-unix»
	 *
	 * @param const char * param
	 */
	void Client::check_param_connect_unix (const char * param)
	{
		if (param == nullptr)
		{
			this->err ("Необходимо путь к UNIX-сокету. Пример: «/var/run/cashka.sock»");
		}

		if (strtok (nullptr, " ") != nullptr)
		{
			this->err ("Параметры заданы неверно.");
		}

		char * _realpath = realpath(param, nullptr);
		if (_realpath == nullptr)
		{
			this->err ("Путь к UNIX-сокету задан неверно.");
		}

		this->unix_socket = param;
	}

	/**
	 * Показать информацию о соединении
	 */
	void Client::_connect_info ()
	{
		if (this->is_connect)
		{
			if (this->connect_type == "ip")
			{
				cout << this->host << ":" << this->port;
			}
			else if (this->connect_type == "unix")
			{
				cout << "unix:" << this->unix_socket_basename;
			}
		}

		cout << "> ";
		cout.flush();
	}
}
