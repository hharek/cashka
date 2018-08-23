#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "inc/cashka.h"
#include "inc/err.h"
#include "opt/opt.h"
#include "pm/pm.h"

/**
 * main ()
 */
int main (int argc, char ** argv)
{
	/* Получаем опции */
	struct opt * o = opt (argc, argv);
	if (o == NULL)
	{
		fprintf (stderr, "%s\n", err_get ()->message);
		return EXIT_FAILURE;
	}

	/* Выполняем команду */
	int result = 0;
	if (strcmp (o->command, "start") == 0)
		result = pm_start (o->pid_file, o->foreground, o->process_title, argc, argv);
	else if (strcmp (o->command, "stop") == 0)
		result = pm_stop (o->pid_file);
	else if (strcmp (o->command, "restart") == 0)
		result = pm_restart (o->pid_file, o->foreground, o->process_title, argc, argv);
	else if (strcmp (o->command, "status") == 0)
		result = pm_status (o->pid_file);

	/* Ошибка */
	if (result != 0)
	{
		fprintf (stderr, "%s\n", err_get ()->message);
		return EXIT_FAILURE;
	}

    return EXIT_SUCCESS;
}