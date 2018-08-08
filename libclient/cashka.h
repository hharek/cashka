/**
 * Библиотека для работы клиента cashka
 */

/**
 * Соединение
 */
void connect (const char * host, const unsigned int port);

/**
 * Соединение по unix-сокету
 */
void connect_unix (const char * unix_socket);