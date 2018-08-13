#include <stdarg.h>

/**
 * Коды ошибок
 */
enum err_code
{
	CASHKA_SUCCESS,
	CASHKA_ERR_1
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
enum err_code err_set (enum err_code code, const char * format, const char * param);

/**
 * Получить сведения по последней ошибке
 */
struct err * err_get ();