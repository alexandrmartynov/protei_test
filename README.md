# protei_test

В данной работе реализовано два приложения: клиент и сервер.

Сборка проекта осуществляется посредством CMake.

## common

Содержит классы для работы с сетью:
- InternetAddress - класс содержит базовые настройки интернет-адреса;
- Socket - базовый класс для сокетов;
- SocketTcp/ SocketUdp - сокеты TCP и UDP;

## сlient

При запуске клиента необходимо указать протокол передачи данных и номер порта.
```
-t tcp/udp - тип подключения;
-p 8080 - порт сервера;
```
Для отключения необходимо ввести слово "-exit".

## server

Сервер реализован с использованием асинхронных сокетов (epoll).
Это позволило работать с клиентами по двум протоколам(TCP и UDP) одновременно.
Сервер работает в режиме эхо.
После получения сообщения от клиента производится обработка согласно ТЗ.
