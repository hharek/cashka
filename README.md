# cashka

Simple key-value storage

REQUIREMENTS:
- nlohmann-json (https://github.com/nlohmann/json)


-------------------------------------------------------------------------------------

Простое key-value хранилище

Требования:
- nlohmann-json (https://github.com/nlohmann/json)

Примечание: пока обычный tcp-echo сервер :).


### Список доступных команд

$ cashka -h
```
Применение: cashka [ОПЦИИ] КОМАНДА
Опции:
  -h    --help          - показать справку
  -v    --version       - показать версию программы и выйти
  -p    --pid-file      - указать pid-файл
  -c    --config        - указать конфигурационный файл
  -f    --foreground    - запустить процесс на переднем плане (не в фоне)
  -H    --host          - указать хост (по умолчанию: 127.0.0.1)
  -P    --port          - указать порт (по умолчанию: 3000)
  -u    --unix-socket   - указать путь к unix-сокету

Команды:
  start     - запуск сервера 
  stop      - стоп сервера
  restart   - перезагрузить сервер
  status    - статус сервера

Примеры:
  cashka -h
  cashka --version
  cashka -p my.pid -c my.json start
  cashka -H 127.0.0.1 -P 3000
  cashka --unix-socket=cashka.sock
  cashka stop
  cashka restart
  cashka status
```

$ cashka-cli -h
```
Применение: cashka-cli [ОПЦИИ]
Опции:
  -h    --help          - показать справку
  -v    --version       - показать версию программы и выйти
  -c    --config        - указать конфигурационный файл
  -H    --host          - указать хост (по умолчанию: 127.0.0.1)
  -P    --port          - указать порт (по умолчанию: 3000)
  -u    --unix-socket   - указать путь к unix-сокету

Примеры:
  cashka-cli -h
  cashka-cli --version
  cashka-cli -c my.json
  cashka-cli -H 127.0.0.1 -P 3000
  cashka-cli --unix-socket=cashka.sock
```

### Другое
- [CHANGELOG](https://github.com/hharek/cashka/wiki/CHANGELOG)
- [ROADMAP](https://github.com/hharek/cashka/wiki/ROADMAP)
