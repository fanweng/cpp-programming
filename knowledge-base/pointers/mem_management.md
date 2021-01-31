# Memory Management

C++ allows to allocate the memory in the runtime dynamically but we need to deallocate it manually after the usage. The Dynamic memory allocation is more efficient, especially for arrays, where a lot of time, we don't know the size of array until the runtime.

* `new` operator

The basic syntax is `ptrVar = new dataType`. The `new` operator returns the address of the allocated memory location (on the heap).

* `delete` operator

The memory deallocation uses `delete` operator, which returns the memory back to the operating system.

```c++
// declare and allocate memory for that pointer
int *ptr = new int;
int *arr_ptr = new int[10];
// assign a value to the allocated memory
*ptr = 10;
// deallocate the memory
delete ptr;
delete [] arr_ptr;
```

## `std::allocator()`

All STL containers uses `std::allocator` to encapsulate the memory management. It separates the **allocation** and do **construction** in two steps, as well as separates **destruction** and **deallocation** in two steps.

```c++
allocator<string> myAllocator;
string *str = myAllocator.allocate(3);  // allocate space for 3 strings
myAllocator.construct(str, "Greeting");
myAllocator.construct(str+1, "From");
myAllocator.construct(str+2, "Earth");
myAllocator.destroy(str);
myAllocator.destroy(str+1);
myAllocator.destroy(str+2);
myAllocator.deallocate(str, 3);         // deallocate space for 3 strings
```