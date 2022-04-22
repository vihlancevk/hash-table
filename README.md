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
```c
char *buffer      = (char*)elem;
unsigned int hash = buffer[0];
for ( size_t i = 0; i < size - 1; i++ )
{
    hash = ( hash << 1 | hash >> 31 ) ^ buffer[i + 1];
}
```
![](https://github.com/vihlancevk/hash-table/blob/main/res/hashTableImages/(5)HashDed.png)
### HashRot13
Хеш-функция, которая работает по следующему алгоритму
```c
int hash     = 0;
char *buffer = (char*)elem;
for(size_t i = 0; i < size; i++)
{
    hash += (int)buffer[i];
    hash -= (hash << 13) | (hash >> 19);
}
```
![](https://github.com/vihlancevk/hash-table/blob/main/res/hashTableImages/(6)HashRot13.png)
#### Итог
Наиболее равномерное распределение нам даёт функция HashRot13, её мы и будем использовать для второй части ислледования.
## Ускорение хеш-таблицы
Для замера времени действия каждой функции будем использовать профайлер `callgrind` и его графическую оболочку `kcachegrind`.
### Версия без оптимизаций
Время выполнения программы в условных единицах - 46 461 694.
![](https://github.com/vihlancevk/hash-table/blob/main/optimizations/(1)no_optimizations.png)
По данной информации мы можем выбрать функции, которые будем оптимизировать.
### Версия с ассемблерными вставками
Первый тип нашей оптимизации - ассемблерные встаки, его мы применим для функций `splitToLines` и `HashTableVerificator`.  
Время выполнения программы в условных единицах - 44 501 569.
![](https://github.com/vihlancevk/hash-table/blob/main/optimizations/(2)asm_insert_optimization.png)
### Версия с ассемблерными вставками и функций на ассемблере
Второй тип нашей оптимизации - переписывание функции на ассемблере, его мы применим на функции `HashRot13`.  
Время выполнения программы в условных единицах - 44 029 310.
![](https://github.com/vihlancevk/hash-table/blob/main/optimizations/(3)asm_fun_optimization.png)
### Версия с измененной логикой программы
В хеш-таблице 4 раза вычислялась длина каждого слова, 3 раза с помощью функции `strlen` и один раз, косвенно, в ассемблерной вставке, в функции `splitToLines`. Поэтому, чтобы ускорить программу, необходимо оставить только косвенный персчет внутри ассемблерной вставки и передавть в функции структуры, которые содержат в себе указатель на константую строку и её длину.  
Время выполнения программы в условных единицах - 40 813 101.
![](https://github.com/vihlancevk/hash-table/blob/main/optimizations/(4)no_strlen_optimization.png)
#### Итог
Общее ускорение составило примерно `13%`.
# Сборка программы

