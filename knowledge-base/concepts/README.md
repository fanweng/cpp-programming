# Concepts

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