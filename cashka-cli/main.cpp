#include <cstdlib>

#include "options.h"
#include "client.h"

using std::string;

using cashka_cli::Options;
using cashka_cli::Client;

int main (int argc, char ** argv)
{
	/* Для функции rand */
	srand (time(0));

	try
	{
		/* Создаём опции */
		Options opt;
		opt.cli (argc, argv);
		opt.config ();
		opt.merge ();

		/* Обрабатываем команды */
		Client client (opt);
		client.connect_by_options ();
		client.on ();

		return EXIT_SUCCESS;
	}
	catch (std::exception & ex)
	{
		std::cerr << "Ошибка: " << ex.what() << std::endl;
		return EXIT_FAILURE;
	}
}
