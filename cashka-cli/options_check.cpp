#include <iostream>
#include <string>

#include "cashka-cli.h"
#include "options.h"

using std::string;

namespace cashka_cli
{
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

	/**
	 * Проверить параметр «unix-socket»
	 *
	 * @return void
	 */
	void Options::check_unix_socket ()
	{
		if (!this->unix_socket.empty())
		{
			if
			(
				(!this->config_host.empty() || !this->cli_host.empty()) ||
				(this->config_port != 0 || this->cli_port != 0)
			)
			{
				err ("Нельзя задать одновременно опцию «host» или «port» и «unix-socket».");
			}
		}
	}
}