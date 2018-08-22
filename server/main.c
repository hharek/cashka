#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "inc/cashka.h"
#include "inc/err.h"
#include "opt/opt.h"
#include "pm/pm.h"

/* Делаем глобальными «argc» и «argv» */
int argc;
char ** argv;

/* Глобальные переменные */
pid_t cashka_pid = 0;

int main (int ac, char ** av)
{
	/* Делаем глобальными «argc» и «argv» */
	argc = ac;
	argv = av;

	/* Получаем опции */
	struct opt * o = opt ();
	if (o == NULL)
	{
		fprintf (stderr, "%s\n", err_get ()->message);
		return EXIT_FAILURE;
	}

	/* Выполняем команду */
	int result = 0;
	if (strcmp (o->command, "start") == 0)
		result = pm_start (o);
	else if (strcmp (o->command, "stop") == 0)
		result = pm_stop (o);
	else if (strcmp (o->command, "restart") == 0)
		result = pm_restart (o);
	else if (strcmp (o->command, "status") == 0)
		result = pm_status (o);

	/* Ошибка */
	if (result != 0)
	{
		fprintf (stderr, "%s\n", err_get ()->message);
		return EXIT_FAILURE;
	}

    return EXIT_SUCCESS;
}