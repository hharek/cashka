#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/prctl.h>
#include <string.h>

#include "../inc/cashka.h"
#include "../inc/err.h"
#include "../opt/opt.h"
#include "../pm/pm.h"

extern char ** environ;

/**
 * Старт
 */
int pm_start (char * pid_file, bool foreground, char * process_title, int argc, char ** argv)
{
	/* Проверяем запушен ли процесс */
	if (pm_is_running (pid_file))
		return err_set (PM_IS_RUNNING, NULL);

	/* Форкнуть процесс */
	if (!foreground)
		pm_fork ();

	/* Создать PID-файл */
	if (pm_pid_file_create (pid_file))
		return err_get ()->code;

	/* Сменить название процесса */
	pm_set_process_title (process_title, argc, argv);

	/* Зацикливаемся */
	while (true)
	{
		sleep (1);
	}

	return 0;
}

/**
 * Стоп
 */
int pm_stop (char * pid_file)
{
	/* Проверяем запущен ли процесс */
	int pid = pm_is_running(pid_file);
	if (!pid)
		return err_set (PM_NOT_RUNNING, NULL);

	/* Отправляем команду kill нашему процессу */
	if (kill(pid, SIGTERM) != 0)
		return err_set (PM_DO_NOT_STOP, NULL);

	return 0;
}

/**
 * Перезагрузка
 */
int pm_restart (char * pid_file, bool foreground, char * process_title, int argc, char ** argv)
{
	int result = 0;

	result = pm_stop (pid_file);
	if (result)
		return result;

	usleep (200000);

	result = pm_start (pid_file, foreground, process_title, argc, argv);
	if (result)
		return result;

	return result;
}

/**
 * Статус
 */
int pm_status (char * pid_file)
{
	if (pm_is_running(pid_file))
		printf ("Работает.\n");
	else
		printf ("Не работает.\n");

	return 0;
}

/**
 * Проверить запущен ли процесс
 */
int pm_is_running (char * pid_file)
{
	/* Читаем содержимое pid-файла */
	FILE * fp = fopen (pid_file, "r");
	if (!fp)
		return false;

	char buf[10] = {'\0'};
	fread (buf, 9, 1, fp);

	/* Переводим в число */
	char * end;
	pid_t pid = (unsigned int)strtol (buf, &end, 0);
	if (*end && *end != '\n')
		return false;

	if (!pid)
		return false;

	/* Делаем запрос kill */
	if (kill(pid, 0) == 0)
		return pid;

	return false;
}

/**
 * Форкнут процесс
 */
void pm_fork ()
{
	/* Форкаемся */
	pid_t pid;
	pid = fork();

	/* Родительский процесс закрываем */
	if (pid != 0)
		exit (EXIT_SUCCESS);

	/* Я свободен :) */
	return;
}

/**
 * Создать PID файл
 */
int pm_pid_file_create (char * pid_file)
{
	/* Получаем PID */
	pid_t pid = getpid ();

	/* Создаём PID-файл */
	FILE * fp = fopen (pid_file, "w");
	if (!fp)
		return err_set (PM_PID_FILE_NOT_CREATE, NULL);

	/* Переводим int в char[] */
	char pid_str[12] = {'\0'};
	sprintf (pid_str, "%d", pid);

	/* Помешаем PID в файл */
	if (!fwrite (pid_str, 12, 1, fp))
		return err_set (PM_PID_FILE_NOT_CREATE, NULL);

	fclose (fp);

	return 0;
}

/**
 * Задать имя процессу
 */
void pm_set_process_title (char * title, int argc, char ** argv)
{
	/* Копируем argv и чистим */
	char ** argv_old = argv;							/* Сохраняем ссылку на старый «argv» */
	argv = malloc (sizeof (char *) * (argc + 1));		/* «argv» указывает на новый массив */
	for (unsigned int i = 0; i < argc; i++)
	{
		argv[i] = malloc (sizeof(char) * (strlen(argv_old[i]) + 1));
		strcpy (argv[i], argv_old[i]);

		/* Чистим старый массив */
		memset (argv_old[i], '\0', strlen(argv_old[i]));
	}
	argv[argc] = NULL;									/* Последний элемент массива указывает на NULL */

	/* Вычисляем размер environ */
	int environ_size = 0;
	for (int i = 0; environ[i] != NULL; i++)
	{
		environ_size++;
	}

	/* Копируем в environ новый массив и чистим */
	char ** environ_old = environ;									/* Сохраняем ссылку на старый «environ» */
	environ = malloc (sizeof (char *) * (environ_size + 1));		/* «environ» указывает на новый массив */
	for (unsigned int i = 0; i < environ_size; i++)
	{
		environ[i] = malloc (sizeof(char) * (strlen(environ_old[i]) + 1));
		strcpy (environ[i], environ_old[i]);

		/* Чистим старый массив */
		memset (environ_old[i], '\0', strlen(environ_old[i]));
	}
	environ[environ_size] = NULL;									/* Последний элемент массива указывает на NULL */

	/* Назначаем имя процессу */
	strcpy (argv_old[0], title);
	prctl (PR_SET_NAME, title, 0, 0, 0);							/* Назначаем имя потоку */
}