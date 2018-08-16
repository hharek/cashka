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
	/* Инициализируем структуру нулями */
	struct opt o_cli;
	o_cli.process_title = NULL;
	o_cli.config_file = NULL;
	o_cli.foreground = false;
	o_cli.pid_file = NULL;
	o_cli.host = NULL;
	o_cli.port = 0;
	o_cli.unix_socket = NULL;
	o_cli.command = NULL;

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
				o_cli.config_file = strdup (optarg);
				break;

			case 'f':
				o_cli.foreground = true;
				break;

			case 'p':
				o_cli.pid_file = strdup (optarg);
				break;

			case 'H':
				o_cli.host = strdup (optarg);
				break;

			case 'P':
				o_cli.port = (unsigned int)strtoul (optarg, &strtoul_end, 0);
				if (*strtoul_end)
					return err_set (CLI_PORT_INCORRECT, NULL);
				break;

			case 'u':
				o_cli.unix_socket = strdup (optarg);
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

	o->command = strdup (command);

	/* Проверяем */
	int result = opt_check (&o_cli, "cli");
	if (result != 0)
		return result;

	/* Назначаем */
	if (o_cli.config_file != NULL)
		o->config_file = o_cli.config_file;

	if (o_cli.foreground == true)
		o->foreground = true;

	if (o_cli.pid_file != NULL)
		o->pid_file = o_cli.pid_file;

	if (o_cli.host != NULL)
		o->host = o_cli.host;

	if (o_cli.port != 0)
		o->port = o_cli.port;

	if (o_cli.unix_socket != NULL)
		o->unix_socket = o_cli.unix_socket;

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
