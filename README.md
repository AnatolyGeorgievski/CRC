# CRC-64 Cyclic redundancy check

* _Анатолий М. Георгиевский_ , 2020-2026 (https://github.com/AnatolyGeorgievski)

>Циклический избыточный код (англ. Cyclic redundancy check, CRC) — алгоритм нахождения контрольной суммы, предназначенный для проверки целостности данных.

RU: 
**Песочница по разработке алгоритмов CRC**

За основу взята идея использования умножения без переносов CLMUL в системе команд Intel x86. 
\see Intel/ Fast CRC Computation Using PCLMULQDQ Instruction.
Аналогичные алгоритмы могут быть получены для системы команд ARM Neon (Aarch64) на инструкциях полиномиального умножения. 

От оригинальной работы осталось только операция single_FOLD. Весе остальное существенно переработано и предложен собственный алгоритм редуцирования. Выведен алгоритм для отраженного порядка бит. Всего получено четыре алгоритма, для CRC с разрядностью меньше 64 бит и для CRC-64 бита. Представлены алгоритмы для генерации таблиц и сдвиговых констант с прямым и отраженным порядком. 


* Реализованы алгоритмы с прямым и отраженным порядком бит CRC-8/CRC16/CRC-24/CRC32/CRC64. 
* Также представлены алгоритмы редуцирования в конечном поле Галуа для операции GFMUL, для полиномов 64 и 128 бит с прямым и отраженным порядком бит.
* Статья, в которой я попытался объяснить, как получены эти алгоритмы
(https://project-george.blogspot.com/2019/12/crc.html)

2026: Добавлены варианты, таблиц и реализована поддержка AArch64
* CRC-64/ECMA-182
* CRC-64/NVME
* CRC-64/XZ (используется в ZFS для хеш-таблиц)
* CRC-32, CRC-32B, CRC-32/ISCSI CRC-32C, CRC-32K
* CRC-24/OpenPGP

Список работ
* [1] Intel®: Fast CRC Computation for Generic Polynomials Using PCLMULQDQ Instruction
* [2] Intel®: Intel® Carry-Less Multiplication Instruction and its Usage for Computing the GCM Mode
* [3] Speeding up CRC32C computations with Intel CRC32 instruction
Gueron, S., 28 Feb 2012, In: Information Processing Letters. 112, 5, p. 179-185 7 p.
* [4] Efficient implementation of the Galois Counter Mode using a carry-less multiplier and a fast reduction algorithm
Gueron, S. & Kounavis, M., 1 Jul 2010