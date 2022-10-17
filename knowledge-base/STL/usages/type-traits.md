# Type Traits

`<type_traits>` library enables to check/compare/modify types at **compile time**, which reduces overhead on the runtime.

## Primary Type Categories

Check the type category of your type.

```c++
template <typename T> T foo(T a) {
    static_assert(std::is_integral<T>::value, "T is not integral");
}
foo(10.1); // T=double, T is not integral, foo() exits

struct A {
    int a;
    int f(int){ return 2011; }
};
int a = std::is_member_object_pointer<int A::*>::value; // 1
int b = std::is_member_function_pointer<int (A::*)(int)>::value; // 1
```

## Composite Type Categories

`is_arithmetic` = `is_floating_point` or `is_integral`

`is_fundamental` = `is_arithmetic` or `is_void`

`is_object` = `is_arithmetic` or `is_enum` or `is_pointer` or `is_member_pointer`

`is_reference` = `is_lvalue_reference` or `is_rvalue_reference`

`is_compound` = complement of `is_fundamental`

`is_member_pointer` = `is_member_object_pointer` or `is_member_function_pointer`

## Type Comparisons

`is_base_of`: check if `Derived` class is derived from `Base`

`is_convertible`: check if `From` class can be converted to `To`

`is_same`: check if `T` class is the same as `U`

## Type Modifications

Modify types during compile time: `add_const`, `remove_const`, `make_signed`, etc.

```c++
std::is_const<int>::value; // 0
std::is_const<const int>::value; // 1
std::is_const<add_const<int>::type>::value // 1
```
