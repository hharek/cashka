#pragma once

#include "../cashka/options.h"
#include "../cashka-cli/options.h"
#include "../query/query.h"

namespace query::set
{
	const unsigned char TYPE = 0x06;

	/**
	 * Запрос
	 * -------------------------------------------------------------
	 * | type | key_length | value_length | key | value | checksum |
	 * -------------------------------------------------------------
	 */
	class Request
	{
		public:

			/* Данные запроса */
			struct data
			{
				unsigned char type;					/* Тип */
				uint16_t key_length;				/* Ключ. Длина (2 байта = от 1 до 65535) */
				uint32_t value_length;				/* Значение. Длина (4 байта = от 1 до 4294967295) */
				char * key;							/* Ключ */
				char * value;						/* Значение */
				uint32_t checksum;					/* Контрольная сумма */
			};

			/* Создать */
			query::result make (char * key, char * value);

			/* Спарсить */
			data parse (unsigned char * buf);
	};

	/**
	 * Ответ
	 * ----------------------------
	 * | result (true) | checksum |
	 * ----------------------------
	 */
	class Response
	{
		public:

			/* Данные */
			struct data
			{
				bool result;					/* Результат */
				uint32_t checksum;				/* Контрольная сумма */
			};

			/* Создать */
			query::result make ();

			/* Спарсить */
			data parse (unsigned char * buf);
	};
}