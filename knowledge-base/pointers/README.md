# Pointers



## I. Pointers Basics

A pointer stores the value of an address for a variable or a function. It is very useful:
1. access variables not in the current function scope
2. operate arrays efficiently
3. dynamical memory management
4. implement the polymorphism in object oriented programming
5. access specific addresses in the memory for embedded systems

#### Dereferencing notation

```c++
int arr[] {1, 2, 3, 4};
int *arr_ptr {arr};
/* Subscript Notation */
std::cout << arr[1] << arr_ptr[1] << std::endl;
/* Offset Notation */
std::cout << *(arr + 1) << *(arr_ptr + 1) << std::endl;
```

#### `const` and pointer

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

#### Pointers in functions

- Return a pointer from a function
    * *SHOULD* return pointers to
        + dynamically allocated memory in the function (remember to deallocate it)
        + data passed in the function as a parameter
    * *SHOULDN'T* return pointers to
        + local function variable (out of scope once returning from the function)

#### Pointer pitfalls

- Uninitialized pointers
- Dangling pointers
    * pointing to released memory
    * pointing to invalid memory address
- Not checking if `new` fails
    * if `new` fails, it will throw an exception
- Leaking memory
    * forget to release allocated memory

#### Reference

- Reference is implemented as a *constant pointer* that is *automatically dereferenced*
    * an alias for a variable
    * must be initialized to a variable when declared (cannot be null)
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

#### L-values and R-values

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

- **Smart pointers**:
    * is an object defined by class templates
        + wrapped around a raw pointer
        + overloaded operators: `*`, `->`. But arithmetic operators not supported: `++`, `--`, etc.
    * can only point to *heap-allocated* memory
    * automatically call deletion when out of the scope, can have custom deleters
    * adhere to RAII principles
    * `unique_ptr`, `shared_ptr`, `weak_ptr`, `auto_ptr` (deprecated). Defined in the `<memory>` header in the C++ Standard Library.

#### RAII (Resource Acquisition Is Initialization)

A common idiom used in software design based on the container object lifetime. RAII objects are allocated on the stack.

- Resource Acquisition: open a file, allocate memory, acquire a lock, etc.
- Is Initialization: resource is acquired in a constructor.
- Resource Relinquishing: in the destructor (close a file, etc.)

#### `unique_ptr`

A `unique_ptr` cannot be assigned/copied to another `unique_ptr`, passed by value to a function, or used in any C++ Standard Library algorithm that requires copies to be made.

It can only be moved by `std::move()`. This means the ownership of the memory resource is transferred to another `unique_ptr` and the original `unique_ptr` no longer owns it.

```c++
// ptrA -> Song object
auto ptrA = std::make_unique<Song>("May it be", "Enya");
// ptrA
// ptrB -> Song object
auto ptrB = std::move(ptrA);

std::vector<std::unique_ptr<int>> vec;
std::unique_ptr<int> ptr {new int(10)};
vec.push_back(ptr);             // error, push_back actually copies
vec.push_back(std::move(ptr));  // Ok

std::unique_ptr<int, MyIntDeleter> ptrC(new int(10), myIntDeleter()); // custom deleter
```

Common methods: `get()`, `get_deleter()`, `release()`, `reset()`, `swap()`.

- `unique_ptr::get`, `unique_ptr::release`, `unique_ptr::reset`
    * `get()` returns the *stored pointer* which points to the object managed by the `unique_ptr`. The `unique_ptr` still has the ownership of the pointer, i.e. responsible for deleting the managed data at some point.
    * `release()` releases the ownership of its *stored pointer* by returning its value and replacing it with `nullptr`. It doesn't destroy the managed object but `unique_ptr` is free of the responsibility of deleting the object.
    * `reset()` destroys the managed object.

```c++
                                         // foo   bar    p
                                         // ---   ---   ---
std::unique_ptr<int> foo;                // null
std::unique_ptr<int> bar;                // null  null
int* p = nullptr;                        // null  null  null

foo = std::unique_ptr<int>(new int(10)); // (10)  null  null
bar = std::move(foo);                    // null  (10)  null
p = bar.get();                           // null  (10)  (10)
*p = 20;                                 // null  (20)  (20)
p = nullptr;                             // null  (20)  null
foo = std::unique_ptr<int>(new int(30)); // (30)  (20)  null
p = foo.release();                       // null  (20)  (30)
*p = 40;                                 // null  (20)  (40)
```

#### `shared_ptr`

A `shared_ptr` shares the **ownership** of a resource.

A `shared_ptr` can be copied, passed by value in function arguments, and assigned to other `shared_ptr` instances. All instances point to the **same object**, and point to the **same control block** which keeps the reference count. When reference count reaches 0, the control block deletes the memory resource and itself.

Common methods: `get()`, `get_deleter()`, `unique()`, `reset()`, `use_count()`.

```c++
auto ptrA = std::make_shared<Song>("May it be", "Enya");
auto ptr1 = ptrA;
ptr1.use_count();   // get reference count: 2
ptrA.reset();       // decrement the reference count, nullify the ptrA
ptr1.use_count();   // get reference count: 1

std::shared_ptr<Song> ptrB(new Song("Lady Gaga", "Just Dance"));
auto ptr2 {ptrB};

std::shared_ptr<Song> ptrC(nullptr);
ptrC = std::make_shared<Song>("Elton John", "I'm Still Standing");
```

#### `weak_ptr`

A `weak_ptr` holds a weak reference to an object that is owned by a `shared_ptr` without increasing the reference count. It must be converted to `shared_ptr` in order to access the referenced object. A `weak_ptr` will only return a *non-null* `shared_ptr` when there is a `shared_ptr` somewhere keeping the object alive.

Common methods: `expired()` checks if the resource is deleted, `lock()` creates a `shared_ptr` on the resource, `swap()`, `reset()`, `use_count()`.

`weak_ptr` helps to break the *circular references (cyclic dependency)* between `shared_ptr` instances. Take this `C->A<->B` as an example: C points to the circular reference entity made by A and B.

- If all pointers are `shared_ptr`: when going out of the scope, only C will be released properly. After C is deleted, A cannot be released because the reference count for A is non-zero (pointed by B). Thus, B cannot be released, too.

- If B points to A as a `weak_ptr`, others are `shared_ptr`: when going out of the scope, all pointers will be cleaned up. Since B points to A *weakly*, the reference count for A reaches zero after C is release. Then, A can be cleaned up and reference count for B decreases to zero. Thus, B is cleaned up, too.

```c++
class B;    // forward declaration
class A {
    std::shared_ptr<B> b_ptr;
public:
    void set_B(std::shared_ptr<B> &b) { b_ptr = b; }
    A();
    ~A();
};
class B {
    std::shared_ptr<A> a_ptr; // make this a weak_ptr to prevent the circular reference issue
    // std::weak_ptr<A> a_ptr;
public:
    void set_A(std::shared_ptr<A> &a) { a_ptr = a; }
    B();
    ~B();
};

int main(void) {
    std::shared_ptr<A> a = std::make_shared<A>();
    std::shared_ptr<B> b = std::make_shared<B>();
    a->set_B(b);
    b->set_A(a);
    return 0;
} // destructor of A and B won't be called because of circular reference
```



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



## IV. Passing smarter pointers

#### Rule 32 - Passing `unique_ptr<Widget>`

Take a `unique_ptr<Widget>` parameter to express that function assumes ownership of a Widget.

```c++
void sink(std::unique_ptr<Widget> uniqPtr) { /* do something */ }
auto uniqPtr = std::make_unique<Widget>(0);
sink(std::move(uniqPtr));
// sink(uniqPtr); // error, cannot copy
```

#### Rule 33 - Passing `unique_ptr<Widget>&`

Take a `unique_ptr<Widget>&` parameter to express that function reseats the Widget.

```c++
void reseat(std::unique_ptr<Widget>& uniqPtr) {
    uniqPtr.reset(new Widget(1));
    /* do something */
}
auto uniqPtr = std::make_unique<Widget>(0);
reseat(uniqPtr);
// reseat(std::move(uniqPtr)); // error
```

#### Rule 34 - Passing `shared_ptr<Widget>`

Take a `shared_ptr<Widget>` parameter to express that function is part owner of the Widget.

```c++
void share(std::shared_ptr<Widget> shaWid) {
    /* reference counter ++ at start of function body */
    /* reference counter -- at the exiting of function*/
    /* Widget object will stay alive */
}
```

#### Rule 35 - Passing `shared_ptr<Widget>&`

Take a `shared_ptr<Widget>&` parameter to express that function might reseat the shared pointer.

```c++
void reseat(std::shared_ptr<Widget>& shaWid) {
    /* reference counter doesn't change */
    /* Widget object may or may not stay alive */
    /* function can reseat the resource, more like borrowing the resource with the ability to reseat it */
}
```

#### Rule 36 - Passing `const shared_ptr<Widget>&`

Take a `const shared_ptr<Widget>&` parameter to express that function might retain a reference count to the object.

```c++
void mayShare(const std::shared_ptr<Widget>& shaWid) {
    /* reference counter doesn't change */
    /* function cannot reseat the resource, only borrowing the resource */
    /* better to use a pointer Widget* or a reference Widget& in this case */
}
```

#### Rule 37 - Don't pass a pointer or reference obtained from an aliased smart pointer

> **Aliased smart pointer:** when a pointer or a reference to an object managed by a smart pointer, it may suddenly become dangling if the smart pointer is destroyed or modified.

```c++
void oldFunc(Widget* wid) {
    /* do something with wid */
    /* but no guarantee that the Widget object will stay alive during execution */
}
void shared(std::shared_ptr<Widget>& shaPtr) {
    /* reference count doesn't increase */
    oldFunc(*shaPtr);
}
auto globShared = std::make_shared<Widget>(0);
shared(globShared);

/* Solution 1 */
void shared1(std::shared_ptr<Widget> shaPtr) {
    /* reference count ++ because passed by copy */
    oldFunc(*shaPtr);
}

/* Solution 2 */
void shared2(std::shared_ptr<Widget>& shaPtr) {
    /* reference count doesn't increase because passed by reference */
    auto keepAlive = shaPtr; // but this copy increase the reference count
    oldFunc(*shaPtr);
}
```
