#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../inc/cashka.h"
#include "opt.h"

/**
 * Опции по умолчанию
 */
struct opt * opt_def ()
{
	struct opt * o = opt_init ();

	o->process_title = strdup (CASHKA_PROCESS_TITLE);
	o->config_file = strdup (CASHKA_CONFIG_FILE);
	o->foreground = CASHKA_FOREGROUND;
	o->pid_file = strdup (CASHKA_PID_FILE);
	o->host = strdup (CASHKA_HOST);
	o->port = CASHKA_PORT;
	o->unix_socket = strdup (CASHKA_UNIX_SOCKET);

	return o;
}