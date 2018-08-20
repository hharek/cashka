#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inc/cashka.h"
#include "inc/err.h"
#include "opt/opt.h"
#include "pm/pm.h"

int main (int argc, char ** argv)
{
	/* Получаем опции */
	struct opt * o = opt (argc, argv);
	if (o == NULL)
	{
		fprintf (stderr, err_get ()->message);
		return EXIT_FAILURE;
	}

	/* Выполняем команду */
	int result = 0;
	if (strcmp (o->command, "start") == 0)
		result = start (o);
	else if (strcmp (o->command, "stop") == 0)
		result = stop (o);
	else if (strcmp (o->command, "restart") == 0)
		result = restart (o);
	else if (strcmp (o->command, "status") == 0)
		result = status (o);

	/* Ошибка */
	if (result != 0)
	{
		fprintf (stderr, err_get ()->message);
		return EXIT_FAILURE;
	}

    return EXIT_SUCCESS;
}