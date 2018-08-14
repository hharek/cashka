#include <stddef.h>
#include <stdio.h>

#include "../inc/err.h"
#include "opt.h"

/**
 * Получить опции
 */
int opt (int argc, char ** argv, struct opt * o)
{
	/* Инициализируем по умолчанию */
	o->process_title = NULL;
	o->config_file = NULL;
	o->foreground = false;
	o->pid_file = NULL;
	o->host = NULL;
	o->port = 0;
	o->unix_socket = NULL;
	o->command = NULL;

	/* CLI */
	if (opt_cli (argc, argv, o))
		return err_get()->code;

	/* Конфигурационный файл */
	if (o->config_file != NULL)
	{
		if (opt_cfg (o))
			return err_get()->code;
	}



	return 0;
}


