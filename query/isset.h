#pragma once

#include "../cashka/options.h"
#include "../cashka-cli/options.h"
#include "../query/query.h"

namespace query::isset
{
	const unsigned char TYPE = 0x08;

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
	 * ------------------------------------
	 * | result (true) | isset | checksum |
	 * ------------------------------------
	 */
	class Response
	{
		public:

			/* Данные */
			struct data
			{
				bool result;					/* Результат */
				bool isset;						/* Существует ли ключ */
				uint32_t checksum;				/* Контрольная сумма */
			};

			/* Создать */
			query::result make (bool isset);

			/* Спарсить */
			data parse (unsigned char * buf);
	};
}