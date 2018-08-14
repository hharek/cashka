#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../inc/err.h"
#include "opt.h"

#include "../../confi/confi.h"

/*
	process_title		- process-title		- string
	pid_file			- pid-file			- string
	foreground			- foreground		- bool
	host				- host				- string
	port				- port				- uint
	unix_socket			- unix-socket		- string
*/

/**
 * Получить опции по конфигурации
 */
int opt_cfg (struct opt * o)
{
	/* Структура для конфигурационного файла */
	struct opt o_cfg =
	{
		.process_title = NULL,
		.pid_file = NULL,
		.foreground = false,
		.host = NULL,
		.port = 0,
		.unix_socket = NULL
	};

	/* Парсим через confi */
	struct confi_param confi_params[] =
	{
		{ .name = "process_title",	.type = CONFI_TYPE_STRING	},
		{ .name = "pid_file",   	.type = CONFI_TYPE_STRING	},
		{ .name = "foreground",   	.type = CONFI_TYPE_BOOLEAN	},
		{ .name = "host",			.type = CONFI_TYPE_STRING	},
		{ .name = "port",			.type = CONFI_TYPE_UINT		},
		{ .name = "unix_socket",	.type = CONFI_TYPE_STRING	},
		NULL
	};

	int result = confi (o->config_file, confi_params);
	if (result != 0)
	{
		return err_set (CFG_PARSING_ERROR, confi_err ()->message);
	}

	/* Проверяем */

	/* Назначаем */

	return 0;
}


