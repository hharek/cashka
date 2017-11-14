#pragma once

#include <string>
#include <getopt.h>
#include <map>
#include <json.hpp>
#include <getopt.h>

using std::string;
using std::map;

namespace cashka_cli
{
	/**
	 * Опции программы
	 */
	class Options
	{
		private:

			/* Опции по умолчанию */
			string version = "0.64a";						/* Версия программы */
			string config_file = "cashka-cli.json";			/* Путь к конфигурационному файлу */
			string host = "localhost";						/* Имя хоста */
			unsigned int port = 3000;						/* Номер порта */
			string unix_socket;								/* Путь к unix-сокету */
			
			/* ### Командная строка ### */
			/* Опции по CLI */
			string cli_config_file;							/* Путь к конфигурационному файлу, заданный --config (-c) */
			string cli_host;								/* Имя хоста, заданое в --host (-h)  */
			unsigned int cli_port = 0;						/* Номер порта, заданный в --port (-p) */
			string cli_unix_socket;							/* Путь к unix-сокету, заданный в --unix-socket (-u) */
			
			/* Опции для функции getopt () */
			const char * cli_getopt = "hvc:H:P:u:";
			
			/* Опции для функции getopt_long () */
			const option cli_getopt_long[9] = 
			{
				{ "help",			no_argument,		NULL, 'h' },
				{ "version",		no_argument,		NULL, 'v' },
				{ "config",			required_argument,	NULL, 'c' },
				{ "host",			required_argument,	NULL, 'H' },
				{ "port",			required_argument,	NULL, 'P' },
				{ "unix-socket",	required_argument,	NULL, 'u' },
				{ 0, 0, 0, 0 }
			};
			
			/* ### Конфигурационный файл ### */
			/* Опции в конфиге */
			string config_host;								/* Имя хоста, заданный в конфигуарционном файле как «host» */
			unsigned int config_port = 0;					/* Номер порта, заданный в конфигурационном файла как «port» */
			string config_unix_socket;						/* Путь к unix-сокету, заданный в конфигурационном файле как «unix-socket» */

			/* Доступные опции и тип */
			map <string, string> config_options_allow = 
			{
				{ "host",			"string" },
				{ "port",			"uint"   },
				{ "unix-socket",	"string" }
			};

		public:

			/* Конструктор */
			Options ();
			
			/* Парсинг опций заданный в коммандной строке */
			void cli (int argc, char ** argv);

			/* Парсинга конфигурационного файла */
			void config ();

			/* Объединить конфигурацию */
			void merge ();

			/* Отобразить конфигурацию */
			string dump ();

			/* Получить параметры */
			const string & get_version ();
			const string & get_config_file ();
			const string & get_host ();
			const int get_port ();
			const string & get_unix_socket ();
			
		private:

			/* CLI функции */
			void cli_help ();										/* Отобразить справку в консоли */
			void cli_version ();									/* Отобразить версию программы в консоли  */
			void cli_config_file_set (const char * config_file);	/* Проверить и назначить конфигурационный файл, заданный через консоль */
			void cli_host_set (const char * host);					/* Назначить имя хосту, заданное через консоль */
			void cli_port_set (const char * port);					/* Проверить и назначить номер порта, заданный через консоль */
			void cli_unix_socket_set (const char * unix_socket);	/* Назначить unix-сокет, заданный через консоль */

			/* Функции по работе с конфигурационным файлом */
			void config_check_options (nlohmann::json & config);
			void config_host_set (nlohmann::json & host);
			void config_port_set (nlohmann::json & port);
			void config_unix_socket_set (nlohmann::json & socket);
			bool config_file_default_check ();

			/* Проверки опций */
			void check_config_file (const char * config_file);
			void check_port (int port, const char * type);
			void check_unix_socket ();
	};
}