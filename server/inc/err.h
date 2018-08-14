#pragma once

#include <stdarg.h>

/**
 * Коды ошибок
 */
enum err_code
{
	SUCCESS,
	CLI_UNKNOWN_OPTION,
	CLI_NOT_COMMAND,
	CLI_MANY_COMMAND,
	CLI_CONFIG_NOT_EXIST,
	CLI_PORT_INCORRECT,
	CLI_COMMAND_UNKNOWN,
	CLI_HOST_UNIX_SOCKET
};

/**
 * Сообщение об ошибке
 */
struct err
{
	enum err_code code;
	char message[2048];
};

/**
 * Создать сообщение об ошибке
 */
enum err_code err_set (enum err_code code, const char * param);

/**
 * Получить сведения по последней ошибке
 */
struct err * err_get ();