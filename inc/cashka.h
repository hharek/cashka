#pragma once

#include <string>

using std::string;

namespace cashka
{
	/* Ошибка - исключение */
	void err (string message, string type = "default");
}