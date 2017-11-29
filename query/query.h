#pragma once

#include <cstdlib>

namespace query
{
	/* Результат создания запроса или ответа */
	struct result
	{
		char * id;
		unsigned char * content;
		unsigned int length;
	};

	/* Кол-во длина ID */
	const unsigned int ID_LENGTH = 6;

	/* Получить случайный ID */
	char * get_random_id ();
}