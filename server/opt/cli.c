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

extern int argc;
extern char ** argv;

/**
 * Получить опции по CLI
 */
struct opt * opt_cli ()
{
	/* Инициализируем структуру */
	struct opt * o = opt_init ();

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
				{
					err_set (CLI_PORT_INCORRECT, NULL);
					opt_free (o);
					return NULL;
				}
				break;

			case 'u':
				o->unix_socket = strdup (optarg);
				break;

			case '?':
				err_set (CLI_UNKNOWN_OPTION, NULL);
				opt_free (o);
				return NULL;

			default:
				err_set (CLI_UNKNOWN_OPTION, NULL);
				opt_free (o);
				return NULL;
		}

		opt = getopt_long (argc, argv, cli_getopt, cli_getopt_long, &index);
	}

	/* Определяем команду */
	char * command = NULL;

	if (optind >= argc)
	{
		err_set (CLI_NOT_COMMAND, NULL);
		opt_free (o);
		return NULL;
	}


	while (optind < argc)
	{
		if (command != NULL)
		{
			err_set (CLI_MANY_COMMAND, NULL);
			opt_free (o);
			return NULL;
		}

		command = argv[optind];
		optind++;
	}

	o->command = strdup (command);

	/* Проверяем */
	int result = opt_check (o, "cli");
	if (result != 0)
	{
		opt_free (o);
		return NULL;
	}

	return o;
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
