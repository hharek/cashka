#pragma once

#include <cstdlib>

namespace protocol
{
	/* Кол-во длина ID */
	const int ID_LENGTH = 6;

	/* Получить случайный ID */
	char * get_random_id ();
}