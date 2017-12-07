#include <arpa/inet.h>

#include "query.h"
#include "isset.h"

namespace query::isset
{
	/**
	 * Создать запрос
	 * --------------------------------
	 * | type | id | key_length | key |
	 * --------------------------------
	 */
	query::result Request::make (char * key)
	{
		/* Создаём строку */
		unsigned int length =
				1 +						/* type */
				query::ID_LENGTH + 		/* id */
				2 + 					/* key_length */
				strlen (key); 			/* key */

		unsigned char * content = new unsigned char[length];
		unsigned char * pos = content;

		/* type */
		pos[0] = TYPE;
		pos += 1;

		/* id */
		char * id = query::get_random_id ();
		memcpy (pos, id, query::ID_LENGTH);
		pos += query::ID_LENGTH;

		/* key_length */
		if (strlen (key) > 65536)
		{
			err ("get", "Этап request-make. Название ключа не должна превышать 64 Кб.");
		}
		uint16_t key_length = strlen (key);
		key_length = htons (key_length);
		memcpy (pos, &key_length, 2);
		pos += 2;

		/* key */
		memcpy (pos, key, strlen (key));
		pos += strlen (key);

		return
		{
			.id = id,
			.content = content,
			.length = length
		};
	}

	/**
	 * Спарсить запрос
	 * --------------------------------
	 * | type | id | key_length | key |
	 * --------------------------------
	 */
	Request::data Request::parse (unsigned char * buf)
	{
		/* type */
		unsigned char * pos = buf;
		pos += 1;

		/* id */
		char * id = new char[query::ID_LENGTH + 1];
		memcpy (id, pos, query::ID_LENGTH);
		id[query::ID_LENGTH] = 0;
		pos += query::ID_LENGTH;

		/* key_length */
		uint16_t key_length = 0;
		memcpy (&key_length, pos, 2);
		key_length = ntohs (key_length);
		if (key_length > 65536)
		{
			err ("get", "Этап request-parse. Название ключа не должна превышать 64 Кб.");
		}
		pos += 2;

		/* key */
		char * key = new char[key_length + 1];
		memcpy (key, pos, key_length);
		key[key_length] = 0;
		pos += key_length;

		return
		{
			.type = TYPE,
			.id = id,
			.key_length = key_length,
			.key = key
		};
	}

	/**
	 * Создать ответ
	 * -----------------------
	 * | id | result | isset |
	 * -----------------------
	 */
	query::result Response::make (char * id, bool isset)
	{
		unsigned int length =
				query::ID_LENGTH + 		/* id */
				1 + 					/* result */
				1;						/* isset */

		unsigned char * content = new unsigned char[length];
		unsigned char * pos = content;

		/* id */
		memcpy (pos, id, strlen (id));
		pos += strlen (id);

		/* result */
		pos[0] = (unsigned char)true;
		pos += 1;

		/* isset */
		pos[0] = (unsigned char)isset;
		pos += 1;

		return
		{
			.id = strdup (id),
			.content = content,
			.length = length
		};
	}

	/**
	 * Спарсить ответ
	 * -----------------------
	 * | id | result | isset |
	 * -----------------------
	 */
	Response::data Response::parse (unsigned char * buf)
	{
		unsigned char * pos = buf;

		/* id */
		char * id = new char[query::ID_LENGTH + 1];
		memcpy (id, pos, query::ID_LENGTH);
		id[query::ID_LENGTH] = 0;
		pos += query::ID_LENGTH;

		/* result */
		bool result = (bool)pos[0];
		pos += 1;

		/* isset */
		bool isset = (bool)pos[0];
		pos += 1;

		return
		{
			.id = id,
			.result = result,
			.isset = isset
		};
	}
}