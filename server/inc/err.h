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
	CLI_HOST_UNIX_SOCKET,
	CFG_PARSING_ERROR,
	CFG_PROCESS_TITLE_BIG,
	CFG_PROCESS_TITLE_BAD_CHARSET,
	CFG_HOST_UNIX_SOCKET,
	CFG_PORT_INCORRECT,

	PM_IS_RUNNING,
	PM_PID_FILE_NOT_CREATE,
	PM_NOT_RUNNING,
	PM_DO_NOT_STOP
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