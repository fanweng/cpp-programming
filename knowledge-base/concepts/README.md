# Concepts

## Object-Oriented Programming (OOP)

- Classes and objects
- Encapsulation
- Information-hiding
- Inheritance
- Polymorphism

## `struct` vs. `class`

Use `struct` for passive objects with public access (default), don't declare methods in it.

Use `class` for active objects with default private access members, implementing getters/setters and other methods in it.

## Classes and Objects

Class is a blueprint from which objects are created. It has attributes (data members) and methods (member functions).

Object represents a specific instance of a class.

```c++
class Player {
private:
    std::string name;
    int health;
    // data members must be defined at compile time
    std::string skill[5];
public:
    void talk(std::string);
};
Player mike;
mike.name = "Mike";
Player *mary = new Player();
(*mary).name = "Mary";
mary->health = 100;
delete mary;
```

### Constructors and Destructors

- **Constructor**

A special member method, invoked during the object creation. It has the same name as the class, no return type, and can be overloaded. If no constructor provided in the class, C++ will use default no-argument constructor with no initialization.

Constructors can be overloaded, i.e. multiple constructors can exist with the same name/usage but have different arguments.

- **Initialization list**

A better way to create an object with initial attribute values. Assignment in the constructor is a less efficient way because the object is first created with empty values then is assigned with the initial values, one step more than the initialization list.

- **Copy constructor**

Called when an object is **copied**:
1. passing object *by value* as a parameter, `void display_player(Player p)`
2. returning an object from a function *by value*, `return player_obj`
3. construct one object based on another with the same class, `Player another_player {player_obj}`

C++ uses a default compiler-defined copy constructor if there is no user-defined one. It copies the value of each data member to the new object, which is perfectly fine in many cases. (NOTE: default copy constructor uses **shallow copy**, i.e. if a data member is a *raw pointer*, the pointer itself will be copied, not the data it points to. In this situation, both pointers point to the same memory area, if one object is released by destructor, the memory area is no longer valid. The other pointer still refers to the memory without knowing anything.)

The workaround for the above issue is **deep copy**.

```c++
/* suppose data is a raw pointer data member in the Deep class */
Deep::Deep(const Deep &source)
    : Deep{*source.data} { // delegating way
}
Deep::Deep(const Deep &source) { // assignment way
    data = new int;
    *data = *source.data;
}
```

Best practices:
1. use your own copy constructor when the class has **raw pointer** members
2. provide the copy constructor with a **const reference** parameter
3. use STL classes as they're provided with copy constructors
4. avoid using raw pointer data members if possible

- **Move ctor**

C++11 introduced `move` constructor which moves an object instead of copies it. Copy constructor doing deep copy can have significant performance bottleneck. For example: 

```c++
/* suppose data is a raw pointer data member in the Deep class */
Player::Player(const Player &source) {
    data = new int;
    *data = *source.data;
}
vector<Player> vec; // only deep copy ctor, no move ctor is defined
// temporary object (rvalue) is created
// compiler uses copy ctor to add it to the vector
// temp object then is dtor-ed
vec.push_pack(Player{"mike"});
```

Move constructor simply *copies the address of resource from source to the current object*, and *nulls out the pointer in the source*. It's like a shallow copy plus nulling out the copied source pointer.

```c++
/* suppose data is a raw pointer data member in the Deep class */
Player::Player(Player &&source)
    : data{source.data} {
        source.data = nullptr;
}
vector<Player> vec; // with move ctor is defined
// because temporary object is an rvalue
// compiler will use move ctor to move it into the vector, more efficient than copy
// temp object then is dtor-ed and its pointer is null-ed
vec.push_pack(Player{"mike"});
```

- **Explicit ctor**

`explicit` keyword raises the compilation error when the implicit call to a class constructor: [keywords/README.md : keywords/explicit](../keywords/README.md)


- **Destructor**

A special member method, invoked when object is destroyed. It has the same name as the class with a preceding tilde (~). No return type and no parameters. No overloading is allowed. It's useful to release memory and other resources.

```c++
class Player {
private:
    std::string name;
    int health;
public:
    // Overloaded constructors
    Player();
    Player(std::string name_val);
    Player(std::string name_val, int health_val);
    Player(std::string name_val, int health_val, int xp = 0);   // default parameter
    // Copy Constructor
    Player(const Player &source);
    // Destructor
    ~Player() {std::cout << "Destructor called" << std::endl;}
};
Player::Player() {
    name = "None";  // this is assignment, not initialization
    age = 0;
}
Player::Player(std::string name_val)
    : Player {name_val, 0} {    // delegating constructor
}
Player::Player(std::string name_val, int health_val)
    : name{name_val}, health{health_val} {  // initialization lists
}
Player::Player(const Player &source)
    : name{source.name}, health{source.health}, xp{source.xp} {
}
```

- **Request and Suppress Methods**

Compiler can generate implicitly special methods if `=default` is used.

```c++
class SomeType{
public:
  SomeType() = default; // tell compiler to generate a default ctor
  SomeType(int value) { // user defines the ctor
    std::cout << "SomeType(int) " << std::endl;
  };
  explicit SomeType(const SomeType&) = default; // to generate a explicit default copy ctor
  virtual ~SomeType() = default; // to generate a virtual default dtor
};

int main(){
  SomeType someType; // call default ctor
  SomeType someType2(2);
  SomeType someType3(someType2); // call copy ctor
}
```

Compiler can restrict special methods if `=delete` is defined.

```c++
class NonCopyableClass {
public:
  NonCopyableClass() = default;
  // disallow copying
  NonCopyableClass& operator =(const NonCopyableClass&) = delete;
  NonCopyableClass (const NonCopyableClass&) = delete;
  // allow moving
  NonCopyableClass& operator =(NonCopyableClass&&) = default;
  NonCopyableClass (NonCopyableClass&&) = default;
};

class TypeOnStack {
public:
  void* operator new(std::size_t) = delete;
};

class TypeOnHeap {
public:
  ~TypeOnHeap() = delete;
};

void onlyDouble(double){}
template <typename T>
void onlyDouble(T) = delete;

int main(){
  NonCopyableClass nonCopyableClass; // ok: default ctor
  TypeOnStack typeOnStack; // ok: object created on stack
  TypeOnHeap* typeOnHeap = new TypeOnHeap; // ok: object created on heap
  onlyDouble(3.14); // ok: double type argument

  NonCopyableClass nonCopyableClass2(nonCopyableClass); // error: copy ctor disallowed
  TypeOnStack* typeOnHeap2 = new TypeOnStack; // error: cannot create on heap
  TypeOnHeap typeOnStack2; // error: cannot create on stack
  onlyDouble(2011); // error: int argument not accepted
}
```

## Operator Overloading

C++ doesn't know how to deal with the operators on a user-defined type data, it will only apply the default member-wise assignment `=` (shallow copy). Operator overloading must be explicitly defined before using that operator on user-defined data types.

The majority of C++ operators can be overloaded except `::`, `:?`, `.*`, `.`, `sizeof`.

#### Assignment operator

Since the default assignment operator is shallow copy, sometimes it's required to overload the **copy assignment operator** to do a deep copy.

As mentioned in the *Move ctor* section, move operation is more efficient when dealing with the raw pointer data. Thus, we need to overload the **move assignment operator** sometimes.

```c++
class MyString {
private:
    char *str;
public:
    // Overloading copy assignment operator
    MyString &operator=(const MyString &rhs);
    // Overloading move assignment operator
    MyString &operator=(MyString &&rhs);
};

MyString &MyString::operator=(const MyString &rhs) {
    if (this == &rhs) { // if the same object, return the current object
        return *this;
    }
    delete [] str; // deallocate storage for this->str since it'll be overwritten
    str = new char[std::strlen(rhs.str)+1]; // allocate storage for the deep copy
    std::strcpy(str, rhs.str); // perform deep copy
    return *this;
}

MyString &MyString::operator=(MyString &&rhs) { // rvalue reference, it'll be changed so it's not const
    if (this == &rhs) {
        return *this;
    }
    delete [] str;
    str = rhs.str;
    rhs.str = nullptr;
    return *this;
}
str1 = str2;        // we write this in the real code
str1.operator=(s2); // operator= method is called actually
str1 = MyString{"Hello"}; // move assignment is called, more efficient than copying
```

#### Overloading operators as member methods

If using member method to overload an operator, the element on the left-hand side of the operator must be that class object.

```c++
/* Unary Operators */
Number Number::operator-() const;   // -n
Number Number::operator++();        // pre-increment, ++n
Number Number::operator++(int);     // post-increment, "int" isn't used just to distinguish with pre-increment, n++
bool Number::operator!() const;     // !n

/* Binary Operators */
MyString MyString::operator+(const MyString &rhs) const;
MyString MyString::operator-(const MyString &rhs) const;
bool MyString::operator==(const MyString &rhs) const;
bool MyString::operator<(const MyString &rhs) const;

MyString larry{"Larry"};
MyString moe{"Moe"};
MyString result = larry + moe;  // Ok
result = larry + "Moe";         // Ok
result = "Moe" + larry;         // "Moe".operator+(larry), Error, must be a class object
```

#### Overloading operators as global functions (non-member methods)

Since operators may need to access the private members of the class object, they need to be declared as **friend** functions in the class declaration. Otherwise, they need to use getter/setter methods.

```c++
Number operator-(const Number &obj);
Number operator++(Number &obj);         // ++n
Number operator++(Number &obj, int);    // n++
bool operator!(const Number &obj);

class MyString {
    friend MyString operator+(const MyString &lhs, const MyString &rhs);
...
};

MyString operator+(const MyString &lhs, const MyString &rhs) {
    size_t buf_size = std::strlen(lhs.str) + std::strlen(rhs.str) + 1;
    char *buf = new char[buf_size];
    std::strcpy(buf, lhs.str);
    std::strcat(buf, rhs.str);
    MyString temp{buf};
    delete [] buf;
    return temp;
}
MyString operator-(const MyString &lhs, const MyString &rhs);
MyString operator==(const MyString &lhs, const MyString &rhs);
MyString operator<(const MyString &lhs, const MyString &rhs);

MyString larry{"Larry"};
result = "Moe" + larry; // Ok with non-member function 
```

#### Overloading stream insertion and extraction operators

When overloading the insertion stream operator, make sure to return the *reference* of stream so that the *chain of insertion* is possible.

```c++
class MyString {
    friend std::ostream &operator<<(std::ostream &os, const MyString &rhs);
    friend std::istream &operator>>(std::istream &in, MyString &rhs);
...
};

std::ostream &operator<<(std::ostream &os, const MyString &rhs) {
    os << rhs.str;
    return os;
}

std::istream &operator>>(std::istream &in, MyString &rhs) {
    char *buf = new char[100];
    in >> buf;
    rhs = MyString(buf); // depends on the overloaded copy or move assignment operator
    delete [] buf;
    return in;
}
```

## Inheritance

It is possible to inherit attributes and methods from one class to another.

- **derived class (child)**: class that inherits from another class.
- **base class (parent)**: class being inherited from.

Both *inheritance* and *composition* reuse code but they are different:
- Inheritance "is-a" relationship: student "is a" person
- Composition "has-a" relationship: person "has a" book

```c++
class MyClass1 {
};
class MyClass2 {
};
// Multiple inheritance
class MyChild : public MyClass1, public MyClass2 {
};
// Multilevel inheritance
class MyGrandChild : public MyChild {
};
```

#### The ctor/dtor of derived classes

When a derived object is created, base class ctor executes then the derived class.

When a derived object is destroyed, derived class dtor executes then the base class.

A derived class does NOT inherit: ctor, dtor, overloaded assignment operators, friend functions. C++11 allows explicit inheritance of base "non-special" ctor with `using Base_Class::Base_Class` and some limitations.

If we want to control exactly which Base class ctor to use, simply add the Base class ctor with arguments to the initialization list of the Derived class ctor.

```c++
Derived::Derived(int x)
    : Base(x), {optional initializers for Derived} {
    // Code
}
```

#### The copy/move ctors of derived classes

Copy/Move constructors are not inherited from the Base class. If we don't provide our own, compiler will use default versions.

In the following code, `other` is a Derived object and it Derived object content is **sliced** out to fit in the Base class copy ctor.

```c++
Derivded::Derived(const Derived &other)
    : Base(other), {Derived initialization list} {
    // Code
}
```

#### The operator= of derived classes

Overloaded assignment operator is not inherited from the Base class.

```c++
class Base {
    int value;
public:
    Base &operator=(const Base &rhs) {
        if (this != &rhs) {
            value = rhs.value;
        }
        return *this;
    }
};
class Derived : public Base {
    int double_value;
public:
    Derived &operator=(const Derived &rhs) {
        if (this != &rhs) {
            Base::operator=(rhs);   // rhs is sliced out to fit in the Base class
            double_value = rhs.double_value;
        }
        return *this;
    }
};
```

#### Override/Redefine base class methods

Default binding for C++ is **static** so that method calls to use is bound at compile time. Derived class object will use `Derived::method1` unless `Base::method` is explicitly invoked from Derived class object.

```c++
class Account {
public:
    void deposit(double amount) { balance += amount; }
    
};
class Saving_Account : public Account {
public:
    void deposit(double amount) {   // redefine the deposit method
        amount += some_interest;
        Account::deposit(amount);   // invoke base class method
    }
};
```

## Polymorphism

Polymorphism is an object can behave differently based on different circumstances.

Rumtime polymorphism / Late binding / dynamic binding: virtual function

Compile time polymorphism / Early binding / Static binding: function overloading, operator overloading

#### Implementation

The compiler will not hardcode the method call, it will compile a lookup table to retrieve the function by offsetting to the correct address of the object.

#### Runtime Polymorphism

It is achieved by using inheritance and virtual functions, and it is called by Base class pointer/reference.

- **Virtual functions:** are overridden functions bound dynamically. Unlike redefined functions, which are bound statically.

- **Virtual destructor:** if a Derived class is destroyed by deleting its storage via the Base class pointer, the behavior is undefined in the C++ standard. Therefore, if a class has virtual functions, a public virtual destructor *MUST* be provided

```c++
class Account {
public:
    virtual bool deposit(double amount);
    virtual ~Account();
};
class Savings_Account : public Account {
    virtual bool deposit(double amount); // "virtual" keyword is not required in the derived class
    virtual ~Savings_Account();
};
Account *p1 = new Account();
Account *p2 = new Savings_Account();
p1->deposit(100.0); // Account::deposit
p2->deposit(200.0); // Savings_Account::deposit
delete p1;
delete p2;
```
#### `override` specifier

It is added to the end of Derived class virtual function method, telling compiler to ensure the function will override the the exactly same Base class virtual function.

#### `final` specifier

Added to the end of a class: prevents a class from being derived from.

Added to the end of a method: prevents a virtual method from being overridden in the derived class.

#### Pure `virtual` functions and Abstract classes

- Abstract class:
    * cannot instantiate objects
    * used as base classes in the inheritance hierarchies
    * contains at least one **pure** `virtual` function
        + pure virtual function is used to make a class abstract
        + specified with `=0` in its declaration, `virtual void foo() = 0;`
        + don't provide implementation typically

- Concrete class:
    * used to instantiate objects
    * all member functions are defined
    * derived class MUST override the base class' pure virtual function, i.e. concrete class MUST provides implementation. Otherwise the derived class would be considered as an abstract class

#### Use a class as an interface

An abstract class that has only pure virtual functions can be used as an interface. These functions provide a set of `public` services to the user of the class. The derived class MUST implement those functions.

```c++
class Printable { // interface class providing a print() service
    friend ostream &operator<<(ostream &os, const Printable &obj);
public:
    virtual void print(ostream &os) const = 0; // pure virtual function
};
ostream &operator<<(ostream &os, const Printable &obj) {
    obj.print(os);
    return os;
}

class Any_Class : public Printable { // derived concrete class
public:
    virtual void print(ostream &os) override { // override/implement the print() service
        os << "Hello from Any_Class";
    }
};
```

## Exceptions

When an error occurs, C++ will throw an exception (error).

* `throw` keyword throws an exception which lets us create a custom error. 
* `try` statement defines a block of code to be tested for errors.
* `catch` statement defines the error handling, a block of code to be executed if an error occurs in the `try` block.

```c++
try {
    int age = 15;
    if (age < 18) {
        throw age;
    }
}
catch (int num) {
    cout << "Age " << num << " denied!" << endl;
}

```

If we don't know the `throw` type, "three dots" syntax `catch(...) {}` will handle any type of exception.

## Type Casting

`typid(expression)` allows to check the type of an expression.

#### Implicit conversion

It doesn't require any operator and automatically performs when a value is copied to a compatible type.

#### Explicit conversion

Many conversions that imply a different interpretation of the value require an explicit conversion, by using functional or C-like casting.

```c++
short a = 2000;
int b, c, d;
b = a;          // implicit: value of a is promoted from short to int
c = (int)a;     // explicit: C-like cast notation
d = int(a);     // explicit: functional notation
```

In order to control the type conversion between classes, there are four specific casting operators:

* `dynamic_cast`

It is used only with pointers and references to objects. Its purpose is to ensure that the result of the type conversion is a valid complete object of the requested class. Therefore, `dynamic_cast` is always successful when __casting a class to its base class__.

Casting from base class to derived class produces a compilation error unless the base class is **polymorphic**. During the runtime, `dynamic_cast` performs a special checking to ensure the expression yields a valid conversion.

* `static_cast`

It performs conversions between pointers to related classes, for both base-to-derived and derived-to-base. It is up to the programmer to ensure the conversion is safe because `static_cast` has no safety check as `dynamic_cast` does.

```c++
class MyBase {};
class MyDerived : public MyBase {};
MyBase *a = new MyBase;
MyDerived b* = static_cast<MyDerived*>(a);
```

* `reinterpret_cast`

It converts any pointer type to any other pointer type, even of unrelated classes. The operation result is a simple binary copy of the value from one pointer to the other.

* `const_cast`

This operator manipulates the *const-ness* of an object, either to be set or to be removed.

```c++
const int x = 50;
const int* y = &x;
cout<<"old value is"<<*y<<"\n"; // y=50
int* z=const_cast<int *>(y);    // only this can allow us to make change to the value of const pointer y, otherwise const value of x is read-only
*z=100;
cout<<"new value is"<<*y;       // y=100
```

## Namespace

Namespaces allow us to group named entities that otherwise would have *global scope* into narrower scopes. This organizes the elements of programs into different logical scopes referred to by name.

* Namespace declaration appears only at global scope.
* Namespace declaration can be nested within another namespace.
* No access specifiers (`public`, `private`, etc.).
* No semicolon after the closing brace.
* Definition of namespace can be split into several units.

```c++
namespace ns 
{ 
    void display(); 
    class geek 
    { 
    public: 
       void display(); 
    }; 
} 
void ns::geek::display() 
{ 
    cout << "ns::geek::display()\n"; 
} 
void ns::display() 
{ 
    cout << "ns::display()\n"; 
} 
```

## Template

Template is a powerful tool which idea is to pass data type as a parameter so that we don't need to write the same code for different data type. Templates are expanded at compiler time, similar to macros, but compiler does type checking before template expansion.

Keywords: `template`, `typename`/`class`.

1. **Function templates:** A generic function can be used for different data types.

```c++
template <typename T>
T myMax(T x, T y) {
    return (x > y)? x : y;
} // function won't be compiled at here, because T is unknown

cout << myMax<int>(3, 7) << endl;   // now compiler can generate appropriate function becuase T is int
cout << myMax(1.2, 3.8) << endl;    // often, compiler can deduce the T according to the inputs
```

2. **Class templates:** if the class definition is independent of the data type, i.e. allowing *plug-in* any data type.

```c++
template <typename T, int N>
class Array {
    friend std::ostream &operator<<(std::ostream &os, const Array<T, N> &arr) {
        os << "[ ";
        for (const auto &val: arr.value) {
            os << val << " ";
        }
        os << "]" << std::endl;
        return os;
    }
public:
    Array() = default;
    Array(T init_val) {
        for (auto &item: values) {
            item = init_val;
        }
    }
    void fill(T val) {
        for (auto &item: values) {
            item = val;
        }
    }
    int get_size() const {
        return size;
    }
    T &operator[](int index) {  // overloaded subscript operator for easy use
        return values[index];
    }
private:
    T values[N];    // N needs to be known at compile time
    int size {N};
};

/* Implementation */
Array<int, 5> intNums {1};  // Array of five elements filled with integer one
```

## l-value and r-value

- **lvalue**

Refers to **memory location** which identifies an object. An *lvalue* may appear as either left hand or right hand side of an assignment operator.

L-value reference uses `&`.

- **rvalue**

Refers to **data value** that is stored at some address in memory. An *rvalue* can't have a value assigned to it which means it can only appear on the right hand side of an assignment operator.

R-value reference uses `&&`.

```c++
int x{100};
void func(int &num);    // A
void func(int &&num);   // B
func(x);    // calls prototype A, x is an lvalue
func(300);  // calls prototype B, 300 is an rvalue 
```