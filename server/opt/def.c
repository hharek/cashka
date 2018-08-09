#include <stdbool.h>

#include "opt.h"

/**
 * Опции по умолчанию
 */
struct opt opt_def ()
{
	return (struct opt)
	{
		.process_title = "cashka",
		.command = "start",
		.config_file = "cashka.json",
		.foreground = false,
		.pid_file = "cashka.pid",
		.host = "localhost",
		.port = 3000,
		.unix_socket = ""
	};
}