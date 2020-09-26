# Memory Management

C++ allows to allocate the memory in the runtime dynamically but we need to deallocate it manually after the usage. The Dynamic memory allocation is more efficient, especially for arrays, where a lot of time, we don't know the size of array until the runtime.

* `new` operator

The basic syntax is `ptrVar = new dataType`. The `new` operator returns the address of the allocated memory location.

* `delete` operator

The memory deallocation uses `delete` operator, which returns the memory back to the operating system.

```c++
// declare and allocate memory for that pointer
int* ptr = new int;
// assign a value to the allocated memory
*ptr = 10;
// deallocate the memory
delete ptr;
```