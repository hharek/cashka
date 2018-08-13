#include <stdbool.h>
#include <stdio.h>

#include "../inc/cashka.h"
#include "opt.h"

/**
 * Опции по умолчанию
 */
int opt_def (struct opt * o)
{
	o->process_title = CASHKA_PROCESS_TITLE;
	o->command = CASHKA_COMMAND;
	o->config_file = CASHKA_CONFIG_FILE;
	o->foreground = CASHKA_FOREGROUND;
	o->pid_file = CASHKA_PID_FILE;
	o->host = CASHKA_HOST;
	o->port = CASHKA_PORT;
	o->unix_socket = CASHKA_UNIX_SOCKET;

	return 0;
}