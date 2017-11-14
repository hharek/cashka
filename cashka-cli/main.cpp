#include <string>
#include <iostream>

#include "cashka-cli.h"
#include "options.h"
#include "client.h"

using std::string;

using cashka_cli::Options;

int main (int argc, char ** argv)
{
	try
	{
		/* Создаём опции */
		Options opt;
		opt.cli (argc, argv);
		opt.config ();
		opt.merge ();





		return EXIT_SUCCESS;
	}
	catch (std::exception & ex)
	{
		std::cerr << "Ошибка: " << ex.what() << std::endl;
		return EXIT_FAILURE;
	}
}
