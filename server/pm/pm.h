#include <stdbool.h>

/**
 * Старт
 */
int pm_start (struct opt * o);

/**
 * Стоп
 */
int pm_stop (struct opt * o);

/**
 * Перезагрузка
 */
int pm_restart (struct opt * o);

/**
 * Статус
 */
int pm_status (struct opt * o);

/* Статические методы */
static bool pm_is_running (const char * pid_file);		/* Проверить запущен ли процесс */
static void pm_fork ();									/* Форкнуть процесс */
static int  pm_pid_file_create (const char * pid_file);	/* Создать PID файл */
static void pm_set_process_title (const char * title);	/* Задать имя процессу */