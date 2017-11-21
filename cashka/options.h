#pragma once

#include <string>
#include <getopt.h>
#include <map>
#include <json.hpp>
#include <getopt.h>

using std::string;
using std::map;

namespace cashka
{
	/**
	 * Опции программы
	 */
	class Options
	{
		private:

			/* Опции по умолчанию */
			string process_title = "cashka";				/* Имя процесса */
			string version = "0.67a";						/* Версия программы */
			string command;									/* Команда (start, stop, restart, status) */
			string config_file = "cashka.json";				/* Путь к конфигурационному файлу */
			bool foreground = false;						/* Запускать процесс на переднем плане (не в фоне) */
			string pid_file = "cashka.pid";					/* Путь к PID-файлу */
			string host = "localhost";						/* Имя хоста */
			unsigned int port = 3000;						/* Номер порта */
			string unix_socket;								/* Путь к unix-сокету */
			
			/* ### Командная строка ### */
			/* Опции по CLI */
			string cli_config_file;							/* Путь к конфигурационному файлу, заданный --config (-c) */
			bool cli_foreground_set = false;				/* Указан ли флаг запуска на переднем плане (не в фоне), заданный в --foreground (-f) */
			string cli_pid_file;							/* Путь к PID-файлу, заданный в --pid-file (-p) */
			string cli_host;								/* Имя хоста, заданое в --host (-h)  */
			unsigned int cli_port = 0;						/* Номер порта, заданный в --port (-p) */
			string cli_unix_socket;							/* Путь к unix-сокету, заданный в --unix-socket (-u) */
			
			/* Опции для функции getopt () */
			const char * cli_getopt = "hvp:c:fH:P:u:";
			
			/* Опции для функции getopt_long () */
			const option cli_getopt_long[9] = 
			{
				{ "help",			no_argument,		NULL, 'h' },
				{ "version",		no_argument,		NULL, 'v' },
				{ "pid-file",		required_argument,	NULL, 'p' },
				{ "config",			required_argument,	NULL, 'c' },
				{ "foreground",		no_argument,		NULL, 'f' },
				{ "host",			required_argument,	NULL, 'H' },
				{ "port",			required_argument,	NULL, 'P' },
				{ "unix-socket",	required_argument,	NULL, 'u' },
				{ 0, 0, 0, 0 }
			};
			
			/* Оригинальные argc и argv */
			int argc; 
			char ** argv;
			
			/* ### Конфигурационный файл ### */
			/* Опции в конфиге */
			string config_process_title;					/* Имя процесса, заданный в конфигурационному файле как «process-title» */
			string config_pid_file;							/* Путь к PID-файлу, заданный в конфигурационном файла как «pid-file» */
			string config_host;								/* Имя хоста, заданный в конфигуарционном файле как «host» */
			unsigned int config_port = 0;					/* Номер порта, заданный в конфигурационном файла как «port» */
			bool config_foreground = false;					/* Запускать ли на переднем плане (не в фоне), заданный в конфигурационном файле «foreground» */
			string config_unix_socket;						/* Путь к unix-сокету, заданный в конфигурационном файле как «unix-socket» */

			/* Доступные опции и тип */
			map <string, string> config_options_allow = 
			{
				{ "process-title",	"string" },
				{ "pid-file",		"string" },
				{ "foreground",		"bool"   },
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
			const string & get_process_title ();
			const string & get_version ();
			const string & get_command ();
			const string & get_config_file ();
			const string & get_pid_file ();
			const bool get_foreground ();
			const string & get_host ();
			const int get_port ();
			const string & get_unix_socket ();
			int get_argc ();
			char ** get_argv ();
			
		private:

			/* CLI функции */
			void cli_command_set (const char * command);			/* Проверить и назначить комманду */
			void cli_help ();										/* Отобразить справку в консоли */
			void cli_version ();									/* Отобразить версию программы в консоли  */
			void cli_config_file_set (const char * config_file);	/* Проверить и назначить конфигурационный файл, заданный через консоль */
			void cli_pid_file_set (const char * pid_file);			/* Назначить PID-файл, заданный через консоль */
			void cli_host_set (const char * host);					/* Назначить имя хосту, заданное через консоль */
			void cli_port_set (const char * port);					/* Проверить и назначить номер порта, заданный через консоль */
			void cli_unix_socket_set (const char * unix_socket);	/* Назначить unix-сокет, заданный через консоль */

			/* Функции по работе с конфигурационным файлом */
			void config_check_options (nlohmann::json & config);
			void config_process_title_set (nlohmann::json & process_title);
			void config_pid_file_set (nlohmann::json & pid_file);
			void config_foreground_set (nlohmann::json & foreground);
			void config_host_set (nlohmann::json & host);
			void config_port_set (nlohmann::json & port);
			void config_unix_socket_set (nlohmann::json & socket);
			bool config_file_default_check ();

			/* Проверки опций */
			void check_command (const char * command);
			void check_process_title (const char * process_title);
			void check_config_file (const char * config_file);
			void check_port (int port, const char * type);
			void check_unix_socket ();
	};
}