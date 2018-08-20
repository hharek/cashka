#include <stdio.h>

#include "inc/cashka.h"
#include "inc/err.h"
#include "opt/opt.h"

int main (int argc, char ** argv)
{
	struct opt * o = opt (argc, argv);
	if (o == NULL)
	{
		fprintf (stderr, err_get ()->message);
		return 1;
	}

//	struct opt
//	{
//		char * process_title;				/* Имя процесса */
//		char * config_file;					/* Путь к конфигурационному файлу */
//		bool foreground;					/* Запускать процесс на переднем плане (не в фоне) */
//		char * pid_file;					/* Путь к PID-файлу */
//		char * host;						/* Имя хоста */
//		unsigned int port;					/* Номер порта */
//		char * unix_socket;					/* Путь к unix-сокету */
//		char * command;						/* Команда (start, stop, restart, status) */
//	};

	printf ("process_title = %s\n", o->process_title);
	printf ("config_file = %s\n", o->config_file);
	printf ("foreground = %i\n", o->foreground);
	printf ("pid_file = %s\n", o->pid_file);
	printf ("host = %s\n", o->host);
	printf ("port = %i\n", o->port);
	printf ("unix_socket = %s\n", o->unix_socket);
	printf ("command = %s\n", o->command);


    return 0;
}