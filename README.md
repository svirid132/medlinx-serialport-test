Отпарвилтель байтов. Он отправляет по порту /dev/tnt1 сообщения. 
Схема сообщений:
- адрес отправителя(1 байт) 1...50
- тип (1 байт) 1...7
- размер данных в байтах(2 байта)
- данные(для сообщения типа 1 - числа в int32, для сообщения типа 2 - текст в utf8, для типа 3..7 - данные отсутсвуют)
- crc16
