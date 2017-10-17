#include <iostream>
#include <fstream>
#include <string>
#include <getopt.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <cstdlib>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "cashka.h"
#include "options.h"


extern int opterr;

namespace cashka
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
		/* Сохраняем оригинальные argc, argv для смены заголовка */
		this->argv = argv;
		this->argc = argc;
		
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

				case 'f':
				{
					this->cli_foreground_set = true;
				}
				break;

				case 'p':
				{
					this->cli_pid_file_set (optarg);
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

				case '?':
				{
					err ("Неизвестная опция.", "cli");
				}
				break;
			}

			opt = getopt_long (argc, argv, this->cli_getopt, this->cli_getopt_long, &index);
		}

		/* Определяем команду */
		char * command = nullptr;

		if (optind >= argc)
		{
			err ("Не указана команда. Используйте: «start», «stop», «restart», «status».", "cli");
		}

		while (optind < argc)
		{
			if (command != nullptr)
			{
				err ("Указано несколько команд.", "cli");
			}

			command = argv[optind];
			optind++;
		}

		this->cli_command_set (command);

	}

	/**
	 * Проверить и назначить команду
	 * 
	 * @param const char * command
	 * @return void
	 */
	void Options::cli_command_set (const char * command)
	{
		this->check_command (command);
		this->command = command;
	}

	/**
	 * Показать помощь по командной строке и выйти
	 * 
	 * @return void
	 */
	void Options::cli_help ()
	{
		std::cout << R"(Применение: cashka [ОПЦИИ] КОМАНДА
Опции:
  -h    --help          - показать справку
  -v    --version       - показать версию программы и выйти
  -p    --pid-file      - указать pid-файл
  -c    --config        - указать конфигурационный файл
  -f    --foreground    - запустить процесс на переднем плане (не в фоне)
  -H    --host          - указать хост (по умолчанию: 127.0.0.1)
  -P    --port          - указать порт (по умолчанию: 3000)

Команды:
  start     - запуск сервера 
  stop      - стоп сервера
  restart   - перезагрузить сервер
  status    - статус сервера

Примеры:
  cashka -h
  cashka --version
  cashka -p my.pid -c my.json start
  cashka stop
  cashka restart
  cashka status
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
	 * Назначить pid-файл
	 * 
	 * @param const char * pid_file
	 * @return void
	 */
	void Options::cli_pid_file_set (const char * pid_file)
	{
		this->cli_pid_file = pid_file;
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

}