#include <arpa/inet.h>

#include "query.h"
#include "set.h"

namespace query::set
{
	/**
	 * Создать запрос
	 * -------------------------------------------------------------
	 * | type | key_length | value_length | key | value | checksum |
	 * -------------------------------------------------------------
	 */
	query::result Request::make (char * key, char * value)
	{
		/* Создаём строку */
		unsigned int length =
			1 +						/* type */
			2 + 					/* key_length */
			4 + 					/* value_length */
			strlen (key) + 			/* key */
			strlen (value) +		/* value */
			4;						/* checksum */

		unsigned char * content = new unsigned char[length];
		unsigned char * pos = content;

		/* type */
		pos[0] = TYPE;
		pos += 1;

		/* key_length */
		if (strlen (key) > 65536)
		{
			err ("Этап request-make. Название ключа не должна превышать 64 Кб.");
		}
		uint16_t key_length = strlen (key);
		key_length = htons (key_length);
		memcpy (pos, &key_length, 2);
		pos += 2;

		/* value_length */
		if (strlen (value) > 4294967295)
		{
			err ("Этап request-make. Значение не должно превышать 4 Гб.");
		}
		uint32_t value_length = strlen (value);
		value_length = htonl (value_length);
		memcpy (pos, &value_length, 4);
		pos += 4;

		/* key */
		memcpy (pos, key, strlen (key));
		pos += strlen (key);

		/* value */
		memcpy (pos, value, strlen (value));
		pos += strlen (value);

		/* checksum */
		uint32_t checksum = query::checksum (content, length - 4);
		checksum = htonl (checksum);
		memcpy (pos, &checksum, 4);
		pos += 4;

		return
		{
			.content = content,
			.length = length
		};
	}

	/**
	 * Спарсить запрос
	 * -------------------------------------------------------------
	 * | type | key_length | value_length | key | value | checksum |
	 * -------------------------------------------------------------
	 */
	Request::data Request::parse (unsigned char * buf)
	{
		/* type */
		unsigned char * pos = buf;
		pos += 1;

		/* key_length */
		uint16_t key_length = 0;
		memcpy (&key_length, pos, 2);
		key_length = ntohs (key_length);
		pos += 2;

		/* value_length */
		uint32_t value_length = 0;
		memcpy (&value_length, pos, 4);
		value_length = ntohl (value_length);
		pos += 4;

		/* key */
		char * key = new char[key_length + 1];
		memcpy (key, pos, key_length);
		key[key_length] = 0;
		pos += key_length;

		/* value */
		char * value = new char[value_length + 1];
		memcpy (value, pos, value_length);
		value[value_length] = 0;
		pos += value_length;

		/* checksum */
		uint32_t checksum = 0;
		memcpy (&checksum, pos, 4);
		checksum = ntohl (checksum);
		if (checksum != query::checksum (buf, pos - buf))
		{
			err ("Этап request-parse. Неверная контрольная сумма.");
		}
		pos += 4;

		return
		{
			.type = TYPE,
			.key_length = key_length,
			.value_length = value_length,
			.key = key,
			.value = value,
			.checksum = checksum
		};
	}

	/**
	 * Создаём ответ
	 * ----------------------------
	 * | result (true) | checksum |
	 * ----------------------------
	 */
	query::result Response::make ()
	{
		unsigned int length = 1 + 4;
		unsigned char * content = new unsigned char[length];
		unsigned char * pos = content;

		/* result */
		pos[0] = (unsigned char)true;
		pos += 1;

		/* checksum */
		uint32_t checksum = query::checksum (content, length - 4);
		checksum = htonl (checksum);
		memcpy (pos, &checksum, 4);
		pos += 4;

		return
		{
			.content = content,
			.length = length
		};
	}

	/**
	 * Спарсим ответ
	 * ----------------------------
	 * | result (true) | checksum |
	 * ----------------------------
	 */
	Response::data Response::parse (unsigned char * buf)
	{
		unsigned char * pos = buf;

		/* result */
		bool result = (bool)pos[0];
		pos += 1;

		/* checksum */
		uint32_t checksum = 0;
		memcpy (&checksum, pos, 4);
		checksum = ntohl (checksum);
		if (checksum != query::checksum (buf, pos - buf))
		{
			err ("Этап response-parse. Неверная контрольная сумма.");
		}
		pos += 4;

		return
		{
			.result = result,
			.checksum = checksum
		};
	}
}