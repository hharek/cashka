#include <string>
#include <iostream>
#include <fstream>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <libgen.h>

#include <json.hpp>

#include "cashka.h"
#include "options.h"

using json = nlohmann::json;
using std::string;

namespace cashka
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
		if (!config["process_name"].is_null())	{ this->config_process_name_set (config["process_name"]);		}
		if (!config["pid_file"].is_null())		{ this->config_pid_file_set (config["pid_file"]);				}
		if (!config["foreground"].is_null())	{ this->config_foreground = config["foreground"].get<bool>();	}
		if (!config["host"].is_null())			{ this->config_host_set (config["host"]);						}
		if (!config["port"].is_null())			{ this->config_port_set (config["port"]);						}
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
	 * Проверить и назначить имя процесса
	 * 
	 * @param nlohmann::json & process_name
	 * @return void
	 */
	void Options::config_process_name_set (json & process_name)
	{
		string process_name_s = process_name.get<string>();

		this->check_process_name(process_name_s.c_str());

		this->config_process_name = process_name_s;
	}

	/**
	 * Проверить и назначить путь к PID-файлу
	 * 
	 * @param nlohmann::json & pid_file
	 * @return void
	 */
	void Options::config_pid_file_set (nlohmann::json & pid_file)
	{
		this->config_pid_file = pid_file.get<string>();
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