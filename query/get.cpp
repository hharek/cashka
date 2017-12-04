#include "query.h"
#include "get.h"
#include <arpa/inet.h>

namespace query::get
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
				ID_LENGTH + 			/* id */
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
		id[ID_LENGTH] = 0;
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
	 * ----------------------------------------------
	 * | id | result | isset | value_length | value |
	 * ----------------------------------------------
	 */
	query::result Response::make (char * id, char * value)
	{
		unsigned int length =
				ID_LENGTH + 			/* id */
				1 + 					/* result */
				1 +						/* isset */
				4 + 					/* value_length */
				strlen (value);			/* value */

		unsigned char * content = new unsigned char[length];
		unsigned char * pos = content;

		/* id */
		memcpy (pos, id, strlen (id));
		pos += strlen (id);

		/* result */
		pos[0] = (unsigned char)true;
		pos += 1;

		/* isset */
		pos[0] = (unsigned char)true;
		pos += 1;

		/* value_length */
		if (strlen (value) > 4294967295)
		{
			err ("get", "Этап request-make. Значение не должно превышать 4 Гб.");
		}

		uint32_t value_length = strlen (value);
		value_length = htonl (value_length);
		memcpy (pos, &value_length, 4);
		pos += 4;

		/* value */
		if (value != nullptr)
		{
			memcpy (pos, value, strlen (value));
			pos += strlen (value);
		}

		return
		{
			.id = strdup (id),
			.content = content,
			.length = length
		};
	}

	/**
	 * Создать ответ «ключ отсутствует»
	 * -----------------------
	 * | id | result | isset |
	 * -----------------------
	 */
	query::result Response::make_false (char * id)
	{
		unsigned int length =
				ID_LENGTH + 			/* id */
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
		pos[0] = (unsigned char)false;
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
	 * ----------------------------------------------
	 * | id | result | isset | value_length | value |
	 * ----------------------------------------------
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

		/* value_length */
		uint32_t value_length = 0;
		memcpy (&value_length, pos, 4);
		value_length = ntohl (value_length);
		if (value_length > 4294967295)
		{
			err ("get", "Этап request-parse. Значение не должно превышать 4 Гб.");
		}
		pos += 4;

		/* value */
		char * value = nullptr;
		if (value_length != 0)
		{
			value = new char[value_length + 1];
			memcpy (value, pos, value_length);
			value[value_length] = 0;
			pos += value_length;
		}

		return
		{
			.id = id,
			.result = result,
			.isset = isset,
			.value_length = value_length,
			.value = value
		};
	}
}