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
		char * id = get_random_id ();
		unsigned int length = 1 + ID_LENGTH;
		unsigned char * content = new unsigned char[length];
		content[0] = TYPE;
		content++;
		memcpy (content, id, query::ID_LENGTH);
		content--;

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
		buf += 1;
		char * id = new char[query::ID_LENGTH + 1];
		memcpy (id, buf, query::ID_LENGTH);
		id[ID_LENGTH] = 0;

		return
		{
			.type = hello::TYPE,
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
		memcpy (pos, id, strlen (id)); 					pos += strlen (id);
		pos[0] = (unsigned char)true;					pos += 1;
		pos[0] = (unsigned char)strlen (name);			pos += 1;
		memcpy (pos, name, strlen (name));				pos += strlen (name);
		pos[0] = (unsigned char)strlen (version);		pos += 1;
		memcpy (pos, version, strlen (version));		pos += strlen (version);

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
		char * id = new char[query::ID_LENGTH + 1];
		memcpy (id, buf, query::ID_LENGTH);
		id[query::ID_LENGTH] = 0;
		buf += query::ID_LENGTH;

		bool result = (bool)buf[0];
		buf += 1;

		unsigned int name_length = (int)buf[0];
		buf += 1;

		char * name = new char[name_length + 1];
		memcpy (name, buf, name_length);
		name[name_length] = 0;
		buf += name_length;

		unsigned int version_length = (int)buf[0];
		buf += 1;

		char * version = new char[version_length + 1];
		memcpy (version, buf, version_length);
		version[version_length] = 0;

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