#pragma once

#include "../cashka/options.h"
#include "../cashka-cli/options.h"
#include "../query/query.h"

namespace query::isset
{
	const unsigned char TYPE = 0x06;

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
	 * -----------------------
	 * | id | result | isset |
	 * -----------------------
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
			};

			/* Создать */
			query::result make (char * id, bool isset);

			/* Спарсить */
			data parse (unsigned char * buf);
	};
}