#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>

#include "../inc/cashka.h"
#include "../inc/err.h"
#include "../opt/opt.h"
#include "../pm/pm.h"

/**
 * Старт
 */
int start (struct opt * o)
{
	if (is_running (o->pid_file))
		printf ("Уже запушен\n");
	else
		printf ("Не запушен\n");

//	printf ("Команда: start.\n");
	return 0;
}

/**
 * Стоп
 */
int stop (struct opt * o)
{
	printf ("Команда: stop.\n");
	return 0;
}

/**
 * Перезагрузка
 */
int restart (struct opt * o)
{
	printf ("Команда: restart.\n");
	return 0;
}

/**
 * Статус
 */
int status (struct opt * o)
{
	printf ("Команда: status.\n");
	return 0;
}


/**
 * Проверить запущен ли процесс
 */
bool is_running (const char * pid_file)
{
	/* Читаем содержимое pid-файла */
	FILE * fp = fopen (pid_file, "r");
	if (fp)
	{
		char buf[10] = {'\0'};
		fread (buf, 9, 1, fp);

		/* Переводим в число */
		char * end;
		pid_t pid = (unsigned int)strtol (buf, &end, 0);
		if (*end && *end != '\n')
		{
			fclose (fp);
			return false;
		}

		/* Делаем запрос kill */
		if (kill(pid, 0) == 0)
		{
			return true;
		}
	}
	return false;
}