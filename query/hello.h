#pragma once

#include "../cashka/options.h"
#include "../cashka-cli/options.h"

namespace query::hello
{
	const unsigned char TYPE = 0x00;

	/**
	 * Запрос
	 * -------------
	 * | type | id |
	 * -------------
	 */
	class Request
	{
		public:

			/* Данные запроса */
			struct data
			{
				const unsigned char type;		/* Тип */
				char * id;						/* ID  */
			};

			/* Создать */
			query::result make ();

			/* Спарсить */
			data parse (unsigned char * buf);
	};

	/**
	 * Ответ
	 * ---------------------------------------------------------------
	 * | id | result | name_length | name | version_length | version |
	 * ---------------------------------------------------------------
	 */
	class Response
	{
		public:

			/* Данные */
			struct data
			{
				char * id;						/* ID */
				bool result;					/* Результат */
				unsigned int name_length;		/* Имя сервера. Длина */
				char * name;					/* Имя сервера */
				unsigned int version_length;	/* Версия сервера. Длина */
				char * version;					/* Версия сервера */
			};

			/* Создать */
			query::result make (const char * id, const char * name, const char * version);

			/* Спарсить */
			data parse (unsigned char * buf);
	};
}
