#include <stddef.h>
#include <stdio.h>

#include "../inc/err.h"

/**
 * Получить опции
 */
int opt (int argc, char ** argv, struct opt * o)
{
	printf ("Опции.\n");

	return err_set (CASHKA_ERR_1, "Сообщение об ошибке", NULL);
}


