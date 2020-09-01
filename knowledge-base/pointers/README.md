# Pointers

## `unique_ptr`

Defined in the `<memory>` header in the C++ Standard Library.

A `unique_ptr` cannot be copied to another `unique_ptr`, passed by value to a function, or used in any C++ Standard Library algorithm that requires copies to be made.

It can only be moved by `std::move()`. This means the ownership of the memory resource is transferred to another `unique_ptr` and the original `unique_ptr` no longer owns it.

```c++
// ptrA -> Song object
auto ptrA = make_unique<Song>("May it be", "Enya");
// ptrA
// ptrB -> Song object
auto ptrB = std::move(ptrA);
```

## `shared_ptr`

Defined in the `<memory>` header in the C++ Standard Library.

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