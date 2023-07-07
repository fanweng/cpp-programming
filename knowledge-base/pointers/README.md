# Pointers



## I. Pointers Basics

A pointer stores the value of an address for a variable or a function. It is very useful:
1. access variables not in the current function scope
2. operate arrays efficiently
3. dynamical memory management
4. implement the polymorphism in object oriented programming
5. access specific addresses in the memory for embedded systems

### Dereferencing notation

```c++
int arr[] {1, 2, 3, 4};
int *arr_ptr {arr};
/* Subscript Notation */
std::cout << arr[1] << arr_ptr[1] << std::endl;
/* Offset Notation */
std::cout << *(arr + 1) << *(arr_ptr + 1) << std::endl;
```

### `const` and pointer

- Pointers to constants
    * data pointed by the pointer **cannot** be changed
    * pointer itself can change to point somewhere else
- Constant pointers
    * data pointed by the pointer can be changed
    * pointer itself **cannot** change to point somewhere else
- Constant pointers to constants
    * data pointed by the pointer **cannot** be changed
    * pointer itself **cannot** change to point somewhere else

```c++
int a {100};
int b {10};

const int* ptr1 {&a};   // pointer to constant
*ptr1 = 2;  // Error
ptr1 = &b;  // Ok

int* const ptr2 {&a};   // constant pointer
*ptr2 = 2;  // Ok
ptr2 = &b;  // Error

const int* const ptr3 {&a}; // constant pointer to constant
*ptr3 = 2;  // Error
ptr3 = &b;  // Error
```

### Pointers in functions

- Return a pointer from a function
    * *SHOULD* return pointers to
        + dynamically allocated memory in the function (remember to deallocate it)
        + data passed in the function as a parameter
    * *SHOULDN'T* return pointers to
        + local function variable (out of scope once returning from the function)

### Pointer pitfalls

- Uninitialized pointers
- Dangling pointers
    * pointing to released memory
    * pointing to invalid memory address
- Not checking if `new` fails
    * if `new` fails, it will throw an exception
- Leaking memory
    * forget to release allocated memory

### Reference `&`

- Reference is implemented as a *constant pointer* that is *automatically dereferenced*
    * an alias for a variable
    * must be initialized to a variable when declared (cannot be `nullptr`)
    * once initialized, cannot be made to refer to a different variable
    * very useful as function parameters

```c++
vector<int> v {1, 2, 3};
for (auto num : v) {
    num = 0;    // num is a copy, it doesn't modify the vector element
}
for (auto &num : v) {
    num = 0;    // num is a reference, it changes the vector
}
for (auto const &num : v) {
    num = 0;    // num is a constant reference, compiler error
}
```

### L-values and R-values

- L-value
    * values that have names and are addressable
    * modifiable if they are not constants
    * *reference* must refer to L-value, not R-value

```c++
int x {10};             // x is L-value
string name {"Mike"};   // name is L-value
```

- R-value
    * non-addressable and non-assignable
    * a value that is not an L-value
        + on the right-hand side of an assignment expression
        + a literal
        + a temporary which is intended to be non-modifiable
    * can be assigned to L-value explicitly

```c++
int y = x + 10;         // x + 10 is R-value
string name {"Mike"};   // "Mike" is R-value
int maxNum = max(0,1);  // max(0,1) is R-value
```



## II. Raw pointers vs. Smart pointers

- Problems with **raw pointers**:
    * absolute flexibility with memory management: allocation/deallocation/lifetime
    * potential serious issues: uninitialized pointers, memory leaks, dangling pointers, not exception safe
    * ownership question: who owns the pointer? when to delete?

- [**Smart pointers**](smart-pointer.md):
    * is an object defined by class templates
        + wrapped around a raw pointer
        + overloaded operators: `*`, `->`. But arithmetic operators not supported: `++`, `--`, etc.
    * can only point to *heap-allocated* memory
    * automatically call deletion when out of the scope, can have custom deleters
    * adhere to RAII principles
    * `unique_ptr`, `shared_ptr`, `weak_ptr`, `auto_ptr` (deprecated). Defined in the `<memory>` header in the C++ Standard Library.



## III. Custom deleters

For some special use-cases, we need more than to just destroy the object on the heap when destroying a smart pointer.

```c++
/* Function deleter */
void my_deleter(Any_Class *raw_ptr) {
    // your custom deleter code here
    delete raw_ptr;
}
shared_ptr<Any_Class> ptr { new Any_Class{}, my_deleter };

/* Or, Lambda deleter */
shared_ptr<Any_Class> ptr ( new Any_Class{}, [] (Any_Class *ptr) {
    // your custom deleter code here
    delete ptr;
});
```
