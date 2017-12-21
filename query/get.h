#pragma once

#include "../cashka/options.h"
#include "../cashka-cli/options.h"
#include "../query/query.h"

namespace query::get
{
	const unsigned char TYPE = 0x04;

	/**
	 * Запрос
	 * --------------------------------------
	 * | type | key_length | key | checksum |
	 * --------------------------------------
	 */
	class Request
	{
		public:

			/* Данные запроса */
			struct data
			{
				unsigned char type;				/* Тип */
				uint16_t key_length;			/* Ключ. Длина (2 байта = от 1 до 65535) */
				char * key;						/* Ключ */
				uint32_t checksum;				/* Контрольная сумма */
			};

			/* Создать */
			query::result make (char * key);

			/* Спарсить */
			data parse (unsigned char * buf);
	};

	/**
	 * Ответ
	 * -------------------------------------------------------------------
	 * | result (true) | isset (true)  | value_length | value | checksum |
	 * -------------------------------------------------------------------
	 * --------------------------------------------
	 * | result (true) | isset (false) | checksum |
	 * --------------------------------------------
	 */
	class Response
	{
		public:

			/* Данные */
			struct data
			{
				bool result;					/* Результат */
				bool isset;						/* Существует ли ключ */
				uint32_t value_length;			/* Значение. Длина (4 байта = от 1 до 4294967295) */
				char * value;					/* Значение */
				uint32_t checksum;				/* Контрольная сумма */
			};

			/* Создать */
			query::result make (char * value);

			/* Создать ответ «ключ отсутствует» */
			query::result make_false ();

			/* Спарсить */
			data parse (unsigned char * buf);
	};
}