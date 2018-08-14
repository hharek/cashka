#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/cashka.h"
#include "../inc/err.h"
#include "opt.h"

/*
    help			--help			-h
    version         --version		-v
	config_file		--config 		-c
	foreground		--foreground 	-f
	pid_file		--pid-file 		-p
	host			--host 			-H
	port			--port			-P
	unix_socket		--unix-socket	-u

    command
*/

/**
 * Получить опции по CLI
 */
int opt_cli (int argc, char ** argv, struct opt * o)
{
	/* Опции для функции getopt () */
	const char * cli_getopt = "hvc:fp:H:P:u:";

	/* Опции для функции getopt_long () */
	struct option cli_getopt_long[] =
	{
		{ "help",			no_argument,		NULL, 'h' },
		{ "version",		no_argument,		NULL, 'v' },
		{ "config",			required_argument,	NULL, 'c' },
		{ "foreground",		no_argument,		NULL, 'f' },
		{ "pid-file",		required_argument,	NULL, 'p' },
		{ "host",			required_argument,	NULL, 'H' },
		{ "port",			required_argument,	NULL, 'P' },
		{ "unix-socket",	required_argument,	NULL, 'u' },
		NULL
	};

	/* Парсим опции */
	opterr = 0;
	int opt = 0;
	int index = 0;

	opt = getopt_long (argc, argv, cli_getopt, cli_getopt_long, &index);

	char * strtoul_end;
	while (opt != -1)
	{
		switch (opt)
		{
			case 'h':
				opt_cli_help ();
				break;

			case 'v':
				opt_cli_version ();
				break;

			case 'c':
				o->config_file = strdup (optarg);
				break;

			case 'f':
				o->foreground = true;
				break;

			case 'p':
				o->pid_file = strdup (optarg);
				break;

			case 'H':
				o->host = strdup (optarg);
				break;

			case 'P':
				o->port = (unsigned int)strtoul (optarg, &strtoul_end, 0);
				if (*strtoul_end)
					return err_set (CLI_PORT_INCORRECT, NULL);
				break;

			case 'u':
				o->unix_socket = strdup (optarg);
				break;

			case '?':
				return err_set (CLI_UNKNOWN_OPTION, NULL);
				break;

			default:
				return err_set (CLI_UNKNOWN_OPTION, NULL);
				break;
		}

		opt = getopt_long (argc, argv, cli_getopt, cli_getopt_long, &index);
	}

	/* Определяем команду */
	char * command = NULL;

	if (optind >= argc)
		return err_set (CLI_NOT_COMMAND, NULL);

	while (optind < argc)
	{
		if (command != NULL)
			return err_set (CLI_MANY_COMMAND, NULL);

		command = argv[optind];
		optind++;
	}

	if
	(
		strcmp (command, "start") != 0 &&
		strcmp (command, "stop") != 0 &&
		strcmp (command, "restart") != 0 &&
		strcmp (command, "status") != 0
	)
	{
		return err_set (CLI_COMMAND_UNKNOWN, NULL);
	}

	o->command = strdup (command);

//	/* Отладочные сообщения */
//	printf ("config_file: %s\n", o->config_file);
//	printf ("foreground: %i\n", o->foreground);
//	printf ("pid_file: %s\n", o->pid_file);
//	printf ("host: %s\n", o->host);
//	printf ("port: %i\n", o->port);
//	printf ("unix_socket: %s\n", o->unix_socket);
//	printf ("command: %s\n", o->command);

	return 0;
}

/**
 * Вывод команды --help (-h)
 */
void opt_cli_help ()
{
	const char * str = R"(Применение: cashka [ОПЦИИ] КОМАНДА
Опции:
  -h    --help          - показать справку
  -v    --version       - показать версию программы и выйти
  -c    --config        - указать конфигурационный файл
  -f    --foreground    - запустить процесс на переднем плане (не в фоне)
  -p    --pid-file      - указать pid-файл
  -H    --host          - указать хост (по умолчанию: 127.0.0.1)
  -P    --port          - указать порт (по умолчанию: 3000)
  -u    --unix-socket   - указать путь к unix-сокету

Команды:
  start     - запуск сервера
  stop      - стоп сервера
  restart   - перезагрузить сервер
  status    - статус сервера

Примеры:
  cashka -h
  cashka --version
  cashka -p my.pid -c my.json start
  cashka -H 127.0.0.1 -P 3000
  cashka --unix-socket=cashka.sock
  cashka stop
  cashka restart
  cashka status
)";

	printf (str);
	exit (EXIT_SUCCESS);
}

/**
 * Вывод команды --version (-v)
 */
void opt_cli_version ()
{
	printf ("%s %s\n", CASHKA_SERVER_NAME, CASHKA_VERSION);
	exit (EXIT_SUCCESS);
}
