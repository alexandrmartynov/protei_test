# protei_test

В данной работе было реализовано два приложения: клиент и сервер.
Сборка проекта осуществляется посредством CMake.

## common

Содержит классы для работы с сокетами:
- InternetAddress - класс содержит базовые настройки интернет-адреса;
- Socket - базовый класс для сокетов;
- SocketTcp/ SocketUdp - сокеты TCP и UDP;

## сlient

При запуске клиента необходимо указать протокол передачи данных и порт.
```
-t tcp/udp - тип подключения;
-p 8080 - порт сервера;
```
Для того, чтобы отключиться, необходимо ввести кодовое слово "-exit".

## server

Сервер реализован с использованием асинхронных сокетов (epoll).
Это позволило работать с клиентом по двум протоколам(TCP и UDP) одновременно.
Сервер работает в режиме эхо.
После получения сообщения от клиента производится обработка согласно ТЗ.
