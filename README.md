# Хеш-таблица
Реализация хеш-таблицы методом цепочек для решения проблемы коализий.
## Исследование различных хеш-функций
### HashOne
Хеш-функция, которая возвращает 1 для любой строки
![](https://github.com/vihlancevk/hash-table/blob/main/res/hashTableImages/(1)HashOne.png)
### HashFirstSymbol
Хеш-функция, которая возвращает ASCII код первого символа строки
![](https://github.com/vihlancevk/hash-table/blob/main/res/hashTableImages/(2)HashFirstSymbol.png)
### HashWordLen
Хеш-функция, которая возвращает длину строки
![](https://github.com/vihlancevk/hash-table/blob/main/res/hashTableImages/(3)HashWordLen.png)
### HashSumSymbol
Хеш-функция, которая возвращает сумму ASCII кодом символов строки
![](https://github.com/vihlancevk/hash-table/blob/main/res/hashTableImages/(4)HashSumSymbol.png)
### HashDed
Хеш-функция, которая работает по следующему алгоритму
```
h0   = str[0]
hi+1 = (hi rol 1) ^ str[i+1]
```
![](https://github.com/vihlancevk/hash-table/blob/main/res/hashTableImages/(5)HashDed.png)
### HashRot13
Хеш-функция, которая работает по следующему алгоритму
```
hash  = 0
hash += str[i]
hash -= (hash << 13) | (hash >> 19)
```
![](https://github.com/vihlancevk/hash-table/blob/main/res/hashTableImages/(6)HashRot13.png)
#### Итог
Наиболее равномерное распределение нам даёт функция HashRot13, её мы и будем использовать для второй части ислледования
## Ускорение хеш-таблицы
Для замера времени действия каждой функции будем использовать профайлер callgrind и его графическую оболочку kcachegrind
### Версия без оптимизаций
Количество тактов, которые приходятся на программу - 46 461 694
![](https://github.com/vihlancevk/hash-table/blob/main/optimizations/no_optimizations.png)
По данной информации мы можем выбрать функции, которые будем оптимизировать
### Версия с ассемблерными вставками
Первый тип нашей оптимизации - ассемблерные встаки, его мы применим для функций splitToLines и HashTableVerificator  
Количество тактов, которые приходятся на программу - 44 501 569
![](https://github.com/vihlancevk/hash-table/blob/main/optimizations/asm_insert_optimization.png)
### Версия с ассемблерными вставками и функций на ассемблере
Второй тип нашей оптимизации - переписывание функции на ассемблере, его мы применим на функции HashRot13  
Количество тактов, которые приходятся на программу - 44 029 310
![](https://github.com/vihlancevk/hash-table/blob/main/optimizations/asm_fun_optimization.png)
### Версия с измененной логикой программы
Третим типом оптимизации должно было стать применение интринсиков внутри програмы, которые требуют выравненных данных, чего нет в моей версии хеш-таблицы, однако я пытался найти им применение, но наткнулся на то, что в моей программе 4 раза высчитывается длина каждого слова, 3 раза с помощью функции strlen и один раз, косвенно, в ассемблерной вставке. Поэтому я переделал немного структуру программы, чтобы оставить только косвенный персчет внутри ассемблерной вставки и передавть в функции структуры, которые содержат в себе указатель на константую строку и длину этой стркоки  
Количество тактов, которые приходятся на программу - 40 813 101
![](https://github.com/vihlancevk/hash-table/blob/main/optimizations/no_strlen_optimization.png)
И это оказалось самым быстрым ускорением!
#### Итог
Лучше всего грамотно подходить к созданию архитектуры своей программы, что лучше всего повысит её производительность  
Итоговое ускорение составило примерно 13%
