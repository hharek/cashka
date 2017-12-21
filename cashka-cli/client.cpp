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

#include "../query/query.h"
#include "../query/hello.h"
#include "../query/set.h"
#include "../query/get.h"
#include "../query/isset.h"

using std::cout;
using std::endl;
using std::string;

namespace cashka_cli
{
	/**
	 * Конструктор
	 */
	Client::Client (Options & options) : options (options)
	{
		if (options.get_unix_socket().empty())
		{
			this->host = options.get_host ();
			this->port = std::to_string (options.get_port ());
		}
		else
		{
			this->unix_socket = options.get_unix_socket ();
		}

		/* Поготавливаем множества для прослушки */
		FD_ZERO (&this->fd_read);
		FD_ZERO (&this->fd_all);

		/* Добавляем клавиатуру во множество */
		FD_SET (STDIN_FILENO, &fd_all);
	}

	/**
	 * Деструктор
	 */
	Client::~Client ()
	{
		this->_close ();
	}

	/**
	 * Соединится по опциям
	 */
	void Client::connect_by_options ()
	{
		try
		{
			if (!this->unix_socket.empty ())
			{
				this->_connect_unix (this->unix_socket.c_str ());
			}
			else if (!this->host.empty () && !this->port.empty ())
			{
				this->_connect (this->host.c_str (), this->port.c_str ());
			}
		}
		catch (string & message)
		{
			cout << "Ошибка: " << message << endl;
		}
	}

	/**
	 * Ожидание команд от клавиатуры и клиентского сокета
	 */
	void Client::on ()
	{
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

			/* Данные не пришли на сокеты и на клавиатуру */
			if (select_result < 1)
			{
				this->_connect_info ();
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
						this->_read_stdin ();
					}
					/* Данные с сокета */
					else if (fd == this->socket)
					{
						this->_read_socket ();
					}
				}
				catch (string & message)
				{
					cout << "Ошибка: " << message << endl;
				}
			}

			/* Информация о соединении стёрта */
			this->is_connect_info = false;
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
	void Client::_read_stdin ()
	{
		char buf[512] = { 0 };
		int read_size = read (STDIN_FILENO, buf, 512);

		if (read_size == -1)
		{
			this->err ("Невозможно прочитать данные с клавиатуры.");
		}

		buf[strlen (buf)-1] = '\0';

		/* Выход */
		if ((string)buf == "exit" || (string)buf == "quit")
		{
			if (this->is_connect)
			{
				this->_close ();
			}

			exit (EXIT_SUCCESS);
		}

		/* Парсим и выполняем команду */
		this->_parse_stdin (buf);
	}

	/**
	 * Читаем данные с сокета
	 */
	void Client::_read_socket ()
	{
		unsigned char buf[512] = {0};
		int recv_size = recv (this->socket, buf, 512, 0);

		if (recv_size == 0)
		{
			cout << "Сервер закрыл соединение." << endl;
			this->_close ();
		}
		else if (recv_size < 0)
		{
			this->err ((string)"Не удалось получить данные с сервера. Подробнее: " + strerror(errno));
			this->_close ();
		}
		else if (recv_size < 5)
		{
			this->err ("Слишком маленькое сообщение.");
		}

		bool result = (bool)buf[0];

		/* Читаем сообщение */
		if (result)
		{
			switch (this->query_type)
			{
				/* hello */
				case query::hello::TYPE:
				{
					this->_hello_read (buf);
				}
				break;

				/* set */
				case query::set::TYPE:
				{
					this->_set_read (buf);
				}
				break;

				/* get */
				case query::get::TYPE:
				{
					this->_get_read (buf);
				}
				break;

				/* isset */
				case query::isset::TYPE:
				{
					this->_isset_read (buf);
				}
				break;
			}
		}
		/* Сообщение об ошибке */
		else
		{
			this->err (query::err_parse (buf));
		}
	}

	/**
	 * Спарсить команду
	 *
	 * @param const char * command
	 */
	void Client::_parse_stdin (const char * buf)
	{
		char * str = strdup (buf);

		char * command = strtok (str, " ");

		if (command == nullptr)
		{
			this->err ("Не задана команда.");
		}

		if ((string)command == "help")
		{
			this->_help ();
		}
		else if ((string)command == "connect")
		{
			char * param = strtok (nullptr, " ");
			string * ip_port = this->_parse_connect (param);
			this->_connect (ip_port[0].c_str(), ip_port[1].c_str());
			delete[] ip_port;
		}
		else if ((string)command == "connect-unix")
		{
			char * param = strtok (nullptr, " ");
			const char * unix_socket = this->_parse_connect_unix (param);
			this->_connect_unix (unix_socket);
		}
		else if ((string)command == "close")
		{
			this->_close ();
		}
		else if ((string)command == "hello")
		{
			this->query_type = query::hello::TYPE;
			this->_hello_send ();
		}
		else if ((string)command == "set")
		{
			char * key = strtok (nullptr, " ");
			char * value = strtok (nullptr, " ");

			if (key == nullptr || value == nullptr)
			{
				this->err ("Параметры заданы неверно.");
			}

			if (strtok (nullptr, " ") != nullptr)
			{
				this->err ("Параметры заданы неверно.");
			}

			this->query_type = query::set::TYPE;
			this->_set_send (key, value);
		}
		else if ((string)command == "get")
		{
			char * key = strtok (nullptr, " ");

			if (key == nullptr)
			{
				this->err ("Параметры заданы неверно.");
			}

			if (strtok (nullptr, " ") != nullptr)
			{
				this->err ("Параметры заданы неверно.");
			}

			this->query_type = query::get::TYPE;
			this->_get_send (key);
		}
		else if ((string)command == "isset")
		{
			char * key = strtok (nullptr, " ");

			if (key == nullptr)
			{
				this->err ("Параметры заданы неверно.");
			}

			if (strtok (nullptr, " ") != nullptr)
			{
				this->err ("Параметры заданы неверно.");
			}

			this->query_type = query::isset::TYPE;
			this->_isset_send (key);
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
	void Client::_connect (const char * host, const char * port)
	{
		cout << "Соединяюсь по адресу «" << host << ":" << port << "»" << endl;

		/* Создаём сокет IP и подключаемся */
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

		int socket_connect = 0;
		if ((socket_connect = ::socket (addrinfo->ai_family, addrinfo->ai_socktype, addrinfo->ai_protocol)) == -1)
		{
			this->err ((string)"Не удалось подключиться. Подробнее: " + strerror(errno));
		}

		if (::connect (socket_connect, addrinfo->ai_addr, addrinfo->ai_addrlen) == -1)
		{
			this->err ((string)"Не удалось подключиться. Подробнее: " + strerror(errno));
		}

		/* Если был подключён ранее, отключаемся со старого */
		if (this->is_connect)
		{
			this->_close ();
		}

		/* Обозначаем подключение */
		this->socket = socket_connect;
		this->is_connect = true;
		this->connect_type = "ip";

		FD_SET (this->socket, &this->fd_all);
		this->fd_max = this->socket + 1;

		/* Прописать хост на основании getaddrinfo */
		void * addr;
		char ip_str[INET6_ADDRSTRLEN];

		if (addrinfo->ai_family == AF_INET)
		{
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)addrinfo->ai_addr;
			addr = &(ipv4->sin_addr);
		}
		else if (addrinfo->ai_family == AF_INET6)
		{
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)addrinfo->ai_addr;
			addr = &(ipv6->sin6_addr);
		}

		inet_ntop (addrinfo->ai_family, addr, ip_str, sizeof ip_str);
		this->host = ip_str;
		this->port = port;

		cout << "Соединение установлено." << endl;
	}

	/**
	 * Соединится по UNIX-сокету
	 *
	 * @param const char * unix_socket
	 */
	void Client::_connect_unix (const char * unix_socket)
	{
		cout << "Соединяюсь с unix-сокетом «" << unix_socket << "»" << endl;

		/* Создаём unix-сокет и подключаемся */
		struct sockaddr_un hints;
		memset (&hints, 0, sizeof (struct sockaddr_un));
		hints.sun_family = AF_LOCAL;
		strncpy (hints.sun_path, unix_socket, sizeof (hints.sun_path) - 1);

		int socket_connect;
		if ((socket_connect = ::socket (AF_UNIX, SOCK_STREAM, 0)) == -1)
		{
			this->err ((string)"Не удалось подключиться. Подробнее: " + strerror(errno));
		}

		if (::connect (socket_connect, (struct sockaddr *)&hints, sizeof(struct sockaddr_un)) == -1)
		{
			this->err ((string)"Не удалось подключиться. Подробнее: " + strerror(errno));
		}

		/* Если был подключён ранее, отключаемся со старого */
		if (this->is_connect)
		{
			this->_close ();
		}

		/* Обозначаем подключение */
		this->socket = socket_connect;
		this->is_connect = true;
		this->connect_type = "unix";
		this->unix_socket = unix_socket;
		this->unix_socket_basename = (string)basename (unix_socket);

		FD_SET (this->socket, &this->fd_all);
		this->fd_max = this->socket + 1;

		cout << "Соединение установлено." << endl;
	}

	/**
	 * Закрыть соединение
	 */
	void Client::_close ()
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
	 * @param unsigned int length
	 */
	void Client::_send (const unsigned char * message, const unsigned int length)
	{
		if (!this->is_connect)
		{
			this->err ("Вам необходимо сначала подключиться к серверу, чтобы отправлять сообщение.");
		}

		int send_size = ::send (this->socket, message, length, 0);
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
	string * Client::_parse_connect (const char * param)
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

		string * ip_port_ar = new string[2];
		ip_port_ar[0] = ip_port.substr (0, pos);
		ip_port_ar[1] = ip_port.substr (pos + 1);

		return ip_port_ar;
	}

	/**
	 * Проверка параметров команды «connect-unix»
	 *
	 * @param const char * param
	 */
	const char * Client::_parse_connect_unix (const char * param)
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

		return param;
	}

	/**
	 * Показать информацию о соединении
	 */
	void Client::_connect_info ()
	{
		if (this->is_connect_info)
		{
			return;
		}

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

		this->is_connect_info = true;
	}

	/**
	 * Показать справку
	 */
	void Client::_help ()
	{
		std::cout << "cashka-cli " << options.get_version() << endl;

		std::cout << R"(
Команды:
  connect           - соединиться с сервером по протоколу IPv4/IPv6
  connect-unix      - соединиться с сервером по unix-сокету
  close             - закрыть текущее соединение
  quit (exit)       - выход
  hello             - узнать версию сервера и проверить соответствие версий
  set               - создать ключ со значением
  get               - получить значение ключа
  isset             - проверка наличия ключа

Примеры:
  connect 127.0.0.1:3000
  connect ::1:3000
  connect-unix cashka.sock
  hello
  set key1 value1
  get key1
  isset key1
  close
  quit (exit)
)";
	}

	/**
	 * Запрос «hello». Отправить
	 */
	void Client::_hello_send ()
	{
		query::hello::Request hello;
		auto result = hello.make ();

		this->_send (result.content, result.length);

		/* Очистить */
		delete[] result.content;
	}

	/**
	 * Запрос «hello». Прочитать ответ
	 */
	void Client::_hello_read (unsigned char * buf)
	{
		query::hello::Response hello;
		query::hello::Response::data data;

		try
		{
			data = hello.parse (buf);
			if (options.get_version() != (string)data.version)
			{
				cout << "Версии клиента и сервера не совпадают. Соряныч :(" << endl;
				this->_close();
			}

			cout << "Сервер: " << data.name << " " << data.version << endl;
		}
		catch (const char * error)
		{
			this->err (error);
		}

		/* Очистить */
		delete[] data.name;
		delete[] data.version;
	}

	/**
	 * Запрос «set». Отправить
	 */
	void Client::_set_send (char * key, char * value)
	{
		query::set::Request set;
		query::result result;

		try
		{
			result = set.make (key, value);
			this->_send (result.content, result.length);
		}
		catch (const char * error)
		{
			this->err (error);
		}

		/* Очистить */
		delete[] result.content;
	}

	/**
	 * Запрос «set». Прочитать ответ
	 */
	void Client::_set_read (unsigned char * buf)
	{
		query::set::Response set;
		query::set::Response::data data;

		try
		{
			data = set.parse (buf);
			cout << "Переменная назначена." << endl;
		}
		catch (const char * error)
		{
			this->err (error);
		}
	}

	/**
	 * Запрос «get». Отправить
	 */
	void Client::_get_send (char * key)
	{
		query::get::Request get;
		query::result result;

		try
		{
			result = get.make (key);
			this->_send (result.content, result.length);
		}
		catch (const char * error)
		{
			this->err (error);
		}

		/* Очистить */
		delete[] result.content;
	}

	/**
	 * Запрос «get». Прочитать ответ
	 */
	void Client::_get_read (unsigned char * buf)
	{
		query::get::Response get;
		query::get::Response::data data;

		try
		{
			data = get.parse (buf);
			if (data.isset)
			{
				cout << data.value << endl;
			}
			else
			{
				cout << "Указанный ключ отсутствует." << endl;
			}
		}
		catch (const char * error)
		{
			this->err (error);
		}

		/* Очистить */
		delete[] data.value;
	}

	/**
	 * Запрос «isset». Отправить
	 */
	void Client::_isset_send (char * key)
	{
		query::isset::Request isset;
		query::result result;

		try
		{
			result = isset.make (key);
			this->_send (result.content, result.length);
		}
		catch (const char * error)
		{
			this->err (error);
		}

		/* Очистить */
		delete[] result.content;
	}

	/**
	 * Запрос «isset». Прочитать ответ
	 */
	void Client::_isset_read (unsigned char * buf)
	{
		query::isset::Response isset;
		query::isset::Response::data data;

		try
		{
			data = isset.parse (buf);
			if (data.isset)
			{
				cout << "Ключ присутствует." << endl;
			}
			else
			{
				cout << "Ключ отсутствует." << endl;
			}
		}
		catch (const char * error)
		{
			this->err (error);
		}
	}
}