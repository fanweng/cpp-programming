# Concepts

## Object-Oriented Programming (OOP)

- Classes and objects
- Encapsulation
- Information-hiding
- Inheritance
- Polymorphism

## Classes and Objects

Class is a blueprint from which objects are created. It has attributes (data) and methods (functions).

Object represents a specific instance of a class.

```c++
class Player {
private:
    std::string name;
    int health;
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

- **Initialization list**

A better way to create an object with initial attribute values. Assignment in the constructor is a less efficient way because the object is first created with empty values then is assigned with the intial values, one step more than the initialization list.

- **Copy constructor**

Called when an object is copied:
1. passing object *by value* as a parameter, `void display_player(Plaery p)`
2. returning an object from a function *by value*, `return player_obj`
3. construct one object based on another with the same class, `Player another_player {player_obj}`

C++ uses a default compiler-defined copy constructor if there is no user-defined one. It copies the value of each data member to the new object, which is perfectly fine in many cases. (NOTE: if data member is a *pointer*, the pointer itself will be copied, not the data it points to.)

Best practices:
1. use your own copy constructor when the class has **raw pointer** members
2. provide the copy constructor with a **const reference** parameter
3. use STL classes as they're provided with copy constructors
4. avoid using raw pointer data members if possible

- **Destructor**

A special member method, invoked when object is destroyed. It has the same name as the class with a preceeding tilde (~). No return type and no parameters. No overloading is allowed. It's useful to release memory and other resources.

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

## Inheritance

It is possible to inherit attributes and methods from one class to another.

- **derived class (child)**: class that inherits from another class.
- **base class (parent)**: class being inherited from.

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

## Polymorphism

Polymorphism is an object can behave differently based on differnt circumstances.

Rumtime polymorphism: virtual function

Compile time polymorphism: function overloading, operator overloading

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

1. Function templates: A generic function can be used for different data types.

```c++
template <typename T>
T myMax(T x, T y) {
    return (x > y)? x : y;
}

cout << myMax<int>(3, 7) << endl;
cout << myMax<double>(1.2, 3.8) << endl;
```

2. Class templates: if the class definition is independent of the data type.

```c++
template <typename T>
class Array {
private:
    T *ptr;
    int size;
public:
    Array(T arr[], int s);
    void print;
};

template <typename T>
Array<T>::Array(T arr[], int s) { // constructor
}

template <typename T>
void Array<T>::print() { // method
}
```