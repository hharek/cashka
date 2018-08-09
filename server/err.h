#include <stdarg.h>

enum err_code
{
	CASHKA_SUCCESS
};

/**
 * Инициализируем глобальную переменную с сообщением о последней ошибке
 */
struct err_mess
{
	int code;
	char message[2048];
}
err_mess =
{
	.code = 0,
	.message = {'\0'}
};

/**
 * Создать сообщение об ошибке
 */
enum err_code err_mess_set (enum err_code code, const char * format, const char * param);

/**
 * Вернуть последнее сообщение об ошибке
 */
struct err_mess err ();