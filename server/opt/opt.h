#include <stdbool.h>

/**
 * Структура для опций
 */
struct opt
{
	const char * server_name;			/* Имя сервера */
	const char * version;				/* Версия программы */
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
 * Вывод параметров
 */
char * opt_dump (struct opt * cli, struct opt * cfg, struct opt * def);

/**
 * Получить опции
 */
int opt (int argc, char ** argv, struct opt * o);

/**
 * Получить опции по командной строке
 */
int opt_cli (int argc, char ** argv, struct opt * o);

int opt_cli_command (char * command, void * ptr);
int opt_cli_config_file (char * config_file, void * ptr);
int opt_cli_foreground (char * foreground, void * ptr);
int opt_cli_pid_file (char * pid_file, void * ptr);
int opt_cli_host (char * host, void * ptr);
int opt_cli_port (char * port, void * ptr);
int opt_cli_unix_socket (char * unix_socket, void * ptr);

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






