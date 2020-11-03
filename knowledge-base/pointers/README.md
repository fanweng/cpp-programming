# Pointers

## Raw pointers vs. Smart pointers

- Problems with **raw pointers**:
    * absolute flexibility with memory management: allocation/deallocation/lifetime
    * potential serious issues: uninitialized pointers, memory leaks, dangling pointers, not exception safe
    * ownership question: who owns the pointer? when to delete?

- **Smart pointers**:
    * is an object defined by class templates
        + wrapped around a raw pointer
        + overloaded operators: `*`, `->`. But arithmetic operators not supported: `++`, `--`, etc.
    * can only point to *heap-allcoated* memory
    * automatically call deletion when out of the scope, can have custom deleters
    * adhere to RAII principles
    * `unique_ptr`, `shared_ptr`, `weak_ptr`, `auto_ptr` (deprecated)

#### RAII (Resource Acquisition Is Initialization)

A common idiom used in software design based on the container object lifetime. RAII objects are allocated on the stack.

- Resource Acquisition: open a file, allocate memory, acquire a lock, etc.
- Is Initialization: resource is acquired in a constructor.
- Resource Relinquishing: in the destructor (close a file, etc.)

## `unique_ptr`

Defined in the `<memory>` header in the C++ Standard Library.

A `unique_ptr` cannot be assigned/copied to another `unique_ptr`, passed by value to a function, or used in any C++ Standard Library algorithm that requires copies to be made.

It can only be moved by `std::move()`. This means the ownership of the memory resource is transferred to another `unique_ptr` and the original `unique_ptr` no longer owns it.

```c++
// ptrA -> Song object
auto ptrA = make_unique<Song>("May it be", "Enya");
// ptrA
// ptrB -> Song object
auto ptrB = std::move(ptrA);

std::vector<std::unique_ptr<int>> vec;
std::unique_ptr<int> ptr {new <int>(10)};
vec.push_back(ptr);             // error, push_back actually copies
vec.push_back(std::move(ptr));  // Ok
```

#### `unique_ptr::get`, `unique_ptr::release`, `unique_ptr::reset`

* `get()` returns the *stored pointer* which points to the object managed by the `unique_ptr`. The `unique_ptr` still has the ownershipe of the pointer, i.e. responsible for deleting the managed data at some point.
* `release()` releases the ownership of its *stored pointer* by returning its value and replacing it with `nullptr`. It doesn't destroy the managed object but `unique_ptr` is free of the responsbility of deleting the object.
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

## `shared_ptr`

Defined in the `<memory>` header in the C++ Standard Library.

A `shared_ptr` shares the **ownership** of a resource.

A `shared_ptr` can be copied, passed by value in function arguments, and assigned to other `shared_ptr` instances. All instances point to the **same object**, and point to the **same control block** which keeps the reference count. When reference count reaches 0, the control block deletes the memory resource and itself.

```c++
auto ptrA = make_shared<Song>("May it be", "Enya");
auto ptr1 = ptrA;
ptr1.use_count(); // get reference count

shared_ptr<Song> ptrB(new Song("Lady Gaga", "Just Dance"));
auto ptr2(ptrB);

shared_ptr<Song> ptrC(nullptr);
ptrC = make_shared<Song>("Elton John", "I'm Still Standing");
```

## `weak_ptr`

A `weak_ptr` holds a weak reference to an object that is owned by a `shared_ptr` without increasing the reference count. It must be converted to `shared_ptr` in order to access the referenced object. A `weak_ptr` will only return a *non-null* `shared_ptr` when there is a `shared_ptr` somewhere keeping the object alive.

`weak_ptr` helps to break the *circular references (cyclic dependency)* between `shared_ptr` instances. Take this `C->A<->B` as an example: C points to the circular reference entity made by A and B.

- If all pointers are `shared_ptr`: when going out of the scope, only C will be released properly. After C is deleted, A cannot be released because the reference count for A is non-zero (pointed by B). Thus, B cannot be released, too.

- If B points to A as a `weak_ptr`, others are `shared_ptr`: when going out of the scope, all pointers will be cleaned up. Since B points to A *weakly*, the reference count for A reaches zero after C is release. Then, A can be cleaned up and reference count for B decreases to zero. Thus, B is cleaned up, too.