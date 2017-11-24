#include "protocol.h"
#include "hello.h"

#include <cstring>

namespace protocol
{
	/**
	 * Конструктор
	 */
	Hello::Hello ()
	{
		
	}

	/**
	 * Деструктор
	 */
	Hello::~Hello ()
	{

	}

	/* Вернуть данные по запросу */
	char * Hello::query_id () 				{ return query.id;			}
	char * Hello::query_data () 			{ return query.data;   		}
	unsigned int Hello::query_length ()		{ return query.length; 		}
	char * Hello::answer_data () 			{ return answer.data; 		}
	unsigned int Hello::answer_length () 	{ return answer.length;		}
	char * Hello::answer_version () 		{ return answer.version;	}

	/**
	 * Создать запрос hello
	 */
	void Hello::query_make ()
	{
		query.id = get_random_id ();
		query.length = 1 + ID_LENGTH;		// type + id
		query.data = new char[query.length];
		query.data[0] = query.type;
		query.data++;
		strncpy (query.data, query.id, ID_LENGTH);
		query.data--;
	}

	/**
	 * Спарсить запрос
	 */
	void Hello::query_parse (char * buffer)
	{
		query.id = buffer + 1;
		query.data = buffer;
		query.length = 1 + ID_LENGTH;
	}

	/**
	 * Создать ответ
	 */
	void Hello::answer_make (const char * name, const char * version)
	{
		/* Временные переменные */
		char * _name = new char[strlen(name) + 1];
		strcpy (_name, name);
		char * _version = new char[strlen(version) + 1];
		strcpy (_version, version);

		/* Формируем ответ */
		answer =
		{
			.id = query.id,
			.result = true,
			.name_length = (unsigned int)strlen (name),
			.name = _name,
			.version_length = (unsigned int)strlen (version),
			.version = _version
		};

		/**
		 * Создаём такой бинарник
		 * ---------------------------------------------------------------
		 * | id | result | name_length | name | version_length | version |
		 * ---------------------------------------------------------------
		 */
		answer.length +=
			strlen (answer.id) +			/* id */
			1 +								/* result */
			1 +								/* name_length */
			strlen (answer.name) +			/* name */
			1 +								/* version_length */
			strlen (answer.version);		/* version */

		answer.data = new char [answer.length];

		char * pos = answer.data;
		strncpy (pos, answer.id, strlen (answer.id)); 			pos += strlen (answer.id);
		pos[0] = (char)answer.result;							pos += 1;
		pos[0] = (char)answer.name_length;						pos += 1;
		strncpy (pos, answer.name, strlen (answer.name)); 		pos += strlen (answer.name);
		pos[0] = (char)answer.version_length;					pos += 1;
		strncpy (pos, answer.version, strlen (answer.version));	pos += strlen (answer.version);
	}

	/**
	 * Спарсить ответ
	 */
	void Hello::answer_parse (char * buffer)
	{
		/**
		 * Распарсиваем
		 * ---------------------------------------------------------------
		 * | id | result | name_length | name | version_length | version |
		 * ---------------------------------------------------------------
		 */

		answer.id = new char[ID_LENGTH + 1];
		strncpy (answer.id, buffer, ID_LENGTH);
		answer.id[ID_LENGTH] = 0;
		buffer += ID_LENGTH;

		answer.result = (bool)buffer[0];
		buffer += 1;

		answer.name_length = (int)buffer[0];
		buffer += 1;

		answer.name = new char[answer.name_length + 1];
		strncpy (answer.name, buffer, answer.name_length);
		answer.name[answer.name_length] = 0;
		buffer += answer.name_length;

		answer.version_length = (int)buffer[0];
		buffer += 1;

		answer.version = new char[answer.version_length + 1];
		strncpy (answer.version, buffer, answer.version_length);
		answer.version[answer.version_length] = 0;

		std::cout << "Сервер: " << answer.name << " " << answer.version << std::endl;
	}
}