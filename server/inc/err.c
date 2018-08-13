#include <stdio.h>
#include <string.h>
#include "err.h"

/**
 * Инициализируем глобальную переменную с сообщением о последней ошибке
 */
struct err err;

/**
 * Назначить сообщение об ошибке
 */
enum err_code err_set (enum err_code code, const char * format, const char * param)
{
	err.code = code;

	if (param != NULL)
	{
		sprintf (err.message, format, param);
	}
	else
	{
		strncpy (err.message, format, 2048);
	}

	return code;
}

/**
 * Получить сведения о последней ошибке
 */
struct err * err_get ()
{
	return &err;
}