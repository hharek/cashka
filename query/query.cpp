#include <cstdlib>
#include <cstring>
#include <string>
#include <stdexcept>
#include <stddef.h>
#include <stdint.h>
#include <arpa/inet.h>

#include "query.h"

using std::invalid_argument;

namespace query
{
	/**
	 * Получить контрольную сумму методом CRC32
	 */
	uint32_t checksum (const unsigned char * buf, unsigned int len)
	{
		/* Задаём полином */
		static const uint32_t POLY = 0xedb88320;		/* CRC-32 (Ethernet, ZIP, etc.) polynomial in reversed bit order. */
//		static const uint32_t POLY = 0x82f63b78;		/* CRC-32C (iSCSI) polynomial in reversed bit order.  */

		uint32_t crc = 0;
		crc = ~crc;

		while (len--)
		{
			crc ^= *buf++;
			for (int k = 0; k < 8; k++)
			{
				crc = crc & 1 ? (crc >> 1) ^ POLY : crc >> 1;
			}
		}

		return ~crc;
	}

	/**
	 * Сообщение об ошибке
	 */
	void err (const char * error)
	{
		throw error;
	}

	/**
	 * Создать пакет «Сообщение об ошибке»
	 * ----------------------------------------------------
	 * | result (false) | error_length | error | checksum |
	 * ----------------------------------------------------
	 */
	result err_make (const char * error)
	{
		/* content */
		unsigned int length =
				1 + 					/* result (false) */
				2 + 					/* error_length */
				strlen (error) +		/* error */
				4;						/* checksum */
		unsigned char * content = new unsigned char[length];
		unsigned char * pos = content;

		/* result (false) */
		pos[0] = (unsigned char)false;
		pos += 1;

		/* error_length */
		uint16_t error_length = strlen (error);
		error_length = htons (error_length);
		memcpy (pos, &error_length, 2);
		pos += 2;

		/* error */
		memcpy (pos, error, strlen (error));
		pos += strlen (error);

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
	 * Распарсить пакет «Сообщение об ошибке»
	 * ----------------------------------------------------
	 * | result (false) | error_length | error | checksum |
	 * ----------------------------------------------------
	 */
	char * err_parse (unsigned char * buf)
	{
		unsigned char * pos = buf;

		/* result */
		bool result = (bool)pos[0];
		pos += 1;

		/* error_length */
		uint16_t error_length = 0;
		memcpy (&error_length, pos, 2);
		error_length = ntohs (error_length);
		pos += 2;

		/* error */
		char * error = new char[error_length + 1];
		memcpy (error, pos, error_length);
		error[error_length] = 0;
		pos += error_length;

		/* checksum */
		uint32_t checksum = 0;
		memcpy (&checksum, pos, 4);
		checksum = ntohl (checksum);
		if (checksum != query::checksum (buf, pos - buf))
		{
			err ("Этап response-parse. Неверная контрольная сумма.");
		}
		pos += 4;

		return error;
	}
}