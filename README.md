
Сообщение:
```
{
    "author": "Olga Zudina",
    "date": "2019-10-16T19:32:10+03:00",
    "id": "0d300e89-d833-473e-8a42-1b99eb4d6e24",
    "message": "MarshalText implements the encoding.TextMarshaler interface. The time is formatted in RFC 3339 format, with sub-second precision added if present.",
    "type": "new_message"
}
```

Запрос на поиск:
```
{
    "author": "Olga Zudina",
    "date": "2019-10-16T03:00:00+03:00",
    "message": "already",
    "type": "search_query"
}
```

Задача:
1. вычитать набор сообщений с сервера,
	распарсить автора, дату, соощение и идентификатор сообщения
2. Получив запрос на поиск сообщений, произвести
	поиск по сохраненным данным
3. Вернуть серверу сообщение, которое содержит идентификаторы
	найденный сообещний через пробел
