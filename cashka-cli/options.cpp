#include <iostream>

#include "options.h"

using std::string;

namespace cashka_cli
{
	/**
	 * Конструктор по умолчанию. Ничего не делает
	 */
	Options::Options () {}

	/**
	 * Объединяем конфигурации
	 * 
	 * @return void
	 */
	void Options::merge ()
	{
		if (!this->cli_config_file.empty())			{ this->config_file = this->cli_config_file;		}

		if (!this->config_host.empty())				{ this->host = this->config_host;					}
		if (!this->cli_host.empty())				{ this->host = this->cli_host;						}

		if (this->config_port != 0)					{ this->port = this->config_port;					}
		if (this->cli_port != 0)					{ this->port = this->cli_port;						}

		if (!this->config_unix_socket.empty())		{ this->unix_socket = this->config_unix_socket;		}
		if (!this->cli_unix_socket.empty())			{ this->unix_socket = this->cli_unix_socket;		}

		this->check_unix_socket();
	}

	/**
	 * Показать опции
	 * 
	 * @return string
	 */
	string Options::dump ()
	{
		string dump;

		string foreground = "false";

		dump += "version: "			+ this->version					+ "\n";
		dump += "config_file: "		+ this->config_file				+ "\n";

		if (this->unix_socket.empty())
		{
			dump += "host: "		+ this->host					+ "\n";
			dump += "port: "		+ std::to_string (this->port);
		}
		else
		{
			dump += "unix-socket: "	+ this->unix_socket;
		}

		return dump;
	}

	/* Получить опции */
	const string & Options::get_version ()			{ return this->version;			}
	const string & Options::get_config_file ()		{ return this->config_file;		}
	const string & Options::get_host ()				{ return this->host;			}
	const      int Options::get_port ()				{ return this->port;			}
	const string & Options::get_unix_socket ()		{ return this->unix_socket;		}
}