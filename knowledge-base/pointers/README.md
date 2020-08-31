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
