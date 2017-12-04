#include <cstdlib>
#include <string>
#include <stdexcept>

#include "query.h"

using std::invalid_argument;

namespace query
{
	/**
	 * Получить случайный ID
	 */
	char * get_random_id ()
	{
		static const char alphanum[] = 	"0123456789abcdefghijklmnopqrstuvwxyz";

		char * id = new char[ID_LENGTH + 1];

		for (int i = 0; i < ID_LENGTH; ++i)
		{
			id[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
		}

		id[ID_LENGTH] = 0;

		return id;
	}

	/**
	 * Сообщение об ошибке
	 */
	void err (std::string query, std::string message)
	{
		throw invalid_argument ((std::string)"Запрос «" + query + "». " + message);
	}
}