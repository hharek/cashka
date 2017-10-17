#include <string>
#include <exception>
#include <stdexcept>

#include "cashka.h"

using std::string;
using std::invalid_argument;

namespace cashka
{
	/**
	 * Сгенерировать исключение
	 * 
	 * @param string type
	 * @param string message
	 * @return void
	 */
	void err (string message, string type)
	{
		if (type == "default")
		{
			
		}
		else if (type == "cli")
		{
			message = (string)"Командная строка. " + message;
		}
		else if (type == "config")
		{
			message = (string)"Конфигурационный файл. " + message;
		}

		throw invalid_argument (message);
	}
}