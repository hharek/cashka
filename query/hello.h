#pragma once

#include "../cashka/options.h"
#include "../cashka-cli/options.h"

namespace query::hello
{
	const unsigned char TYPE = 0x00;

	/**
	 * Запрос
	 * -------------------
	 * | type | checksum |
	 * -------------------
	 */
	class Request
	{
		public:

			/* Данные запроса */
			struct data
			{
				unsigned char type;				/* Тип */
				uint32_t checksum;				/* Контрольная сумма */
			};

			/* Создать */
			query::result make ();

			/* Спарсить */
			data parse (unsigned char * buf);
	};

	/**
	 * Ответ
	 * -----------------------------------------------------------------------------
	 * | result (true)  | name_length | version_length | name | version | checksum |
	 * -----------------------------------------------------------------------------
	 */
	class Response
	{
		public:

			/* Данные */
			struct data
			{
				uint8_t name_length;			/* Имя сервера. Длина (1 байт = от 1 до 255) */
				uint8_t version_length;			/* Версия сервера. Длина (1 байт = от 1 до 255)  */
				char * name;					/* Имя сервера */
				char * version;					/* Версия сервера */
				uint32_t checksum;				/* Контрольная сумма */
			};

			/* Создать */
			query::result make (const char * name, const char * version);

			/* Спарсить */
			data parse (unsigned char * buf);
	};
}