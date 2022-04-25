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
    
    unsigned int hash = 0;
    char *buffer      = (char*)elem;
    
    for(size_t i = 0; i < size; i++)
    {
        hash += (unsigned int)buffer[i];
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
Также мы будем проверять эффективность оптимизации с ключом компиляции `-O2`.
### Версия без оптимизаций
На примере функции `countNumberLines` на изображении ниже показано, как выглядит профайлер: левую часть занимает список функций, которые мы отсортировали по параметру `Self`, в правой верхней части мы можем видеть функции, которые вызывают выбранную нами, а в правой нижней - функции, которые вызываются из нашей.
![](https://github.com/vihlancevk/hash-table/blob/main/optimizations/(1)no_optimizations.png)
На основе этого мы будем выбирать программы для оптимизации.  
Время выполнения программы в условных единицах - `25 689 218`.  
Измерения проводились с ключом компиляции `-O2`.
### Ускорение функции strchr
Первая функция, которую нужно ускорить согласно нашему методу поиска объектов для оптимизации, - `strchr`. Мы можеи переписать эту функции так, чтобы она работала быстрее чем стандартная, потому что мы заранее знаем строку, в которой будет вестись поиск нужного нам сивмола.
![](https://github.com/vihlancevk/hash-table/blob/main/optimizations/(2)1strchr_optimization.png)
Было
```c
strchr( const char *str, char elem );
```
Стало
```c
#include <immintrin.h>

const char  *SEPARATION_SYMBOLS = " .,!?;:-'`©()<>{}[]/|&*#%$~_\"\n\t";

static int myStrchr( char elem )
{
    __m256i curSym   = _mm256_set1_epi8  ( elem );
    __m256i symbols  = _mm256_lddqu_si256( (__m256i*)SEPARATION_SYMBOLS );
    
    __m256i cmp = _mm256_cmpeq_epi8( symbols, curSym );

    return _mm256_movemask_epi8( cmp );
}
```
![](https://github.com/vihlancevk/hash-table/blob/main/optimizations/(2)strchr_optimization.png)
Функция `myStrchr` так ускорилась, что профилировщик перестал отображать её, как вызываемую в `countNumberLines`.  
1) Время выполнения программы в условных единицах - `18 437 492`;
2) Программа ускорилась примерно в `1.39` раза относительно предыдущей версии;
3) Программа ускорилась примерно в `1.39` раза относительно первоначальной версии.  
  
Измерения проводились с ключом компиляции `-O2`.
### Ускорение функции strcmp
После первой оптимизации больше всего для оптимизации подходит  функция `strcmp` ( выбираем функцию, которая находится сверху списка и не имеет детей ).
![](https://github.com/vihlancevk/hash-table/blob/main/optimizations/(3)1strcmp_optimization.png)
В программе используется метод цепочек для разрешения коллизий, поэтому при большом среднем размере списка внутри функции `HashTableInsert` часто вызывается `strcmp`, чтобы избежать этого, необходимо подобрать нужный размер хеш-таблицы, что приведёт к уменьшению средней длины списка.  
Старая средняя длина списка - 5.2 .  
Новая средняя длина списка  - 1.3 .
![](https://github.com/vihlancevk/hash-table/blob/main/optimizations/(3)strcmp_optimization.png)
1) Время выполнения программы в условных единицах - `17 222 385`;
2) Программа ускорилась примерно в `1.07` раза относительно предыдущей версии;
3) Программа ускорилась примерно в `1.49` раза относительно первоначальной версии.  
  
Измерения проводились с ключом компиляции `-O2`.
### Ускорение функции HashRot13
Далее нам нужно оптимизировать функцию `HashRot13` ( выбираем функцию, которая находится сверху списка и не имеет детей ).
![](https://github.com/vihlancevk/hash-table/blob/main/optimizations/(4)1HashRot13_optimization.png)
Было
```c
unsigned int HashRot13( const void *elem, size_t size )
{
    assert( elem != nullptr );
    
    unsigned int hash = 0;
    char *buffer      = (char*)elem;
    
    for(size_t i = 0; i < size; i++)
    {
        hash += (unsigned int)buffer[i];
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
        test    rsi, rsi
        je      .return
        
        xor     ecx, ecx ; ecx = i    = 0
        xor     eax, eax ; eax = hash = 0
        
.for:
        movsx   edx, byte [rdi + rcx] ; hash += (int)buffer[i]
        add     eax, edx              ;
        
        mov     edx, eax ; hash -= ( hash << 13 ) | ( hash >> 19 )
        rol     edx, 13  ;
        sub     eax, edx ;
        
        add     rcx, 1   ; for
        cmp     rsi, rcx ;
        jne     .for     ;
        
        mov     ecx, eax            ; hash = hash % 2000
        imul    rcx, rcx, 274877907 ;
        shr     rcx, 39             ;
        imul    ecx, ecx, 2000      ;
        sub     eax, ecx            ;
        
        ret
        
.return:
        xor     eax, eax
        ret
```
![](https://github.com/vihlancevk/hash-table/blob/main/optimizations/(4)strcmp_optimization.png)
1) Время выполнения программы в условных единицах - `16 540 686`;
2) Программа ускорилась примерно в `1.04` раза относительно предыдущей версии;
3) Программа ускорилась примерно в `1.55` раза относительно первоначальной версии.  
  
Измерения проводились с ключом компиляции `-O2`.
### Ускорение функции strlen
Дальше мы будем ускорять функцию `strlen` ( выбираем функцию, которая находится сверху списка и не имеет детей ). Для этого перепишем функцию `findNextLine` с помощью ассемблерной вставки, чтобы избежать вызова `strlen`.
![](https://github.com/vihlancevk/hash-table/blob/main/optimizations/(5)1strlen_optimization.png)
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

void splitToLines( Line *lines, int linesCount, char *str )
{
    assert( lines != nullptr );
    assert( linesCount > 0 );
    assert( str   != nullptr );

    char *ptrStr = str;
    ptrStr = (char*)findNextLine(ptrStr);

    for ( int i = 0; i < linesCount; i++ )
    {
        lines[i].str     = ptrStr;
        lines[i].sizeStr = (int)strlen( lines[i].str );
        ptrStr           = (char*)findNextLine( ptrStr );
    }
}
```
Стало
```c
void splitToLines( Line *lines, int linesCount, char *str )
{
    assert( lines != nullptr );
    assert( linesCount > 0 );
    assert( str != nullptr );

    char *ptrStr = str;

    for ( size_t i = 0; i < linesCount; i++ )
    {
        lines[i].str = ptrStr;
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
        lines[i].sizeStr = (int)( ptrStr - 1 - lines[i].str );
    }
}
```
![](https://github.com/vihlancevk/hash-table/blob/main/optimizations/(5)strlen_optimization.png)
1) Время выполнения программы в условных единицах - `15 243 681`;
2) Программа ускорилась примерно в `1.08` раза относительно предыдущей версии;
3) Программа ускорилась примерно в `1.68` раза относительно первоначальной версии.  
  
Измерения проводились с ключом компиляции `-O2`.
## Итог
Суммарно программа ускорилась в `1.68` раза относительно первоначальной версии.  
Измерения проводились с ключом компиляции `-O2`.
# Сборка программы
Для компиляции программы необходимо использовать компилятор `gcc` и ключи компиляции `-O2`, `-mavx2`
