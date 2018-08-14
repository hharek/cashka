#include <stdio.h>
#include <string.h>

#include "err.h"

/**
 * Массив с ошибками
 */
const char * err_mess[] =
{
	[SUCCESS] = "Ошибок нет. Ура.",
	[CLI_UNKNOWN_OPTION] = "Неизвестная опция. Используйте --help для получения справки.",
	[CLI_NOT_COMMAND] = "Не указана команда. Используйте: «start», «stop», «restart», «status» или «--help» для получения справки.",
	[CLI_MANY_COMMAND] = "Указано несколько команд. Используйте --help для получения справки.",
	[CLI_CONFIG_NOT_EXIST] = "Конфигурационный файл указан неверно.",
	[CLI_PORT_INCORRECT] = "Порт задан неверно.",
	[CLI_COMMAND_UNKNOWN] = "Неизвестная команда.",
	[CLI_HOST_UNIX_SOCKET] = "Нельзя указать одновременно «--host» или «--port» и «--unix-socket».",
	[CFG_PARSING_ERROR] = "Ошибка в конфигурационном файле: %s"
};

/**
 * Инициализируем глобальную переменную с сообщением о последней ошибке
 */
struct err err;

/**
 * Назначить сообщение об ошибке
 */
enum err_code err_set (enum err_code code, const char * param)
{
	err.code = code;

	if (param != NULL)
		sprintf (err.message, err_mess[code], param);
	else
		strncpy (err.message, err_mess[code], 2048);

	return code;
}

/**
 * Получить сведения о последней ошибке
 */
struct err * err_get ()
{
	return &err;
}