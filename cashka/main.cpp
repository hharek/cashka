#include <string>
#include <iostream>

#include "cashka.h"
#include "options.h"
#include "pm.h"

using std::string;

using cashka::Options;
using cashka::PM;

int main (int argc, char ** argv) 
{
	try
	{
		/* Создаём опции */
		Options opt;
		opt.cli (argc, argv);
		opt.config ();
		opt.merge ();
		
		/* Менеджер процессов */
		PM pm (opt);
		
		string command = opt.get_command();
		if (command == "start")
		{
			pm.start();
		}
		else if (command == "stop")
		{
			pm.stop();
		}
		else if (command == "restart")
		{
			pm.restart();
		}
		else if (command == "status")
		{
			pm.status();
		}
		
		return EXIT_SUCCESS;
	}
	catch (std::exception & ex)
	{
		std::cerr << "Ошибка: " << ex.what() << std::endl;
		return EXIT_FAILURE;
	}
}
