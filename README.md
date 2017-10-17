# cashka

Simple key-value storage

REQUIREMENTS:
- nlohmann-json (https://github.com/nlohmann/json)


-------------------------------------------------------------------------------------

Простое key-value хранилище

Требования:
- nlohmann-json (https://github.com/nlohmann/json)

Примечание: пока умеет только запускаться :).


### Список доступных команд

$ cashka -h
```
Опции:
  -h    --help          - показать справку
  -v    --version       - показать версию программы и выйти
  -p    --pid-file      - указать pid-файл
  -c    --config        - указать конфигурационный файл
  -f    --foreground    - запустить процесс на переднем плане (не в фоне)
  -H    --host          - указать хост (по умолчанию: 127.0.0.1)
  -P    --port          - указать порт (по умолчанию: 3000)

Команды:
  start     - запуск сервера 
  stop      - стоп сервера
  restart   - перезагрузить сервер
  status    - статус сервера

Примеры:
  cashka -h
  cashka --version
  cashka -p my.pid -c my.json start
  cashka stop
  cashka restart
  cashka status
```
