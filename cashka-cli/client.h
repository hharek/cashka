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
			bool is_connect_info = false;

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
			int select_timeout_usec = 100000;		/* Таймаут в микросекундах */

			/* Последний тип запроса */
			unsigned char query_type = 0x00;

		public:

			/* Конструктор и деструктор */
			Client (Options & options);
			~Client ();

			/* Ожидание комманд с клавиатуры и с устройства */
			void on ();

			/* Соединится по опциям */
			void connect_by_options ();

			/* Показать текст ошибки */
			void err (string message);

		private:

			/* Чтение данных с клавиатуры */
			void _read_stdin ();

			/* Читаем данные с сокета */
			void _read_socket ();

			/* Спарсить строку команды */
			void _parse_stdin (const char * buf);

			/* Соединится с сервером по протоколу IPv4/IPv6 */
			void _connect (const char * host, const char * port);

			/* Соединится по UNIX-сокету */
			void _connect_unix (const char * unix_socket);

			/* Закрыть соединение */
			void _close ();

			/* Отправить сообщение */
			void _send (const unsigned char * message, const unsigned int length);

			/* Проверка параметров команды */
			string * _parse_connect (const char * param);
			const char * _parse_connect_unix (const char * param);

			/* Показать */
			void _connect_info ();

			/* Показать справку */
			void _help ();

			/* Запрос «hello» */
			void _hello_send ();
			void _hello_read (unsigned char * buf);

			/* Запрос «set» */
			void _set_send (char * key, char * value);
			void _set_read (unsigned char * buf);

			/* Запрос «get» */
			void _get_send (char * key);
			void _get_read (unsigned char * buf);

			/* Запрос «isset» */
			void _isset_send (char * key);
			void _isset_read (unsigned char * buf);
	};
}
