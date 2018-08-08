#include <stdio.h>
#include <string.h>

/**
 * Назначить сообщение об ошибке
 */
enum confi_err_code err (enum confi_err_code code, const char * format, const char * param)
{
	err_mess.code = code;

 	if (param != NULL)
	{
		strncpy (err_mess.param, param, 1024);
		sprintf (err_mess.message, format, param);
	}
	else
	{
		strncpy (err_mess.message, format, 2048);
	}

	return code;
}