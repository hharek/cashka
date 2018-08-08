#include <stdlib.h>
#include <string.h>

/**
 * Проверить строку на соответствие типу и назначить по адресу
 */
int value_set (struct confi_param * param)
{
	switch (param->type)
	{
		case CONFI_TYPE_INT:
			return value_set_int (param->name, param->value, param->ptr);

		case CONFI_TYPE_UINT:
			return value_set_uint (param->name, param->value, param->ptr);

		case CONFI_TYPE_DOUBLE:
			return  value_set_double (param->name, param->value, param->ptr);

		case CONFI_TYPE_STRING:
			return  value_set_string (param->name, param->value, param->ptr);

		case CONFI_TYPE_BOOLEAN:
			return  value_set_bool (param->name, param->value, param->ptr);
	}

	return 0;
}

/**
 * Проверить и назначить как число со знаком
 */
int value_set_int (const char * name, const char * value, void * ptr)
{
	char * end;
	strtol (value, &end, 0);
	if (*end)
	{
		return err (CONFI_ERR_VALUE_NOT_INT, "Параметр «%s» не является числом со знаком.", name);
	}

	if (ptr != NULL)
	{
		int value_int = (int)strtol (value, &end, 0);
		*((int *)ptr) = value_int;
	}

	return 0;
}

/**
 * Проверить и назначить как число без знаком
 */
int value_set_uint (const char * name, const char * value, void * ptr)
{
	if (value[0] == '-' || value[0] == '+')
	{
		return err (CONFI_ERR_VALUE_NOT_UINT, "Параметр «%s» не является числом без знака.", name);
	}

	char * end;
	strtoul (value, &end, 0);
	if (*end)
	{
		return err (CONFI_ERR_VALUE_NOT_UINT, "Параметр «%s» не является числом без знака.", name);
	}

	if (ptr != NULL)
	{
		unsigned int value_int = (unsigned int)strtoul (value, &end, 0);
		*((unsigned int *)ptr) = value_int;
	}

	return 0;
}

/**
 *  Проверить и назначить как число с плавающей точкой
 */
int value_set_double (const char * name, const char * value, void * ptr)
{
	char * end;
	strtod (value, &end);
	if (*end)
	{
		return err (CONFI_ERR_VALUE_NOT_DOUBLE, "Параметр «%s» не является числом с плавающей запятой.", name);
	}

	if (ptr != NULL)
	{
		double value_double = strtod (value, &end);
		*((double *) ptr) = value_double;
	}

	return 0;
}

/**
 * Проверить и назначить как строку
 */
int value_set_string (const char * name, const char * value, void * ptr)
{
	if (strlen (value) > CONFI_VALUE_STRING_MAX_LENGTH)
	{
		return err (CONFI_ERR_VALUE_BIG_STRING, "Параметр «%s» содержит слишком длинную строку.", name);
	}

	if (ptr != NULL)
	{
		*((char **)ptr) = strdup (value);
	}

	return 0;
}

/**
 * Проверить и назначить как булёвое значение
 */
int value_set_bool (const char * name, const char * value, void * ptr)
{
	if
	(
		strcmp (value, "1") != 0 &&
		strcmp (value, "0") != 0 &&
		strcmp (value, "true") != 0 &&
		strcmp (value, "false") != 0 &&
		strcmp (value, "yes") != 0 &&
		strcmp (value, "no") != 0 &&
		strcmp (value, "on") != 0 &&
		strcmp (value, "off") != 0
	)
	{
		return err (CONFI_ERR_VALUE_NOT_BOOLEAN, "Параметр «%s» не является булёвым значением. Допустимые значения «1, 0, true, false, yes, no, on, off»", name);
	}

	if (ptr != NULL)
	{
		bool value_bool = false;

		if
		(
			strcmp (value, "1") == 0 ||
			strcmp (value, "true") == 0 ||
			strcmp (value, "yes") == 0 ||
			strcmp (value, "on") == 0)
		{
			value_bool = true;
		}
		else if
		(
			strcmp (value, "0") == 0 ||
			strcmp (value, "false") == 0 ||
			strcmp (value, "no") == 0 ||
			strcmp (value, "off") == 0
		)
		{
			value_bool = false;
		}

		*((bool *) ptr) = value_bool;
	}

	return 0;
}