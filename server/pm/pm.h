#include <stdbool.h>

/**
 * Старт
 */
int pm_start (char * pid_file, bool foreground, char * process_title, int argc, char ** argv);

/**
 * Стоп
 */
int pm_stop (char * pid_file);

/**
 * Перезагрузка
 */
int pm_restart (char * pid_file, bool foreground, char * process_title, int argc, char ** argv);

/**
 * Статус
 */
int pm_status (char * pid_file);

/* Статические методы */
static int pm_is_running (char * pid_file);									/* Проверить запущен ли процесс */
static void pm_fork ();														/* Форкнуть процесс */
static int pm_pid_file_create (char * pid_file);							/* Создать PID файл */
static void pm_set_process_title (char * title, int argc, char ** argv);	/* Задать имя процессу */