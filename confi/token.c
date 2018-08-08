#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/**
 * Разбираем файл на токены
 */
struct token * token_parse_string (const char * str)
{
	static int token_type;
	static char         buf[TOKEN_BUF_SIZE + 1];
	static unsigned int buf_size;

	token_type = TOKEN_BLANK;
	buf[0] = '\0';
	buf_size = 0;

	struct token * tokens = NULL;

	char ch;
	char * pos = (char *)str;

	ch = *(pos++);
	while (1)
	{
		switch (token_type)
		{
			/* Код */
			case TOKEN_BLANK:
				token_type = token_blank (ch, buf, &buf_size, &tokens);
				break;

			/* Комментарий */
			case TOKEN_COMMENT:
				token_type = token_comment (ch, buf, &buf_size, &tokens);
				break;

			/* Идентификатор */
			case TOKEN_WORD:
				token_type = token_word (ch, buf, &buf_size, &tokens);
				break;

			/* Строка в одинарных кавычках */
			case TOKEN_STRING_QUOTES_SINGLE:
				token_type = token_string_quotes_single (ch, buf, &buf_size, &tokens);
				break;

			/* Строка в одинарных кавычках. Экранирование */
			case TOKEN_STRING_QUOTES_SINGLE_ESCAPE:
				token_type = token_string_quotes_single_escape (ch, buf, &buf_size, &tokens);
				break;

			/* Строка в двойных кавычках */
			case TOKEN_STRING_QUOTES_DOUBLE:
				token_type = token_string_quotes_double (ch, buf, &buf_size, &tokens);
				break;

			/* Строка в двойных кавычках. Экранирование */
			case TOKEN_STRING_QUOTES_DOUBLE_ESCAPE:
				token_type = token_string_quotes_double_escape (ch, buf, &buf_size, &tokens);
				break;

			/* Ошибка при парсинге */
			default:
				return NULL;
		}

		if (ch == '\0')
		{
			break;
		}

		ch = *(pos++);
	}

	/* Проверяем порядок следования токенов */
	if (token_check_order (tokens) != 0)
	{
		return NULL;
	}

	return tokens;
}

/**
 * Очистить
 */
void token_free (struct token * tokens)
{
	struct token * t = tokens;
	while (t != NULL)
	{
		static struct token * old;
		old = t;
		t = t->next;
		free (old->content);
		free (old);
	}
}

/**
 * Токен «код»
 */
int token_blank (char ch, char * buf, unsigned int * buf_size, struct token ** tokens)
{
	if (ch == '#')
	{
		return TOKEN_COMMENT;
	}
	else if (isblank (ch) || ch == '\r' || ch == '\n' || ch == '\0')
	{
		return TOKEN_BLANK;
	}
	else if (ch == '=')
	{
		return token_equal (tokens);
	}
	else if (ch == ';')
	{
		return token_semicolon (tokens);
	}
	else if (ch == '\'')
	{
		return TOKEN_STRING_QUOTES_SINGLE;
	}
	else if (ch == '"')
	{
		return TOKEN_STRING_QUOTES_DOUBLE;
	}
	else
	{
		return token_word (ch, buf, buf_size, tokens);
	}
}

/**
 * Токен «комментарий»
 */
int token_comment (char ch, char * buf, unsigned int * buf_size, struct token ** tokens)
{
	if (ch == '\n')
	{
		return TOKEN_BLANK;
	}
	else
	{
		return TOKEN_COMMENT;
	}
}

/**
 * Токен «слово»
 */
int token_word (char ch, char * buf, unsigned int * buf_size, struct token ** tokens)
{
	if (isblank (ch) || ch == '\r' || ch == '\n' || ch == '=' || ch == ';' || ch == '\0')
	{
		buf[*buf_size] = '\0';
		*buf_size = 0;
		token_push (TOKEN_WORD, strdup (buf), tokens);

		return token_blank (ch, buf, buf_size, tokens);
	}
	else
	{
		buf[*buf_size] = ch;
		(*buf_size)++;

		if ((*buf_size) >= TOKEN_WORD_MAX_SIZE)
		{
			return err (CONFI_ERR_TOKEN_WORD_MAX_SIZE, "Превышен размер токена «слово».", NULL);
		}

		return TOKEN_WORD;
	}
}

/**
 * Токен «строка» в одинарных кавычках
 */
int token_string_quotes_single (char ch, char * buf, unsigned int * buf_size, struct token ** tokens)
{
	if (ch == '\'')
	{
		buf[*buf_size] = '\0';
		*buf_size = 0;
		token_push (TOKEN_STRING, strdup (buf), tokens);

		return TOKEN_BLANK;
	}
	else if (ch == '\\')
	{
		return TOKEN_STRING_QUOTES_SINGLE_ESCAPE;
	}
	else
	{
		buf[*buf_size] = ch;
		(*buf_size)++;

		if ((*buf_size) >= TOKEN_STRING_MAX_SIZE)
		{
			return err (CONFI_ERR_TOKEN_STRING_MAX_SIZE, "Превышен размер токена «строка».", NULL);
		}

		return TOKEN_STRING_QUOTES_SINGLE;
	}
}

/**
 * Токен «строка» в одинарных кавычках. Экранирование
 */
int token_string_quotes_single_escape (char ch, char * buf, unsigned int * buf_size, struct token ** tokens)
{
	if (ch == '\'')
	{
		buf[*buf_size] = '\'';
		(*buf_size)++;
	}
	else
	{
		buf[*buf_size] = '\\';
		(*buf_size)++;

		buf[*buf_size] = ch;
		(*buf_size)++;
	}

	return TOKEN_STRING_QUOTES_SINGLE;
}

/**
 * Токен «строка» в двойных кавычках
 */
int token_string_quotes_double (char ch, char * buf, unsigned int * buf_size, struct token ** tokens)
{
	if (ch == '"')
	{
		buf[*buf_size] = '\0';
		*buf_size = 0;
		token_push (TOKEN_STRING, strdup (buf), tokens);

		return TOKEN_BLANK;
	}
	else if (ch == '\\')
	{
		return TOKEN_STRING_QUOTES_DOUBLE_ESCAPE;
	}
	else
	{
		buf[*buf_size] = ch;
		(*buf_size)++;

		if ((*buf_size) >= TOKEN_STRING_MAX_SIZE)
		{
			return err (CONFI_ERR_TOKEN_STRING_MAX_SIZE, "Превышен размер токена «строка».", NULL);
		}

		return TOKEN_STRING_QUOTES_DOUBLE;
	}
}

/**
 * Токен «строка» в двойных кавычках. Экранирование
 */
int token_string_quotes_double_escape (char ch, char * buf, unsigned int * buf_size, struct token ** tokens)
{
	if (ch == 'n' || ch == 'r' || ch == 't' || ch == 'v' || ch == 'e' || ch == 'f' || ch == '\\' || ch == '"')
	{
		char symbol = '\0';
		switch (ch)
		{
			case 'n':
				symbol = '\n';
				break;
			case 'r':
				symbol = '\r';
				break;
			case 't':
				symbol = '\t';
				break;
			case 'v':
				symbol = '\v';
				break;
			case 'e':
				symbol = 0x1B;		// escape знак
				break;
			case 'f':
				symbol = '\f';
				break;
			case '\\':
				symbol = '\\';
				break;
			case '"':
				symbol = '"';
				break;
		}

		buf[*buf_size] = symbol;
		(*buf_size)++;
	}
	else
	{
		buf[*buf_size] = '\\';
		(*buf_size)++;

		buf[*buf_size] = ch;
		(*buf_size)++;
	}

	return TOKEN_STRING_QUOTES_DOUBLE;
}

/**
 * Токен «=»
 */
int token_equal (struct token ** tokens)
{
	token_push (TOKEN_EQUAL, strdup ("="), tokens);

	return TOKEN_BLANK;
}

/**
 * Токен «;»
 */
int token_semicolon (struct token ** tokens)
{
	token_push (TOKEN_SEMICOLON, strdup (";"), tokens);

	return TOKEN_BLANK;
}

/**
 * Добавить токен
 */
void token_push (int type, char * content, struct token ** tokens)
{
	struct token * next = (struct token *)malloc (sizeof (struct token));
	next->type = type;
	next->content = content;
	next->next = NULL;

	/* Первый элемент */
	if (* tokens == NULL)
	{
		* tokens = next;
		return;
	}

	/* Находим последний элемент и добавляем */
	struct token * i = * tokens;
	while (i->next != NULL)
	{
		i = i->next;
	}

	i->next = next;
}

/**
 * Проверяем порядок расположения токенов
 */
int token_check_order (struct token * tokens)
{
	struct token * i = tokens;
	while (i != NULL)
	{
		if
		(
			i->type != TOKEN_WORD ||
			i->next == NULL || i->next->type != TOKEN_EQUAL ||
			i->next->next == NULL || (i->next->next->type != TOKEN_WORD && i->next->next->type != TOKEN_STRING) ||
			i->next->next->next == NULL ||i->next->next->next->type != TOKEN_SEMICOLON
		)
		{
			return err (CONFI_ERR_TOKEN_ORDER, "Неверный порядок токенов после параметра: «%s» ", i->content);
		}

		i = i->next->next->next->next;
	}

	return 0;
}