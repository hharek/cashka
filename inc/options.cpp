#include <iostream>
#include <string>

#include "options.h"

using std::string;

namespace cashka
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
		if (!this->config_process_name.empty()) { this->process_name = this->config_process_name;	}

		if (!this->cli_config_file.empty())		{ this->config_file = this->cli_config_file;		}

		if (!this->config_pid_file.empty())		{ this->pid_file = this->config_pid_file;			}
		if (!this->cli_pid_file.empty())		{ this->pid_file = this->cli_config_file;			}

		if (this->config_foreground)			{ this->foreground = true;							}
		if (this->cli_foreground_set)			{ this->foreground = true;							}

		if (!this->config_host.empty())			{ this->host = this->config_host;					}
		if (!this->cli_host.empty())			{ this->host = this->cli_host;						}

		if (this->config_port != 0)				{ this->port = this->config_port;					}
		if (this->cli_port != 0)				{ this->port = this->cli_port;						}
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
		if (this->foreground)
		{
			foreground = "true";
		}
		
		dump += "process_name: "	+ this->process_name	+ "\n";
		dump += "version: "			+ this->version			+ "\n";
		dump += "command: "			+ this->command			+ "\n";
		dump += "config_file: "		+ this->config_file		+ "\n";
		dump += "foreground: "		+ foreground			+ "\n";
		dump += "pid_file: "		+ this->pid_file		+ "\n";
		dump += "host: "			+ this->host			+ "\n";
		dump += "port: "			+ std::to_string (this->port);

		return dump;
	}

	/* Получить опции */
	const string & Options::get_process_name ()	{ return this->process_name;	}
	const string & Options::get_version ()		{ return this->version;			}
	const string & Options::get_command ()		{ return this->command;			}
	const string & Options::get_config_file ()	{ return this->config_file;		}
	const string & Options::get_pid_file ()		{ return this->pid_file;		}
	const string & Options::get_host ()			{ return this->host;			}
	const      int Options::get_port ()			{ return this->port;			}
	const     bool Options::get_foreground ()	{ return this->foreground;		}
	int Options::get_argc ()					{ return this->argc;			}
	char ** Options::get_argv ()				{ return this->argv;			}
}