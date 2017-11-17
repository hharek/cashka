#pragma once

namespace cashka_cli
{
	/**
	 * Клиент генерирующий запросы на сервер
	 */
	class Client
	{
		private:

			/* Доступ к опциям */
			Options & options;

			/* Хост */
			string host;

			/* Порт */
			string port;

			/* UNIX-сокет */
			string unix_socket;

			/* Базовое имя UNIX-сокета */
			string unix_socket_basename;

			/* Установленно ли соединение */
			bool is_connect = false;

			/* Тип соединения (ip, unix) */
			string connect_type;

			/* Сокет соединения */
			int socket;

			/* Параметры для команды select */
			fd_set fd_all;							/* Множества, которые должен прослушивать select (stdin, socket) */
			fd_set fd_read;							/* Множества, которые прослушивается select */
			int fd_max = 1;							/* Макс. дескриптор для select */
			struct timeval select_timeout;			/* Таймаут */
			int select_timeout_sec = 0;				/* Таймаут в секундах */
			int select_timeout_usec = 200000;		/* Таймаут в микросекундах */

		public:

			/* Конструктор и деструктор */
			Client (Options & options);
			~Client ();

			/* Ожидание комманд с клавиатуры и с устройства */
			void on ();

			/* Показать текст ошибки */
			void err (string message);

		private:

			/* Чтение данных с клавиатуры */
			void read_stdin ();

			/* Читаем данные с сокета */
			void read_socket ();

			/* Спарсить строку команды */
			void parse (const char * buffer);

			/* Соединится с сервером по протоколу IPv4/IPv6 */
			void connect (const char * host, const char * port);

			/* Соединится по UNIX-сокету */
			void connect_unix (const char * unix_socket);

			/* Закрыть соединение */
			void close ();

			/* Отправить сообщение */
			void send (const char * msg);

			/* Проверка параметров команды */
			void check_param_connect (const char * param);
			void check_param_connect_unix (const char * param);

			/* Показать */
			void _connect_info ();
	};
}
