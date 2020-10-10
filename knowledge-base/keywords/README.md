# Keywords

## Access Specifiers

Access specifiers define how the members (attributes and methods) of a class can be accessed.

* `public`: members are accessible from outside the class.
* `private`: members cannot be accessed or viewed from outside the class.
* `protected`: members cannot be accessed from outside the class, however, they can be accessed in **inherited** classes.

## `virtual`

A `virtual` member function is declared within a base class and is overriden by a derived class. They are mainly used to achieve **Runtime Polymorphism** and the function resolution is done at runtime. Virtual functions should be accessed using pointer or reference of base class type to achieve runtime polymorphism.

## `explicit`

The compiler is allowed to make the implicit conversion (type casting) to the single-arugment constructor. The `explicit` keyword tells the compiler that a certain constructor may not be used for implicit type conversion.

```c++
class Foo {
public:
    Foo(int x);
};

class Bar {
public:
    explicit Bar(int x);
};

void main() {
    Foo obj = 's';      // 's' will be implicitly converted to int and calls Foo::Foo(int)

    Bar a = 's';        // Compile-time error: can't convert 's' to an object of type Bar
    Bar b = 3;          // Compile-time error: can't convert 3 to an object of type Bar
    Bar c(3);           // OK: calls Bar::Bar(int)
    Bar d = Bar(3);     // OK: calls Bar::Bar(int)
    Bar e = (Bar)3;     // OK: calls Bar::Bar(int)
}
```

## `auto`

The `auto` keyword specifies that the type of the variable that is being declared will be automatically deducted from its initializer. Developer can spend less time having to write out the data type that compiler already knows. Thus, the time for compilation increases slightly but it doesn't affect the runtime of the program.

```c++
auto x = 4;
auto ptr = &x;
std::cout << std::typeid(x).name() << std::endl;
std::cout << std::typeid(ptr).name() << std::endl;
```

## `using`

1. *using-directives* for namespaces.
2. *using-declarations* for introducing namespace/class members.
3. For type aliasing, similar to `typedef`.

```c++
using namespace std; // 1
using std::string; // 2
using int_ptr = std::shared_ptr<int>; // 3
int_ptr myPtr;
```

## `constexpr`

`constexpr` specifier evaluates an object or function at compile time and the expression can be used in other constant expressions. The main idea is to improve the performance by doing the computation at compile time rather than run time.

* Difference with `const`

`const` may be initialized at compile time or run time. Therefore, it cannot be used for initializing a compile-time constant.

```c++
const     double PI1 = 3.14;
constexpr double PI2 = 3.14;

constexpr double AREA1 = PI1 * 1^2; // error
constexpr double AREA2 = PI2 * 1^2; // OK
```
* Difference with `inline`

Both are for performance improvement. `inline` only expands the function at compile time which saves time of function call overhead, the expression is still evaluated at run time.