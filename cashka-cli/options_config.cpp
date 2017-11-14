#include <string>
#include <iostream>
#include <fstream>
#include <sys/socket.h>

#include <json.hpp>

#include "cashka-cli.h"
#include "options.h"

using json = nlohmann::json;
using std::string;

namespace cashka_cli
{

	/**
	 * Парсим конфигурационный файл
	 * 
	 * @return void
	 */
	void Options::config ()
	{
		string config_file;

		if (this->config_file_default_check())
		{
			config_file = this->config_file;
		}

		if (!this->cli_config_file.empty())
		{
			config_file = this->cli_config_file;
		}

		/* Задан ли конфигурационный файл в консоле */
		if (config_file.empty())
		{
			return;
		}

		/* Парсим конфиг */
		std::ifstream f (config_file);

		json config;
		try
		{
			config = json::parse (f);
		}
		catch (std::exception & ex)
		{
			err ("Конфигурационный файл не является JSON-файлом.", "config");
		}

		/* Проверяем */
		this->config_check_options (config);

		/* Назначаем */
		if (!config["host"].is_null())			{ this->config_host_set (config["host"]);						}
		if (!config["port"].is_null())			{ this->config_port_set (config["port"]);						}
		if (!config["unix-socket"].is_null())	{ this->config_unix_socket_set (config["unix-socket"]);			}
	}

	/**
	 * Проверить конфигурационный файл на присутствие недопустимых опций и на соответствие типу
	 * 
	 * @return void
	 */
	void Options::config_check_options (json & config)
	{
		for (json::iterator config_option = config.begin(); config_option != config.end(); config_option++)
		{
			string key = config_option.key();

			/* Определяем допустимая ли опция */
			auto allow_it = this->config_options_allow.find(key);
			if (allow_it == this->config_options_allow.end())
			{
				err ((string)"Недопустимая опция «" + key + (string)"».", "config");
			}

			/* Проверяем на соответствие типу */
			string type = allow_it->second;
			if (type == (string)"string" && !config[key].is_string())
			{
				err ((string)"Опция «" + key + (string)"» задана неверно. Должна быть строкой.", "config");

				if (config[key].get<string>().empty())
				{
					err ((string)"Опция «" + key + (string)"» задана неверно. Пустая строка.", "config");
				}
			}
			else if (type == (string)"bool" && !config[key].is_boolean())
			{
				err ((string)"Конфиг. Опция «" + key + (string)"» задана неверно. Должна быть «true» или «false».", "config");
			}
			else if (type == (string)"uint" && !config[key].is_number_unsigned())
			{
				err ((string)"Конфиг. Опция «" + key + (string)"» задана неверно. Должна быть числом.", "config");
			}
		}
	}

	/**
	 * Проверить и назначить имя хоста
	 * 
	 * @param nlohmann::json & host
	 * @return void
	 */
	void Options::config_host_set (nlohmann::json & host)
	{
		this->config_host = host.get<string>();
	}

	/**
	 * Проверить и назначить порт
	 * 
	 * @param nlohmann::json & port
	 * @return void
	 */
	void Options::config_port_set (nlohmann::json & port)
	{
		int port_int = port.get<int>();

		this->check_port(port_int, "config");

		this->config_port = (unsigned int)port_int;
	}

	/**
	 * Проверить и назначить путь к PID-файлу
	 * 
	 * @param nlohmann::json & unix_socket
	 * @return void
	 */
	void Options::config_unix_socket_set (nlohmann::json & unix_socket)
	{
		this->config_unix_socket = unix_socket.get<string>();
	}

	/**
	 * Проверить файл прописанный по умолчанию существует
	 * 
	 * @return bool
	 */
	bool Options::config_file_default_check ()
	{
		char * config_file_realpath = realpath(this->config_file.c_str(), NULL);

		if (config_file_realpath != nullptr)
		{
			this->config_file = config_file_realpath;
			return true;
		}
		else
		{
			this->config_file = "";
			return false;
		}
	}

}