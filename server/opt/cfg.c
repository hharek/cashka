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
struct opt * opt_cfg (char * file)
{
	/* Инициализируем структуру */
	struct opt * o = opt_init ();

	/* Нет файла */
	if (file == NULL)
	{
		opt_free (o);
		return NULL;
	}

	int result;

	/* Парсим через confi */
	struct confi_param confi_params[] =
	{
		{ .name = "process_title",	.type = CONFI_TYPE_STRING,	.ptr = &o->process_title	},
		{ .name = "pid_file",   	.type = CONFI_TYPE_STRING,	.ptr = &o->pid_file			},
		{ .name = "foreground",   	.type = CONFI_TYPE_BOOLEAN,	.ptr = &o->foreground		},
		{ .name = "host",			.type = CONFI_TYPE_STRING,	.ptr = &o->host				},
		{ .name = "port",			.type = CONFI_TYPE_UINT,	.ptr = &o->port				},
		{ .name = "unix_socket",	.type = CONFI_TYPE_STRING,	.ptr = &o->unix_socket		},
		NULL
	};

	result = confi (file, confi_params);
	if (result != 0)
	{
		err_set (CFG_PARSING_ERROR, confi_err ()->message);
		opt_free (o);
		return NULL;
	}

	/* Проверяем */
	result = opt_check (o, "cfg");
	if (result != 0)
	{
		opt_free (o);
		return NULL;
	}

	return o;
}


