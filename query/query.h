#pragma once

#include <string>
#include <cstdlib>

namespace query
{
	/* Максимальный размер пакета = Ethernet MSS */
	const unsigned int pack_max_size = 1460;

	/**
	 * Результат создания запроса или ответа
	 */
	struct result
	{
		unsigned char * content;
		unsigned int length;
	};

	/* Получить контрольую сумму */
	uint32_t checksum (const unsigned char * buf, unsigned int len);

	/* Сгенерировать исключение с текстом ошибки */
	void err (const char * error);

	/**
	 * Создать пакет «Сообщение об ошибке»
	 * ----------------------------------------------------
	 * | result (false) | error_length | error | checksum |
	 * ----------------------------------------------------
	 */
	result err_make (const char * error);

	/**
	 * Распарсить пакет «Сообщение об ошибке»
	 * ----------------------------------------------------
	 * | result (false) | error_length | error | checksum |
	 * ----------------------------------------------------
	 */
	char * err_parse (unsigned char * buf);
}