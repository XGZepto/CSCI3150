# CSCI3150 Assignment Two

> ZHOU, Yixiang
> SID: 1155157100

## Question One

### (a) What is the biggest size we can have for a file with SFS?

4,202,496 Bytes

### (b) Provide data block numbers to be read.

- **(i)** 20, 45, 51, 71
- **(ii)** 45, 51, 71, 72, 81, 98, 99, 104
- **(iii)** 45, 51, 71, 72, 81, 98, 99, 104, 110, 142
- **(iv)** 3, 20, 45, 51, 71, 72, 81, 98

## Question Two

### (a) Give the contents of data blocks 0, 1, 5, 8 and 10, respectively.

**Data Block 0**

|file_name|inode_number|
|---------|------------|
|.        |0           |
|..       |0           |
|dir12    |12          |
|dir1     |1           |
|file2    |15          |

**Data Block 1**

|file_name|inode_number|
|---------|------------|
|.        |1           |
|..       |0           |
|dir7     |7           |
|dir2     |2           |

**Data Block 5**

|file_name|inode_number|
|---------|------------|
|.        |5           |
|..       |4           |
|file1    |14          |

**Data Block 8**

|file_name|inode_number|
|---------|------------|
|.        |8           |
|..       |11          |
|dir3     |3           |
|dir4     |4           |
|dir9     |9           |

**Data Block 10**

|file_name|inode_number|
|---------|------------|
|.        |10          |
|..       |4           |

### (b) Show the sequence of the inode numbers and data block numbers passed.

| inode | data block |
|-------|------------|
|0      |0           |
|1      |1           |
|2      |2           |
|13     |13          |

The inode number of **file4** would be obtained in data block 13.
