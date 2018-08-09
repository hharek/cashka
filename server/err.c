#include <stdio.h>
#include <string.h>

#include "err.h"

/**
 * Назначить сообщение об ошибке
 */
enum err_code err_mess_set (enum err_code code, const char * format, const char * param)
{
	err_mess.code = code;

	if (param != NULL)
	{
		sprintf (err_mess.message, format, param);
	}
	else
	{
		strncpy (err_mess.message, format, 2048);
	}

	return code;
}

/**
 * Вернуть сообщение об ошибке
 */
struct err_mess err ()
{
	return err_mess;
}