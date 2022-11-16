#keyword 

# Keywords

## Access Specifiers

Access specifiers define how the members (attributes and methods) of a class can be accessed.

* `public`: members are accessible from outside the class.
* `private`: members cannot be accessed or viewed from outside the class.
* `protected`: members cannot be accessed from outside the class, however, they can be accessed in **inherited**/**derived** classes (still not accessible by the *object* of class).

#### Mode of Inheritance

```c++
class derived : inheritance_mode base {}
```

Members accessible in the *derived* class or in the main function?

| Base Class Member Access Specifier | `public` Mode of Inheritance | `protected` Mode | `private` Mode |
|-------------|-----------|-----------|---------|
| `public`    | Public    | Protected | Private |
| `protected` | Protected | Protected | Private |
| `private`   | Hidden    | Hidden    | Hidden  |


## `virtual`

A `virtual` member function is declared within a base class and is overridden by a derived class. They are mainly used to achieve **Runtime Polymorphism** and the function resolution is done at runtime. Virtual functions should be accessed using pointer or reference of base class type to achieve runtime polymorphism.

## `explicit`

The compiler is allowed to make the implicit conversion (type casting) to the single-argument constructor. The `explicit` keyword tells the compiler that a certain constructor may not be used for implicit type conversion.

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
    Bar d{3};           // OK: calls Bar::Bar(int)
    Bar e = Bar(3);     // OK: calls Bar::Bar(int)
    Bar f = (Bar)3;     // OK: calls Bar::Bar(int)
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

## `static`

[Static](static.md)

## `const`

A `const` object should not be modified at any time. Compiler assumes a method could change the class object so that a `const` object calls any of its method without `const` qualifier will throw a compiler error.

```c++
class Player {
public:
    std::string get_name() const;   // with "const" qualifier, if get_name modifies this object, it gets compiler error
    std::string set_name();         // set_name() is expected to modify this object
};
const Player villain {"Villan", 100, 0};
villain.set_name("Hero");   // compiler error, because const-correctness
villain.get_name();         // ok, because get_name() is declared with const qualifier
```

## `constexpr`

[Constant Expressions](constexpr.md)

## `this`

`this` contains the address of the object, which can only be used in the class scope. All member access is done via the `this` pointer.

```c++
void Account::set_balance(double bal) {
    balance = bal; // this->balance is implied
}
void Account::compare_balance(const Account &other) {
    if (this == &other) ...
}
```

## `friend`

`friend` function or class can access the private class member.

Friendship is declared explicitly in the class that is granting friendship, i.e. friendship must be granted not taken:
- not symmetric: if A is a friend of B, then B is not a friend of A
- no transitive: if A is a friend of B and B is a friend of C, then A is not a friend of C

```c++
class Player {
private:
    friend void display_player(Player &p); // this non-member function can access all private members of Player class
    friend void other_class::show_player(Player &p); // this member function of other_class can access all private members of Player class
    friend class another_clas;  // another_class can access all private members of Player class
};
```

## `noexcept`

This specifies whether a function could throw exceptions or not.

```c++
void func1() noexcept;          // func1 doesn't throw
void func2() noexcept(false);   // func2 may throw
```