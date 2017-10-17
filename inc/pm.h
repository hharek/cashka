#pragma once

#include <sys/types.h>
#include <signal.h>

#include "options.h"

namespace cashka
{

	/**
	 * Управление процессами
	 */
	class PM
	{
		private:

			/* Доступ к опциям */
			Options & options;
			
			/* PID сервака */
			pid_t pid_server;

			/* Копия переменной environ */
			char ** _environ;
			int _environ_size;
			
			/* Копия переменной argv */
			char ** _argv;
			int _argc;
			
		public:

			/* Конструктор */
			PM (Options & options);

			/* Запуск */
			void start ();

			/* Остановка */
			void stop ();

			/* Перезапуск */
			void restart ();

			/* Показать статус процесса */
			void status ();

		private:

			/* Проверить запущен ли процесс */
			bool is_running ();

			/* Создать PID-файл */
			void pid_file_create ();

			/* Форкнуть процесс */
			void forking ();
			
			/* Сменить название процесса */
			void set_process_title (const char * title);
			
			/* Скопировать переменную environ */
			void environ_copy ();
			
			/* Скопировать переменную argv */
			void argv_copy ();
	};

}