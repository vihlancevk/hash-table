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
Время выполнения программы в условных единицах - 46 599 724.
![](https://github.com/vihlancevk/hash-table/blob/main/optimizations/(1)no_optimizations.png)
По данной информации мы можем выбрать функции, которые будем оптимизировать.
### Версия с ассемблерными вставками
Первый тип нашей оптимизации - ассемблерные вставки, его мы применим для функций `splitToLines` и `HashTableVerificator`.  
Время выполнения программы в условных единицах - 44 640 845.
![](https://github.com/vihlancevk/hash-table/blob/main/optimizations/(2)asm_insert_optimization.png)
### Версия с ассемблерными вставками и функций на ассемблере
Второй тип нашей оптимизации - переписывание функции на ассемблере, его мы применим на функции `HashRot13`.  
Время выполнения программы в условных единицах - 44 167 322.
![](https://github.com/vihlancevk/hash-table/blob/main/optimizations/(3)asm_fun_optimization.png)
### Версия с измененной логикой программы
В хеш-таблице 4 раза вычислялась длина каждого слова, 3 раза с помощью функции `strlen` и один раз, косвенно, в ассемблерной вставке, в функции `splitToLines`. Поэтому, чтобы ускорить программу, необходимо оставить только косвенный персчет внутри ассемблерной вставки и передавать в функции структуры, которые содержат в себе указатель на константую строку и её длину.  
Время выполнения программы в условных единицах - 40 951 673.
![](https://github.com/vihlancevk/hash-table/blob/main/optimizations/(4)no_strlen_optimization.png)
### Версия в увеличенным размером хеш-таблицы
В программе используется метод цепочек для разрешения коализий, поэтому при большом среднем размере списка часто внутри функции `HashTableInsert` вызывается `strcmp`, чтобы избежать этого, необходимо подобрать нужный размер хеш-таблицы, что приведёт к уменьшению средней длины списка.  
Старая средняя длина списка - 12.  
Новая средняя длина списка - 5.  
Время выполнения программы в условных единицах - 38 860 644.
![](https://github.com/vihlancevk/hash-table/blob/main/optimizations/(5)resize_hash_table_optimization.png)
### Ещё одно ускорение хеш-функции
В программе по аналогии с `strlen` несколько раз вызывается функция `HashRot13`, что замедляет программу, поэтому нужно изменить код так, чтобы оставить только один вызов. А также данная функция часто вызывается из `HashTableFind`, поэтому имет смысл переписать её на ассемблере более оптимизировано.  
Время выполнения программы в условных единицах - 36 559 187.
![](https://github.com/vihlancevk/hash-table/blob/main/(6)asm_fun1_optimization.png)
### Ускорение функции `countNumberLines`
В этой функции вызывается `tolower` для всех подряд символов, что замедляет программу, поэтому это нужно исправить.  
Время выполнения программы в условных единицах - 33 980 330.
![](https://github.com/vihlancevk/hash-table/blob/main/(7)tolower_optimization.png)
#### Итог
Общее ускорение составило примерно `37%`.
# Сборка программы
Для компиляции программы необходимо использовать компилятор `gcc`
