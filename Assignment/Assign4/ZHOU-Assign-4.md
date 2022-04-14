# CSCI3150 Assignment Four

*ZHOU, Yixiang*
*SID: 1155157100*

| Flag                              | Detail |
| --------------------------------- | ------ |
| Page size                         | 8 byte |
| Offset                            | 3 bit  |
| the first-level page table index  | 2 bit  |
| the second-level page table index | 3 bit  |
| Virtual address                   | 8 bit  |

## 1 Calculation

1. What is the size of the virtual address space?

   $4 \times 8 \times 8 \ \text{Byte} = 2^8 \ \text{Byte}$

2. How many bits does a virtual address have?

   $8  \ \text{bit}$

3. How many bits should be reserved for the first-level page table index, the second-level page table index and the offset respectively?

   $2, 3, 3$ respectively.

## 2 Address Translation

| Decimal | Binary    |
| ------- | --------- |
| 22      | 0001 0110 |
| 68      | 0100 0100 |
| 150     | 1001 0110 |
| 245     | 1111 0101 |

1. Which virtual addresses are mapped?

   68, 150

2. If a virtual address is mapped, what is its corresponding physical address?

   | VA   | PA        |
   | ---- | --------- |
   | 68   | 0010 1110 |
   | 150  | 0010 0110 |

## 3 Page Mapping Setup

|      | PT@PFN=10 | PT@PFN=11 | PT@PFN=12 |
| ---- | --------- | --------- | --------- |
| 0    |           | 5         |           |
| 1    |           | 6         |           |
| 2    | 0         |           |           |
| 3    | 1         |           |           |
| 4    | 2         |           |           |
| 5    |           |           |           |
| 6    | 3         |           | 7         |
| 7    | 4         |           | 8         |

