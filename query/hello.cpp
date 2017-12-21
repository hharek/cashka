#include <cstring>
#include <arpa/inet.h>

#include "query.h"
#include "hello.h"

namespace query::hello
{
	/**
	 * Создать запрос
	 * -------------------
	 * | type | checksum |
	 * -------------------
	 */
	query::result Request::make ()
	{
		/* content */
		unsigned int length =
					1 + 					/* type */
					4;						/* checksum */
		unsigned char * content = new unsigned char[length];
		unsigned char * pos = content;

		/* type */
		pos[0] = TYPE;
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
	 * Спарсить запрос
	 * -------------------
	 * | type | checksum |
	 * -------------------
	 */
	Request::data Request::parse (unsigned char * buf)
	{
		unsigned char * pos = buf;

		/* type */
		pos += 1;

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
			.checksum = checksum
		};
	}

	/**
	 * Создаём ответ
	 * -----------------------------------------------------------------------------
	 * | result (true)  | name_length | version_length | name | version | checksum |
	 * -----------------------------------------------------------------------------
	 */
	query::result Response::make (const char * name, const char * version)
	{
		/* content */
		unsigned int length =
				1 +							/* result (true) */
				1 +							/* name_length */
				1 +							/* version_length */
				strlen (name) +				/* name */
				strlen (version) + 			/* version */
				4;							/* checksum */
		unsigned char * content = new unsigned char [length];
		unsigned char * pos = content;

		/* result (true) */
		pos[0] = (unsigned char)true;
		pos += 1;

		/* name_length */
		pos[0] = (unsigned char)strlen (name);
		pos += 1;

		/* version_legnth */
		pos[0] = (unsigned char)strlen (version);
		pos += 1;

		/* name */
		memcpy (pos, name, strlen (name));
		pos += strlen (name);

		/* version */
		memcpy (pos, version, strlen (version));
		pos += strlen (version);

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
	 * -----------------------------------------------------------------------------
	 * | result (true)  | name_length | version_length | name | version | checksum |
	 * -----------------------------------------------------------------------------
	 */
	Response::data Response::parse (unsigned char * buf)
	{
		unsigned char * pos = buf;

		/* result */
		bool result = (bool)pos[0];
		pos += 1;

		/* name_length */
		uint8_t name_length = (uint8_t)pos[0];
		pos += 1;

		/* version_length */
		uint8_t version_length = (uint8_t)pos[0];
		pos += 1;

		/* name */
		char * name = new char[name_length + 1];
		memcpy (name, pos, name_length);
		name[name_length] = 0;
		pos += name_length;

		/* version */
		char * version = new char[version_length + 1];
		memcpy (version, pos, version_length);
		version[version_length] = 0;
		pos += version_length;

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
			.name_length = name_length,
			.version_length = version_length,
			.name = name,
			.version = version,
			.checksum = checksum
		};
	}
}