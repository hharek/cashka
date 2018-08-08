#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "confi.h"
#include "err.h"
#include "err.c"
#include "check.h"
#include "check.c"
#include "token.h"
#include "token.c"
#include "value.h"
#include "value.c"

/**
 * Спарсить файл
 */
int confi (const char * file, struct confi_param * params)
{
	confi_err ()->code = CONFI_SUCCESS;

	FILE * fp;
	int result = 0;

	/* Проверить конфигурационный файл */
	result = confi_file_check (file, &fp);
	if (result != 0)
	{
		return result;
	}

	/* Читаем файл */
	char content[CONFI_FILE_MAX_SIZE + 1];
	fread (content, CONFI_FILE_MAX_SIZE, 1, fp);

	/* Парсим */
	result = confi_parse_string (content, params);
	if (result != 0)
	{
		return result;
	}

	/* Закрыть файл */
    fclose (fp);

	return 0;
}

/**
 * Спрасить строку
 */
int confi_parse_string (const char * str, struct confi_param * params)
{
	confi_err ()->code = CONFI_SUCCESS;
	confi_err ()->message[0] = '\0';

	/* Проверка параметров */
	int result = confi_params_check (params);
	if (result != 0)
	{
		return result;
	}

	/* Разбираем строку на токены */
	struct token * tokens = token_parse_string (str);

	/* Ошибка при разборе строки на токены */
	if (confi_err ()->code != CONFI_SUCCESS)
	{
		return confi_err ()->code;
	}

	/* Проверяем возможность пустого файла */
	if (tokens == NULL)
	{
		struct confi_param * param = params;

		bool require = false;
		while (param->name != NULL)
		{
			if (param->require)
			{
				require = true;
				break;
			}

			param++;
		}

		if (require)
		{
			return err (CONFI_ERR_FILE_EMPTY_CONTENT, "Пустой файл", NULL);
		}
	}

	/* Заполняем значениями параметры */
	struct confi_param * param = params;
	struct token * t;
	while (param->name != NULL)
	{
		/* Назначаем параметрам значения с конфиг. файла */
		t = tokens;
		bool isset = false;
		while (t != NULL)
		{
			if (strcmp (param->name, t->content) == 0)
			{
				if (isset)
				{
					return err (CONFI_ERR_PARAM_REPEAT, "Параметр «%s» повторяется.", param->name);
				}

				isset = true;
				param->value = t->next->next->content;
				t->param_isset = true;
			}

			t = t->next->next->next->next;
		}

		/* Обязательный параметр */
		if (param->require && !isset)
		{
			return err (CONFI_ERR_PARAM_REQUIRE, "Параметр «%s» является обязательным для заполнения.", param->name);
		}

		param++;
	}

	/* Неизвестные параметры */
	t = tokens;
	while (t != NULL)
	{
		if (!t->param_isset)
		{
			return err (CONFI_ERR_PARAM_UNKNOWN, "Неизвестный параметр «%s».", t->content);
		}

		t = t->next->next->next->next;
	}

	/* Проверяем и назначаем данные */
	param = params;
	while (param->name != NULL)
	{
		if (param->value != NULL)
		{
			if (value_set (param) != 0)
			{
				return err_mess.code;
			}
		}

		param++;
	}

	/* Очищаем массив токенов */
	token_free (tokens);

	return 0;
}

/**
 * Вернуть последнее сообщение об ошибке
 */
struct confi_err * confi_err ()
{
	return &err_mess;
}