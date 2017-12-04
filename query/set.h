#pragma once

#include "../cashka/options.h"
#include "../cashka-cli/options.h"
#include "../query/query.h"

namespace query::set
{
	const unsigned char TYPE = 0x05;

	/**
	 * Запрос
	 * -------------------------------------------------------
	 * | type | id | key_length | key | value_length | value |
	 * -------------------------------------------------------
	 */
	class Request
	{
		public:

			/* Данные запроса */
			struct data
			{
				const unsigned char type;		/* Тип */
				char * id;						/* ID  */
				uint16_t key_length;			/* Ключ. Длина (2 байта = от 1 до 65535) */
				char * key;						/* Ключ */
				uint32_t value_length;			/* Значение. Длина (4 байта = от 1 до 4294967295) */
				char * value;					/* Значение */
			};

			/* Создать */
			query::result make (char * key, char * value);

			/* Спарсить */
			data parse (unsigned char * buf);
	};

	/**
	 * Ответ
	 * ---------------
	 * | id | result |
	 * ---------------
	 */
	class Response
	{
		public:

			/* Данные */
			struct data
			{
				char * id;						/* ID */
				bool result;					/* Результат */
			};

			/* Создать */
			query::result make (char * id);

			/* Спарсить */
			data parse (unsigned char * buf);
	};
}