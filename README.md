# Хеш-таблица
Реализация хеш-таблицы методом цепочек для решения проблемы коализий
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
```shale
hash_0 = str[0]
hash_(i+1) = (hash_i rol 1) ^ str[i+1]
```
![](https://github.com/vihlancevk/hash-table/blob/main/res/hashTableImages/(5)HashDed.png)
### HashRot13
Хеш-функция, которая работает по следующему алгоритму
```shale
hash = 0
hash += str[i]
hash -= (hash << 13) | (hash >> 19)
```
![](https://github.com/vihlancevk/hash-table/blob/main/res/hashTableImages/(6)HashRot13.png)
#### Итог
Наиболее равномерное распределение нам даёт функция HashRot13, её мы и будем использовать для второй части ислледования
## Ускорение хеш-таблицы
