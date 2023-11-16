# System Tool and API Implementations

## memmove()

Copy memory bytes and take care of the overlapping between source and destination.

```c
void memmove(void* dest, void* src, size_t n) {
  if (dest < src) { // no overlapping issue, regular memcpy()
    for (size_t i = 0; i < n; ++i) {
      ((char*)dest)[i] = ((char*)src)[i];
    }
  }
  else if (dest > src) { // may have overlapping issue, copy from end
    for (size_t i = n; i > 0; --i) { // can't use i = n -1 with i >= 0, because size_t>=0 always, infinite loop
      ((char*)dest)[i - 1] = ((char*)src)[i - 1];
    }
  }
}
```

## memcpy()

Copy memory bytes when there is no overlapping between source and destination.

```c
// byte-by-byte copying
void memcpy(void* dest, void* src, size_t n) {
  for (size_t i = 0; i < n; ++i) {
    ((char*)dest)[i] = ((char*)src)[i];
  }
}

// Optimization 1: word-by-word copying (size_t is adapted to 4 bytes on 32-bit machine, 8 bytes on 64-bit)
void memcpy(void* dest, void* src, size_t n) {
  // word copying
  size_t* ldest = (size_t*)dest;
  size_t* lsrc = (size_t*)src;
  while (n > sizeof(size_t)) {
    *ldest++ = *lsrc++;
    n -= sizeof(size_t);
  }

  // byte copying for the rest
  char* cdest = (char*)ldest;
  char* csrc = (char*)lsrc;
  while (n > 0) {
    *cdest++ = *csrc++;
    n--;
  }
}

// Optimization 2: memory aligned word-by-word copying
void memcpy(void* dest, void* src, size_t n) {
  // handle unaligned bytes
  char* cdest = (char*)dest;
  char* csrc = (char*)src;
  while (n > 0 && ((uintptr_t)csrc % sizeof(size_t) != 0) || (uintptr_t)cdest % sizeof(size_t) != 0) {
    *cdest++ = *csrc++;
    n--;
  }

  // handle aligned word-size part
  size_t* ldest = (size_t*)cdest;
  size_t* lsrc = (size_t*)csrc;
  while (n > sizeof(size_t)) {
    *ldest++ = *lsrc++;
    n -= sizeof(size_t);
  }

  // handle aligned byte-size part
  cdest = (char*)ldest;
  csrc = (char*)lsrc;
  while (n > 0) {
    *cdest++ = *csrc++;
    n--;
  }
}
```

### Difference between `memcpy(dest, src, N)` and `strcpy(dest, src)`/`strncpy(dest, src, N)`

`memcpy()` will copy all `N` bytes.

`strcpy()` will copy until reaching `\0` null terminator.

`strncpy()` will copy until reaching `\0` null terminator or reaching `N` bytes.

## strcmp()

+ 0, if two strings are equal
+ positive, if str1 is greater then str2
+ negative, if str1 is less then str2
+ `const unsigned char` casting handles potentially signed `char` implementation

```c
int strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(const unsigned char *)str1 - *(const unsigned char *)str2;
}
```

## malloc_align()

Allocate a memory region and return the pointer which is aligned with input parameter.

```c
void* malloc_align(size_t size, size_t alignment) {
    // alignment must be a power of 2
    if (alignment == 0 || (alignment & (alignment - 1))) {
        return NULL;
    }

    // size: request by user
    // alignment: request address can be any value, we need to find the multiple of alignment
    //            so the maximum multiple of alignment will be "alignment" bytes away from requested
    // sizeof(size_t): store the request address for free_align()
    void* original = (void*)malloc(size + alignment + sizeof(size_t));
    if (original == NULL) return NULL;

    // Calculate the aligned memory address
    char* aligned = ((char*)original + sizeof(size_t));
    aligned += alignment - ((uintptr_t)aligned % alignment); // use these bytes to make up an aligned

    // Store the original pointer just before the aligned address
    ((void**)aligned)[-1] = original;

    return aligned;
}

void free_align(void* aligned) {
    if (aligned == NULL) return;

    // Retrieve the original pointer and free it
    void* original = ((void**)aligned)[-1];
    free(original);
}
```

## grep

Implement `grep mypattern /path/to/myfile`.

```c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_LINE_LENGTH 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Error: wrong input parameters\n");
        return -1;
    }

    char *pattern = argv[1];
    char *filename = argv[2];
    FILE *fp;
    char line[MAX_LINE_LENGTH];
    int line_number = 1;
    int match_found = 0;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: could not open file %s\n", filename);
        return -1;
    }

    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
        if (strstr(line, pattern) != NULL) {
            printf("%s:%d:%s", filename, line_number, line);
            match_found = 1;
        }
        line_number++;
    }

    if (!match_found) {
        printf("No matches found for %s in %s\n", pattern, filename);
    }

    fclose(fp);
    return 0;
}
```

## head

Implement `head numline /path/to/myfile`.

```c
#define MAX_LINE_LENGTH 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Error: wrong input parameters\n");
        return -1;
    }

    // ptr set to the pos next to the numeric value, 10 is the base
    char* ptr;
    unsigned long num_head_lines = strtoul(argv[1], &ptr, 10);
    char *filename = argv[2];
    FILE *fp;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: could not open file %s\n", filename);
        return -1;
    }

    unsigned long line_cnt = 0;
    char line[MAX_LINE_LENGTH];

    // print head lines
    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
        if (line_cnt >= num_head_lines) {
            break;
        }
        else {
            printf("%s", line);
        }
    }

    fclose(fp);
    return 0;
}
```

## tail

Implement `tail numline /path/to/myfile`.

```c
#define MAX_LINE_LENGTH 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Error: wrong input parameters\n");
        return -1;
    }

    // ptr set to the pos next to the numeric value, 10 is the base
    char* ptr;
    unsigned long num_tail_lines = strtoul(argv[1], &ptr, 10);
    char *filename = argv[2];
    FILE *fp;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: could not open file %s\n", filename);
        return -1;
    }

    unsigned long pos = 0;
    unsigned long line_cnt = 0;
    char line[MAX_LINE_LENGTH];

    // go to end of file
    if (fseek(fp, 0, SEEK_END)) {
        printf("Error: could not go to end of file\n");
        return -1;
    }
    else {
        pos = ftell(fp); // return current position, since we are at end, so pos is number of chars in the file

        // search for '\n'
        while (pos) {
            if (!fseek(fp, --pos, SEEK_SET)) { // pos chars away from beginning of file
                if (fgetc(fp) == '\n') {
                    if (line_cnt >= num_tail_lines) {
                        break;
                    }
                    line_cnt++;
                }
            }
            else {
                printf("Error: could not go to %lu position away from the beginning of file\n", pos);
                return -1;
            }
        }

        // print tail lines
        while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
            printf("%s", line);
        }
    }

    fclose(fp);
    return 0;
}
```

# Code Snippet

## Make an allocated region 4-byte aligned

Get the pointer from malloc, and set the pointer to `ptr & ~0x1F` making the 5 least significant bits to 0.

## Swap values of three variables without a helper temporary variable

```c
void mySwap1(int* a, int* b, int* c) {
    *a = *a + *b + *c; // a = a + b + c
    *b = *a - *b - *c; // b = a+b+c - b - c = a
    *c = *a - *b - *c; // c = a+b+c - a - c = b
    *a = *a - *b - *c; // a = a+b+c - a - b = c
}

void mySwap2(int* a, int* b, int* c) {
    *a = *a ^ *b ^ *c; // a = a ^ b ^ c
    *b = *a ^ *b ^ *c; // b = a^b^c ^ b ^ c = a
    *c = *a ^ *b ^ *c; // c = a^b^c ^ a ^ c = b
    *a = *a ^ *b ^ *c; // a = a^b^c ^ a ^ b = c
}
```

## Define a macro

### Determine a number is +/-/0

```c
#define MY_SIGN(x) (((x) > 0) ? "positive" : (((x) < 0) ? "negative" : "0"))
```

## An addition function for different type of data

```c++
template <typename T>
T myAdd(T a, T b) {
    return a + b;
}
```

## Multiple approaches of calculating the average of two integers

```c
// may overflow when adding
int average1(int a, int b) {
    return (a + b) / 2;
}

// avoid overflow by casting
long long average2(int a, int b) {
    return ((long long)a + b) / 2;
}

// avoid overflow using bitwise operation
int average3(int a, int b) {
    int halfA = a >> 1;
    int halfB = b >> 1;
    int avg = halfA + halfB;
    // if both are odd, plus one
    if ((a & 1) && (b & 1)) {
        avg += 1;
    }
    return avg;
}

// avoid overflow using bitwise operation
int average4(int a, int b) {
    return (a & b) + ((a ^ b) >> 1); // & gets the sum, ^
}
```

## Print integer numbers from 100 to 500

But printed number can't have the repeated digit, e.g. number 131 have duplicated 1s so should not print

```c
bool isValid(int num) {
    int digits[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    while (num > 0) {
        int rem = num % 10;
        if (digits[rem] > 0) {
            return false;
        }
        else {
            digits[rem] = 1;
        }
        num /= 10;
    }
    return true;
}

void myPrint(int low, int high) {
    for (int i = low; i <= high; ++i) {
        if (isValid(i)) {
            printf("%d\n", i);
        }
    }
    return;
}
```