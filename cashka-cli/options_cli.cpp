#include <iostream>
#include <fstream>
#include <getopt.h>
#include <stdlib.h>

#include "cashka-cli.h"
#include "options.h"


extern int opterr;

namespace cashka_cli
{

	/**
	 * Получить опции командной строки
	 * 
	 * @param int argc		- стандартный argc из функции main
	 * @param char ** argv	- стандартный argv из функции main
	 * @return void
	 */
	void Options::cli (int argc, char ** argv)
	{
		/* Парсим опции */
		opterr = 0;
		int opt = 0;
		int index = 0;

		opt = getopt_long (argc, argv, this->cli_getopt, this->cli_getopt_long, &index);

		while (opt != -1)
		{
			switch (opt)
			{
				case 'h':
				{
					this->cli_help();
				}
					break;

				case 'v':
				{
					this->cli_version();
				}
					break;

				case 'c':
				{
					this->cli_config_file_set (optarg);
				}
					break;

				case 'H':
				{
					this->cli_host_set (optarg);
				}
					break;

				case 'P':
				{
					this->cli_port_set (optarg);
				}
					break;

				case 'u':
				{
					this->cli_unix_socket_set (optarg);
				}
					break;

				case '?':
				{
					err ("Неизвестная опция.", "cli");
				}
				break;
			}

			opt = getopt_long (argc, argv, this->cli_getopt, this->cli_getopt_long, &index);
		}
	}

	/**
	 * Показать помощь по командной строке и выйти
	 * 
	 * @return void
	 */
	void Options::cli_help ()
	{
		std::cout << R"(Применение: cashka-cli [ОПЦИИ]
Опции:
  -h    --help          - показать справку
  -v    --version       - показать версию программы и выйти
  -c    --config        - указать конфигурационный файл
  -H    --host          - указать хост (по умолчанию: 127.0.0.1)
  -P    --port          - указать порт (по умолчанию: 3000)
  -u    --unix-socket   - указать путь к unix-сокету

Примеры:
  cashka-cli -h
  cashka-cli --version
  cashka-cli -c my.json
  cashka-cli -H 127.0.0.1 -P 3000
  cashka-cli -H ::1 -P 3000
  cashka-cli --unix-socket=cashka.sock
)";

		exit (EXIT_SUCCESS);
	}

	/**
	 * Показать версию программы и выйти
	 * 
	 * @return void
	 */
	void Options::cli_version ()
	{
		std::cout << this->version << std::endl;
		exit (EXIT_SUCCESS);
	}

	/**
	 * Проверить и назначить конфигурационный файл
	 * 
	 * @param const char * config_file
	 * @return void
	 */
	void Options::cli_config_file_set (const char * config_file)
	{
		this->check_config_file (config_file);
		this->cli_config_file = config_file;
	}

	/**
	 * Назначить хост
	 * 
	 * @param const char * host
	 * @return void
	 */
	void Options::cli_host_set (const char * host)
	{
		this->cli_host = host;
	}

	/**
	 * Проверить и назначить порт
	 * 
	 * @param const char * port
	 * @return void
	 */
	void Options::cli_port_set (const char * port)
	{
		int port_int = atoi(port);

		this->check_port (port_int, "cli");

		this->cli_port = (unsigned int)port_int;
	}

	/**
	 * Назначить unix-сокет
	 * 
	 * @param const char * unix_socket
	 * @return void
	 */
	void Options::cli_unix_socket_set (const char * unix_socket)
	{
		this->cli_unix_socket = unix_socket;
	}
}