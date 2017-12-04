#include "query.h"
#include "hello.h"

#include <cstring>

namespace query::hello
{
	/**
	 * Создать запрос
	 * -------------
	 * | type | id |
	 * -------------
	 */
	query::result Request::make ()
	{
		unsigned int length = 1 + ID_LENGTH;
		unsigned char * content = new unsigned char[length];
		unsigned char * pos = content;

		pos[0] = TYPE;
		pos += 1;

		char * id = get_random_id ();
		memcpy (pos, id, strlen (id));
		pos += strlen (id);

		return
		{
			.id = id,
			.content = content,
			.length = length
		};
	}

	/**
	 * Спарсить запрос
	 * -------------
	 * | type | id |
	 * -------------
	 */
	Request::data Request::parse (unsigned char * buf)
	{
		unsigned char * pos = buf;
		pos += 1;

		char * id = new char[query::ID_LENGTH + 1];
		memcpy (id, pos, query::ID_LENGTH);
		id[query::ID_LENGTH] = 0;
		pos += query::ID_LENGTH;

		return
		{
			.type = TYPE,
			.id = id
		};
	}

	/**
	 * Создаём ответ
	 * ---------------------------------------------------------------
	 * | id | result | name_length | name | version_length | version |
	 * ---------------------------------------------------------------
	 */
	query::result Response::make (const char * id, const char * name, const char * version)
	{
		unsigned int length =
				strlen (id) +				/* id */
				1 +							/* result */
				1 +							/* name_length */
				strlen (name) +				/* name */
				1 +							/* version_length */
				strlen (version);			/* version */

		unsigned char * content = new unsigned char [length];
		unsigned char * pos = content;

		memcpy (pos, id, strlen (id));
		pos += strlen (id);

		pos[0] = (unsigned char)true;
		pos += 1;

		pos[0] = (unsigned char)strlen (name);
		pos += 1;

		memcpy (pos, name, strlen (name));
		pos += strlen (name);

		pos[0] = (unsigned char)strlen (version);
		pos += 1;

		memcpy (pos, version, strlen (version));
		pos += strlen (version);

		return
		{
			.id = strdup (id),
			.content = content,
			.length = length
		};
	}

	/**
	 * Спарсим ответ
	 * ---------------------------------------------------------------
	 * | id | result | name_length | name | version_length | version |
	 * ---------------------------------------------------------------
	 */
	Response::data Response::parse (unsigned char * buf)
	{
		unsigned char * pos = buf;

		char * id = new char[query::ID_LENGTH + 1];
		memcpy (id, pos, query::ID_LENGTH);
		id[query::ID_LENGTH] = 0;
		pos += query::ID_LENGTH;

		bool result = (bool)pos[0];
		pos += 1;

		uint8_t name_length = (uint8_t)pos[0];
		pos += 1;

		char * name = new char[name_length + 1];
		memcpy (name, pos, name_length);
		name[name_length] = 0;
		pos += name_length;

		uint8_t version_length = (uint8_t)pos[0];
		pos += 1;

		char * version = new char[version_length + 1];
		memcpy (version, pos, version_length);
		version[version_length] = 0;
		pos += version_length;

		return
		{
			.id = id,
			.result = result,
			.name_length = name_length,
			.name = name,
			.version_length = version_length,
			.version = version
		};
	}
}