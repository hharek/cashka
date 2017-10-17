#include <iostream>
#include <string>

#include "cashka.h"
#include "options.h"

using std::string;

namespace cashka
{

	/**
	 * Проверить комманду
	 * 
	 * @param const char * command
	 * @return void
	 */
	void Options::check_command (const char * command)
	{
		const char * command_allow[] =
		{
			"start",
			"stop",
			"restart",
			"status"
		};

		bool check = false;

		for (int i = 0; i < 4; i++)
		{
			if (std::strcmp (command, command_allow[i]) == 0)
			{
				check = true;
			}
		}

		if (!check)
		{
			err ((string)"Неизвестная команда: «" + (string)command + (string)"». Используйте: «start», «stop», «restart», «status».", "cli");
		}
	}

	/**
	 * Проверить опцию «process_name»
	 * 
	 * @param const char * process_name
	 * @return void
	 */
	void Options::check_process_name  (const char * process_name)
	{
		if (std::strlen(process_name) > 32)
		{
			err ("Опция «process_name» задана неверно. Не должен превышать 32 символа (байтов).", "config");
		}

		for (const char * c = process_name; *c; c++)
		{
			if 
			(
				!std::isalpha(*c) && 
				!std::isalnum(*c) && 
				*c != '_' && 
				*c != '-'
			)
			{
				err ("Опция «process_name» задана неверно. Допускаются символы a-z, 0-9, «_», «-».", "config");
			}
		}
	}

	/**
	 * Проверить опцию «config_file»
	 * 
	 * @param const char * config_file
	 * @return void
	 */
	void Options::check_config_file (const char * config_file)
	{
		char * _realpath = realpath(config_file, NULL);
		if (_realpath == nullptr)
		{
			err ("Конфигурационный файл указан неверно.", "cli");
		}
	}

	/**
	 * Проверить опцию «port»
	 * 
	 * @param const char * port
	 * @param const char * type
	 * @return void
	 */
	void Options::check_port (int port, const char * type)
	{
		if (port < 1)
		{
			err ("Порт задан неверно.", type);
		}

		if (port < 1024 || port > 65535)
		{
			err ("Порт задан неверно. Необходимо указать порт в диапазоне от 1024 до 65535.", type);
		}
	}

}