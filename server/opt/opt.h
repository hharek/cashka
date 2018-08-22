#pragma once

#include <stdbool.h>

/**
 * Структура для опций
 */
struct opt
{
	char * process_title;				/* Имя процесса */
	char * command;						/* Команда (start, stop, restart, status) */
	char * config_file;					/* Путь к конфигурационному файлу */
	bool foreground;					/* Запускать процесс на переднем плане (не в фоне) */
	char * pid_file;					/* Путь к PID-файлу */
	char * host;						/* Имя хоста */
	unsigned int port;					/* Номер порта */
	char * unix_socket;					/* Путь к unix-сокету */
};

/**
 * Получить опции
 */
struct opt * opt ();

/**
 * Получить опции по командной строке
 */
struct opt * opt_cli ();

void opt_cli_help ();
void opt_cli_version ();

/**
 * Получить опции по конфигурационному файлу
 */
struct opt * opt_cfg (char * file);

/**
 * Опции по умолчанию
 */
struct opt * opt_def ();

/**
 * Проверяем параметры
 */
int opt_check (struct opt * o, const char * type);

/**
 * Инициализируем структуру. Дополнительно обнуляем
 */
struct opt * opt_init ();

/**
 * Очистить структуру
 */
void opt_free (struct opt * o);

/**
 * Объединить опции (cli || cfg || def)
 */
struct opt * opt_join (struct opt * o_cli, struct opt * o_cfg, struct opt * o_def);



