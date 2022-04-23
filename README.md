# Хеш-таблица
Реализация хеш-таблицы методом цепочек для решения проблемы коллизий.
## Исследование различных хеш-функций
### HashOne
Хеш-функция, которая возвращает 1 для любой строки.
```c
unsigned int HashOne( const void *elem, size_t size )
{
    assert( elem != nullptr );

    unsigned int hash = 1;

    return hash % ( HASH_TABLE_SIZE );
}
```
![](https://github.com/vihlancevk/hash-table/blob/main/res/hashTableImages/(1)HashOne.png)
### HashFirstSymbol
Хеш-функция, которая возвращает ASCII код первого символа строки.
```c
unsigned int HashFirstSymbol( const void *elem, size_t size )
{
    assert( elem != nullptr );

    unsigned int hash = 0;
    char *buffer      = (char*)elem;

    hash = (unsigned int)buffer[0];

    return hash % ( HASH_TABLE_SIZE );
}
```
![](https://github.com/vihlancevk/hash-table/blob/main/res/hashTableImages/(2)HashFirstSymbol.png)
### HashWordLen
Хеш-функция, которая возвращает длину строки.
```c
unsigned int HashWordLen( const void *elem, size_t size )
{
    assert( elem != nullptr );

    unsigned int hash = (unsigned int)size;

    return hash % ( HASH_TABLE_SIZE );
}
```
![](https://github.com/vihlancevk/hash-table/blob/main/res/hashTableImages/(3)HashWordLen.png)
### HashSumSymbol
Хеш-функция, которая возвращает сумму ASCII кодом символов строки.
```c
unsigned int HashSumSymbol( const void *elem, size_t size )
{
    assert( elem != nullptr );

    unsigned int hash = 0;
    char *buffer      = (char*)elem;

    for(size_t i = 0; i < size; i++)
    {
        hash += (unsigned int)buffer[i];
    }

    return hash % ( HASH_TABLE_SIZE );
}
```
![](https://github.com/vihlancevk/hash-table/blob/main/res/hashTableImages/(4)HashSumSymbol.png)
### HashDed
Хеш-функция, которая работает по следующему алгоритму
```c
unsigned int HashDed( const void *elem, size_t size )
{
    assert( elem != nullptr );

    char *buffer      = (char*)elem;
    unsigned int hash = buffer[0];

    for ( size_t i = 0; i < size - 1; i++ )
    {
        hash = ( hash << 1 | hash >> 31 ) ^ buffer[i + 1];
    }

    return hash % ( HASH_TABLE_SIZE );
}
```
![](https://github.com/vihlancevk/hash-table/blob/main/res/hashTableImages/(5)HashDed.png)
### HashRot13
Хеш-функция, которая работает по следующему алгоритму
```c
unsigned int HashRot13( const void *elem, size_t size )
{
    assert( elem != nullptr );
    
    int hash     = 0;
    char *buffer = (char*)elem;
    
    for(size_t i = 0; i < size; i++)
    {
        hash += (int)buffer[i];
        hash -= (hash << 13) | (hash >> 19);
    }
    
    return hash % ( HASH_TABLE_SIZE );
}
```
![](https://github.com/vihlancevk/hash-table/blob/main/res/hashTableImages/(6)HashRot13.png)
## Итог
Наиболее равномерное распределение нам даёт функция HashRot13, её мы и будем использовать для второй части исcледования.
## Ускорение хеш-таблицы
Для замера времени действия каждой функции будем использовать профайлер `callgrind` и его графическую оболочку `kcachegrind`.  
В `kcachegrind` будем сортировать функции по показателю `Self` (относительное время в условных единицах, которое работала сама функция). Необходимо найти по этому показателю функцию из верхушки списка, которая не имеет "детей", и оптимизировать её.  
Также мы будем проверять эффективность оптимизации с ключом компиляции -O0.
### Версия без оптимизаций
На примере функции `countNumberLines` на изображении ниже показано, как выглядит профайлер: левую часть занимает список функций, которые мы отсортировали по параметру `Self`, в правой верхней части мы можем видеть функции, которые вызывают выбранную нами, а в правой нижней - функции, которые вызываются из нашей.
![](https://github.com/vihlancevk/hash-table/blob/main/optimizations/(1)no_optimizations.png)
На основе этого мы будем выбирать программы для оптимизации.  
( Время выполнения программы в условных единицах - 46 599 724 )
### Версия с ассемблерными вставками
Первый тип нашей оптимизации - ассемблерные вставки, его мы применим для функций `findNextLine` и `HashTableVerificator`.  
`findNextLine`  
![](https://github.com/vihlancevk/hash-table/blob/main/optimizations/(2)1asm_insert_optimization.png)
Было
```c
static void *findNextLine(char *str)
{
    assert(str != nullptr);

    char *ptrStr = str;

    while (*ptrStr != '\0' )
    {
        ptrStr++;
    }

    return ptrStr + 1;
}
void splitToLines(Line *lines, int linesCount, char *str)
{
    assert(lines != nullptr);
    assert(linesCount > 0);
    assert(str   != nullptr);

    char *ptrStr = str;
    ptrStr = (char*)findNextLine(ptrStr);

    for (int i = 0; i < linesCount; i++)
    {
        lines[i].str = ptrStr;
        lines[i].sizeStr = (int)strlen(lines[i].str);
        ptrStr = (char*)findNextLine(ptrStr);
    }
}
```
Стало
```c
void splitToLines(Line *lines, int linesCount, char *str)
{
    assert(lines != nullptr);
    assert(linesCount > 0);
    assert(str  != nullptr);

    char *ptrStr = str;

    for (int i = 0; i < linesCount; i++)
    {
        lines[i].str = ptrStr;
        lines[i].sizeStr = (int)strlen(lines[i].str);
        __asm__ ( "next:\n\t"
                  "cmpb $0x0, (%%rsi)\n\t"
                  "je stop\n\t"
                  "incq %%rsi\n\t"
                  "jmp next\n\t"
                  "stop:\n\t"
                  "incq %%rsi\n\t"
                  :"=S"( ptrStr )
                  :"0" ( ptrStr )
                  :"%rax"
                );
    }
}
```
`HashTableVerificator`  
![](https://github.com/vihlancevk/hash-table/blob/main/optimizations/(2)2asm_insert_optimization.png)
Данная функция была оптимизирована аналогично предыдущей.  
![](https://github.com/vihlancevk/hash-table/blob/main/optimizations/(2)asm_insert_optimization.png)
( Время выполнения программы в условных единицах - 44 640 845. )
### Версия с ассемблерными вставками и функций на ассемблере
Второй тип нашей оптимизации - переписывание функции на ассемблере, его мы применим на функции `HashRot13`.  
![](https://github.com/vihlancevk/hash-table/blob/main/optimizations/(3)1asm_fun_optimization.png)
Было
```c
unsigned int HashRot13( const void *elem, size_t size )
{
    assert( elem != nullptr );
    
    int hash     = 0;
    char *buffer = (char*)elem;
    
    for(size_t i = 0; i < size; i++)
    {
        hash += (int)buffer[i];
        hash -= (hash << 13) | (hash >> 19);
    }
    
    return hash % ( HASH_TABLE_SIZE );
}
```
Стало
```asm
section .text

global HashRot13

; rdi - const void *elem
; rsi - size_t size

HashRot13:

mov eax, 0      ; int hash = 0
mov rbx, 0      ; size_t i = 0

.for:
    xor edx, edx

    cmp rbx, rsi
    jae .return

    mov dl, byte [rdi + rbx] ; int dl = (int)(char*)elem[i]
    add eax, edx             ; hash  += dl
    
    mov ecx, eax        ; (hash << 13)
    shl ecx, 13         ;
    
    mov edx, eax        ; (hash << 19)
    shr edx, 19         ;
    
    or ecx, edx         ; (hash << 13) | (hash << 19)
    
    sub eax, ecx        ; hash -= (hash << 13) | (hash << 19)
    
    inc rbx
    jmp .for

.return:
    xor edx, edx
    mov ecx, 700
    div ecx
    
    mov eax, edx
    ret
```
![](https://github.com/vihlancevk/hash-table/blob/main/optimizations/(3)asm_fun_optimization.png)
( Время выполнения программы в условных единицах - 44 167 322. )
### Версия с измененной логикой программы
В хеш-таблице 4 раза вычислялась длина каждого слова, 3 раза с помощью функции `strlen` и один раз, косвенно, в ассемблерной вставке, в функции `splitToLines`. Поэтому, чтобы ускорить программу, необходимо оставить только косвенный пересчёт внутри ассемблерной вставки и передавать в функции структуры, которые содержат в себе указатель на константую строку и её длину.  
Время выполнения программы в условных единицах - 40 951 673.
![](https://github.com/vihlancevk/hash-table/blob/main/optimizations/(4)no_strlen_optimization.png)
### Версия в увеличенным размером хеш-таблицы
В программе используется метод цепочек для разрешения коллизий, поэтому при большом среднем размере списка часто внутри функции `HashTableInsert` вызывается `strcmp`, чтобы избежать этого, необходимо подобрать нужный размер хеш-таблицы, что приведёт к уменьшению средней длины списка.  
Старая средняя длина списка - 12.  
Новая средняя длина списка - 5.  
Время выполнения программы в условных единицах - 38 860 644.
![](https://github.com/vihlancevk/hash-table/blob/main/optimizations/(5)resize_hash_table_optimization.png)
### Ещё одно ускорение хеш-функции
В программе по аналогии с `strlen` несколько раз вызывается функция `HashRot13`, что замедляет программу, поэтому нужно изменить код так, чтобы оставить только один вызов. А также данная функция часто вызывается из `HashTableFind`, поэтому имеет смысл переписать её на ассемблере более оптимизировано.  
Время выполнения программы в условных единицах - 36 559 187.
![](https://github.com/vihlancevk/hash-table/blob/main/optimizations/(6)asm_fun1_optimization.png)
### Ускорение функции `countNumberLines`
В этой функции вызывается `tolower` для всех подряд символов, что замедляет программу, поэтому это нужно исправить.  
Время выполнения программы в условных единицах - 33 980 330.
![](https://github.com/vihlancevk/hash-table/blob/main/optimizations/(7)tolower_optimization.png)
## Итог
Общее ускорение составило примерно `37%`.
# Сборка программы
Для компиляции программы необходимо использовать компилятор `gcc`
