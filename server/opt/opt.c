#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/cashka.h"
#include "../inc/err.h"
#include "opt.h"

/**
 * Получить опции
 */
struct opt * opt ()
{
	/* CLI */
	struct opt * o_cli = opt_cli ();
	if (o_cli == NULL)
		return NULL;

	/* Конфигурационный файл */
	struct opt * o_cfg = opt_cfg (o_cli->config_file);
	if (o_cfg == NULL)
		return NULL;

	/* По умолчанию */
	struct opt * o_def = opt_def ();

	/* Назначаем (cli || cfg || def) */
	struct opt * o = opt_join (o_cli, o_cfg, o_def);

	/* Очищаем структуры */
	opt_free (o_cli);
	opt_free (o_cfg);
	opt_free (o_def);

	return o;
}

/**
 * Инициализируем структуру нулями
 */
struct opt * opt_init ()
{
	struct opt * o = malloc (sizeof (struct opt));

	o->process_title = NULL;
	o->config_file = NULL;
	o->foreground = false;
	o->pid_file = NULL;
	o->host = NULL;
	o->port = 0;
	o->unix_socket = NULL;
	o->command = NULL;

	return o;
}

/**
 * Освобождаем структуру opt
 */
void opt_free (struct opt * o)
{
	if (o == NULL)
		return;

	if (o->process_title != NULL)
		free (o->process_title);

	if (o->config_file != NULL)
		free (o->config_file);

	if (o->pid_file != NULL)
		free (o->pid_file);

	if (o->host != NULL)
		free (o->host);

	if (o->unix_socket != NULL)
		free (o->unix_socket);

	if (o->command != NULL)
		free (o->command);

	free (o);
}

/**
 * Объединить опции (cli || cfg || def)
 */
struct opt * opt_join (struct opt * o_cli, struct opt * o_cfg, struct opt * o_def)
{
	struct opt * o = opt_init ();

	/* process_title (cfg || def) */
	if (o_cfg->process_title)
		o->process_title = strdup (o_cfg->process_title);
	else
		o->process_title = strdup (o_def->process_title);

	/* command (cli) */
	o->command = strdup (o_cli->command);

	/* config_file (cli || def) */
	if (o_cli->config_file)
		o->config_file = strdup (o_cli->config_file);
	else
		o->config_file = strdup (o_def->config_file);

	/* foreground (cli || cfg || def) */
	if (o_cli->foreground)
		o->foreground = o_cli->foreground;
	else if (o_cfg->foreground)
		o->foreground = o_cfg->foreground;
	else
		o->foreground = o_def->foreground;

	/* pid_file (cli || cfg || def) */
	if (o_cli->pid_file)
		o->pid_file = strdup (o_cli->pid_file);
	else if (o_cfg->pid_file)
		o->pid_file = strdup (o_cfg->pid_file);
	else
		o->pid_file = strdup (o_def->pid_file);

	/* Определяем, что прописать «host + port» или «unix_socket» */
	const char * type;
	if (o_cli->host || o_cli->port)
		type = "host+port";
	else if (o_cli->unix_socket)
		type = "unix-socket";
	else if (o_cfg->host || o_cfg->port)
		type = "host+port";
	else if (o_cfg->unix_socket)
		type = "unix-socket";
	else
		type = "host+port";

	if (strcmp (type, "host+port") == 0)
	{
		/* host (cli || cfg || def) */
		if (o_cli->host)
			o->host = strdup (o_cli->host);
		else if (o_cfg->host)
			o->host = strdup (o_cfg->host);
		else
			o->host = strdup (o_def->host);

		/* port (cli || cfg || def) */
		if (o_cli->port)
			o->port = o_cli->port;
		else if (o_cfg->port)
			o->port = o_cfg->port;
		else
			o->port = o_def->port;
	}
	else if (strcmp (type, "unix-socket") == 0)
	{
		/* unix_socket (cli || cfg || def) */
		if (o_cli->unix_socket)
			o->unix_socket = strdup (o_cli->unix_socket);
		else if (o_cfg->unix_socket)
			o->unix_socket = strdup (o_cfg->unix_socket);
		else
			o->unix_socket = strdup (o_def->unix_socket);
	}

	return o;
}