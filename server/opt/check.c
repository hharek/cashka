#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "../inc/err.h"
#include "opt.h"

/**
 * Проверка параметров
 */
int opt_check (struct opt * o, const char * type)
{
	/* process_title */
	if (o->process_title != NULL)
	{
		if (strlen (o->process_title) > 32)
		{
			err_set (CFG_PROCESS_TITLE_BIG, NULL);
		}

		for (const char * c = o->process_title; *c; c++)
		{
			if
			(
				!isalpha(*c) &&
				!isalnum(*c) &&
				*c != '_' &&
				*c != '-'
			)
			{
				return err_set (CFG_PROCESS_TITLE_BAD_CHARSET, NULL);
			}
		}
	}

	/* command */
	if (o->command != NULL)
	{
		if
		(
			strcmp (o->command, "start") != 0 &&
			strcmp (o->command, "stop") != 0 &&
			strcmp (o->command, "restart") != 0 &&
			strcmp (o->command, "status") != 0
		)
		{
			return err_set (CLI_COMMAND_UNKNOWN, NULL);
		}
	}

	/* config_file */
	if (o->config_file != NULL)
	{
		char * _realpath = realpath(o->config_file, NULL);
		if (_realpath == NULL)
		{
			return err_set (CLI_CONFIG_NOT_EXIST, NULL);
		}
	}

	/* host */
	if (o->host != NULL)
	{
		if (o->unix_socket != NULL)
		{
			if (strcmp (type, "cli") == 0)
				return err_set (CLI_HOST_UNIX_SOCKET, NULL);
			else if (strcmp (type, "cfg") == 0)
				return err_set (CFG_HOST_UNIX_SOCKET, NULL);
		}
	}

	/* port */
	if (o->port != 0)
	{
		if (o->unix_socket != NULL)
		{
			if (strcmp (type, "cli") == 0)
				return err_set (CLI_HOST_UNIX_SOCKET, NULL);
			else if (strcmp (type, "cfg") == 0)
				return err_set (CFG_HOST_UNIX_SOCKET, NULL);
		}

		if (o->port < 1024 || o->port > 65535)
		{
			if (strcmp (type, "cli") == 0)
				return err_set (CLI_PORT_INCORRECT, NULL);
			else if (strcmp (type, "cfg") == 0)
				return err_set (CFG_PORT_INCORRECT, NULL);
		}
	}

	/* unix_socket */
	if (o->unix_socket != NULL)
	{
		if (o->host != NULL || o->port != 0)
		{
			if (strcmp (type, "cli") == 0)
				return err_set (CLI_HOST_UNIX_SOCKET, NULL);
			else if (strcmp (type, "cfg") == 0)
				return err_set (CFG_HOST_UNIX_SOCKET, NULL);
		}
	}

	return 0;
}