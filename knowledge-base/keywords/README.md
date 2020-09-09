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