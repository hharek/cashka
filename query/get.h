#pragma once

#include "../cashka/options.h"
#include "../cashka-cli/options.h"
#include "../query/query.h"

namespace query::get
{
	const unsigned char TYPE = 0x04;

	/**
	 * Запрос
	 * --------------------------------
	 * | type | id | key_length | key |
	 * --------------------------------
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
			};

			/* Создать */
			query::result make (char * key);

			/* Спарсить */
			data parse (unsigned char * buf);
	};

	/**
	 * Ответ
	 * ----------------------------------------------
	 * | id | result | isset | value_length | value |
	 * ----------------------------------------------
	 */
	class Response
	{
		public:

			/* Данные */
			struct data
			{
				char * id;						/* ID */
				bool result;					/* Результат */
				bool isset;						/* Существует ли ключ */
				uint32_t value_length;			/* Значение. Длина (4 байта = от 1 до 4294967295) */
				char * value;					/* Значение */
			};

			/* Создать */
			query::result make (char * id, char * value);

			/* Создать ответ «ключ отсутствует» */
			query::result make_false (char * id);

			/* Спарсить */
			data parse (unsigned char * buf);
	};
}