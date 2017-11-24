#pragma once

#include "../cashka/options.h"
#include "../cashka-cli/options.h"

namespace protocol
{
	/**
	 * Класс для работы с запросом «hello»
	 */
	class Hello
	{
		private:

			/* Запрос */
			struct query
			{
				const char type = 0x00;			/* Тип запроса */
				char * id;						/* ID запроса */
				char * data;					/* Строка запроса */
				unsigned int length;			/* Длина запроса */
			} query;

			/* Ответ */
			struct answer
			{
				char * id;						/* ID запроса, на который высылается ответ */
				bool result;					/* Результат запроса */
				unsigned int name_length;		/* Наименование сервера. Длина строки */
				char * name;					/* Наименование сервера */
				unsigned int version_length;	/* Версия сервера. Длина строки */
				char * version;					/* Версия сервера */
				char * data;					/* Строка ответа */
				unsigned int length;			/* Длина запроса */
			} answer;

		public:

			/* Конструктор и деструктор */
			Hello();
			~Hello ();

			/* Получить сведения по запросу и ответу */
			char * query_id ();
			char * query_data ();
			unsigned int query_length ();
			char * answer_data ();
			unsigned int answer_length ();
			char * answer_version ();

			/* Создать запрос */
			void query_make ();

			/* Спарсить запрос */
			void query_parse (char * buffer);

			/* Создать ответ */
			void answer_make (const char * name, const char * version);

			/* Спарсить ответ */
			void answer_parse (char * buffer);
	};
}
