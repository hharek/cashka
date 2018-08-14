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
int opt (int argc, char ** argv, struct opt * o);

/**
 * Получить опции по командной строке
 */
int opt_cli (int argc, char ** argv, struct opt * o);
void opt_cli_help ();
void opt_cli_version ();

/**
 * Получить опции по конфигурационному файлу
 */
int opt_cfg (struct opt * o);

/**
 * Опции по умолчанию
 */
int opt_def (struct opt * o);






