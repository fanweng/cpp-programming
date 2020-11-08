# Keywords

## Access Specifiers

Access specifiers define how the members (attributes and methods) of a class can be accessed.

* `public`: members are accessible from outside the class.
* `private`: members cannot be accessed or viewed from outside the class.
* `protected`: members cannot be accessed from outside the class, however, they can be accessed in **inherited**/**derived** classes (still not accessible by the *object* of class).

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

## `static`

If a class data member is declared as `static`:
- the data member belongs to the class, *not the specific object*
- useful to store class-wide information

If a class method is declared as `static`:
- it's independent of any particular object
- can only access `static` data members and functions, not even `this` pointer
- can be called using the class name
- can be called even without any object is created

```c++
/* .hpp */
class Player {
private:
    static int num_players; // cannot be initialized here
public:
    static int get_num_players();
};

/* .cpp */
int Player::num_players = 0; // init in the .cpp
Player::Player(std::string name_val)
    : name{name_val} {
        ++num_players;
}
Player::~Player() {
    --num_players;
}
int Player::get_num_players() {
    return num_players;
}
```

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