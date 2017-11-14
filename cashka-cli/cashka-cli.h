#pragma once

#include <string>

using std::string;

namespace cashka_cli
{
	/* Ошибка - исключение */
    void err (string message, string type = "default");
}
