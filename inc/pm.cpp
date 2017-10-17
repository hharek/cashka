#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/prctl.h>

#include "cashka.h"
#include "options.h"
#include "pm.h"

extern char ** environ;

using std::cout;
using std::endl;

namespace cashka
{
	/**
	 * Конструктор
	 */
	PM::PM (Options & options) : options (options) {}

	/**
	 * Запуск 
	 * 
	 * @return void
	 */
	void PM::start ()
	{
		/* Проверяем запущен ли процесс */
		if (this->is_running())
		{
			err ("Сервер уже запущен.");
		}

		/* Форк */
		if (!this->options.get_foreground())
		{
			this->forking();
		}

		/* Создать PID-файл */
		this->pid_file_create();

		/* Сменить название процесса */
		this->set_process_title (options.get_process_name().c_str());
		
		/* Работа в фоне */
		while (true)
		{
			sleep(10);
		}
	}

	/**
	 * Остановить
	 * 
	 * @return void
	 */
	void PM::stop ()
	{
		/* Проверяем запущен ли процесс */
		if (!this->is_running())
		{
			err ("Сервер не запущен.");
		}
		
		if (kill(this->pid_server, SIGTERM) != 0)
		{
			err ("Не удалось установить сервер.");
		}
	}

	/**
	 * Перезапустить
	 * 
	 * @return void
	 */
	void PM::restart ()
	{
		this->stop();
		
		usleep(200000);
		
		this->start();
	}

	/**
	 * Показать статус
	 * 
	 * @return void
	 */
	void PM::status ()
	{
		if (this->is_running())
		{
			cout << "Сервер запущен." << endl;
		}
		else
		{
			cout << "Сервер не запущен." << endl;
		}
	}

	/**
	 * Проверить запущен ли процесс
	 * 
	 * @return bool
	 */
	bool PM::is_running ()
	{
		std::ifstream fp;
		fp.open (this->options.get_pid_file());

		if (fp.is_open())
		{
			pid_t pid;
			fp >> pid;

			if (kill(pid, 0) == 0)
			{
				this->pid_server = pid;
				return true;
			}
		}

		fp.close();

		return false;
	}

	/**
	 * Создать PID-файл
	 */
	void PM::pid_file_create ()
	{
		std::ofstream fp (this->options.get_pid_file());

		if (!fp.is_open())
		{
			err ((string)"Не удалось создать PID-файл. PID-файл: " + this->options.get_pid_file());
		}

		fp << getpid();

		fp.close();
	}

	/**
	 * Форкнуть процесс
	 */
	void PM::forking ()
	{
		/* Форкаемся */
		pid_t pid;
		pid = fork();

		/* Родительский процесс закрываем */
		if (pid != 0)
		{
			exit (EXIT_SUCCESS);
		}

		/* Я свободен :) */
		return;
	}
	
	/**
	 * Сменить название процесса
	 * 
	 * @param const char * title
	 */
	void PM::set_process_title (const char * title)
	{
		/* Копируем переменую environ */
		this->environ_copy();
		
		/* Оригинальный argv */
		char ** argv = options.get_argv();
		
		/* Определяем последнюю позицию environ */
		char * end;
		for (int i = 0; environ[i] != 0; i++)
		{
			end = environ[i] + strlen(environ[i]);
		}

		/* Затираем нулями argv и environ */
		memset (argv[0], 0, (char *) end - (char *) argv[0]);

		/* Прописываем имя своего процесса */
		strcpy (argv[0], title);

		/* Прописываем имя потока */
		prctl (PR_SET_NAME, title, 0, 0, 0);
		
		/* Переменная environ указывает на новый участок со старыми значениями */
		environ = this->_environ;
	}
	
	/**
	 * Копируем переменую environ
	 */
	void PM::environ_copy ()
	{
		int environ_size = 0;
		for (int i = 0; environ[i] != nullptr; i++)
		{
			environ_size++;
		}

		char ** environ_copy = new char * [environ_size];

		for (int i = 0; i < environ_size; i++)
		{
			environ_copy[i] = new char[strlen(environ[i]) + 1];
			strcpy (environ_copy[i], environ[i]);
		}
		
		this->_environ_size = environ_size;
		this->_environ = environ_copy;
	}
}