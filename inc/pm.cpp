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
#include "server.h"

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
		
		/* Запускаем сервак */
		this->server = new Server (this->options);
		this->server->start ();
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
		/* Переносим argv и environ в другое место и зачищаем занятую им область */
		this->argv_move();
		this->environ_move();
		
		/* Прописываем имя своего процесса в старую область argv */
		char ** argv = options.get_argv();
		strcpy (argv[0], title);

		/* Прописываем имя потока */
		prctl (PR_SET_NAME, title, 0, 0, 0);
	}
	
	/**
	 * Перенести «argv» в новую переменную и очистить старую область «argv»
	 */
	void PM::argv_move ()
	{
		int argc = options.get_argc();
		char ** argv = options.get_argv();

		/* Переносим */
		char ** copy = new char * [argc + 1];
		for (int i = 0; i < argc; i++)
		{
			/* Копируем в новый массив */
			copy[i] = new char[strlen(argv[i]) + 1];
			strcpy (copy[i], argv[i]);

			/* Удаляем в старом */
			memset (argv[i], 0, strlen(argv[i]));
		}

		/* Последний элемент должен указывать на 0x00 */
		copy[argc] = nullptr;

		/* Назначаем переменным класса */
		this->_argv = copy;
		this->_argc = argc;
	}
	
	/**
	 * Перенести «environ» в новую переменную и очистить старую область «environ». Необходимо для set_process_title
	 */
	void PM::environ_move ()
	{
		char ** copy;
		int size = 0;

		/* Вычисляем размер environ */
		for (int i = 0; environ[i] != nullptr; i++)
		{
			size++;
		}

		/* Переносим */
		copy = new char * [size + 1];
		for (int i = 0; i < size; i++)
		{
			/* Копируем в новый массив */
			copy[i] = new char[strlen(environ[i]) + 1];
			strcpy (copy[i], environ[i]);

			/* Удаляем в старом */
			memset (environ[i], 0, strlen(environ[i]));
		}

		/* Последний элемент должен указывать на 0x00 */
		copy[size] = nullptr;

		/* Назначаем переменным класса */
		this->_environ = copy;
		
		/* environ теперь укзывает на новый массив, но с такими же данными */
		environ = this->_environ;
	}
}