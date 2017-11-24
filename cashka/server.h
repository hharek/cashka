#pragma once

#include <iostream>
#include <cstring>
#include <string>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

namespace cashka
{
	/**
	 * Серверные функции
	 */
	class Server
	{
		private:
			
			/* Доступ к опциям */
			Options & options;
			
			/* Информация по хосту. Полученное от функции getaddrinfo */
			struct addrinfo * addrinfo = nullptr;
			
			/* Массив мастер-сокетов */
			int * master = nullptr; 
			
			/* Кол-во мастер-сокетов */
			int master_count = 0;
			
			/* Множество дескрипторов сокетов, отслеживаемых на чтение */
			fd_set fd_read;
			
			/* Множество всех дескрипторов сокетов */
			fd_set fd_all;
			
			/* Значение максимального дескриптора */
			int fd_max = 0;
			
			/* Буффер */
			char buffer[255];
			
			/* Таймаут для select */
			struct timeval select_timeout;
			int select_timeout_sec = 2;
			int select_timeout_usec = 0;
			
			/* Приветствие */
			const char * msg_hello = "Здарова. Я простой tcp-echo сервак.\n";
			
		public:
			
			/* Конструктор */
			Server (Options & options);
			
			/* Создать сокет */
			void start ();
			
			/* Закрыть сокет */
			void stop ();
			
			/* Информация по IP/TCP сокету  */
			string socket_ip_info ();
			
		private:
			
			/* Создать IP/TCP сокет */
			void socket_ip_create ();
			
			/* Создать unix-сокет */
			void socket_unix_create ();
			
			/* Отлавливать соединения через select */
			void socket_select ();
			
			/* Добавить нового клиента */
			void client_add (int master);
			
			/* Отключить клиента */
			void client_close (int socket);
			
			/* Прочитать сообщение */
			void _read (int socket);

			/* Отправить сообщение */
			void _send (int socket, char * message, unsigned int length);

			/* Пришёл запрос «hello» */
			void _hello (int socket, char * buffer);
	};
}

