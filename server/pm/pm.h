#include <stdbool.h>

/**
 * Старт
 */
int start (struct opt * o);

/**
 * Стоп
 */
int stop (struct opt * o);

/**
 * Перезагрузка
 */
int restart (struct opt * o);

/**
 * Статус
 */
int status (struct opt * o);

/**
 * Проверить запушен ли процесс
 */
bool is_running (const char * pid_file);