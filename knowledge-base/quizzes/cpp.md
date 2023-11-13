# C++ Questions #

## OOP Features ##

+ class/object
+ inheritance
+ data-hiding
  + abstraction
  + encapsulation
+ polymorphism

See C++ Programming notes [concepts](../concepts/README.md#concepts).

### What is inheritance?

Inheritance allows us to define a `class` in terms of another `class`, which reuses the code, enables fast implementation and easier maintenance.

#### Diamond inheritance problem

If a class A inherits from two base classes B and C, and both B and C have the same member function signature (name and parameter types), this will cause a **compilation error** because the member function is ambiguous.

```c++
class B {
public:
    void foo() {
        std::cout << "B::foo()" << std::endl;
    }
};

class C {
public:
    void foo() {
        std::cout << "C::foo()" << std::endl;
    }
};

class A : public B, public C {
};

int main() {
    A obj;
    obj.foo(); // Ambiguous call - Compiler error
    obj.B::foo(); // ok
    obj.C::foo(); // ok
    return 0;
}
```

### What is polymorphism?

Polymorphism is an object can behave differently based on different circumstances.

+ Rumtime/Dynamic polymorphism or Late/Dynamic binding
	+ Method **overriding**: `virtual` function
+ Compile-time/Static polymorphism or Early/Static binding
	+ Method **overloading**
	+ Operator **overloading**

Detailed notes [polymorphism](../concepts/README.md#polymorphism).


#### How runtime polymorphism works?

Virtual function table - VTable.



## Keywords ##

### What is a `class` and an object?

`class` is the building block of C++ object oriented programming. It's a user defined data type, which holds its own data members and member functions, defining the properties for an object. No memory is allocated when a `class` is defined.

Object is an instance of a `class`. Since the instantiation/creation, the object takes up some memory.

### What is `virtual` keyword?

If a member function marked as `virtual`, its implementation will be called based on the **actual type of the object** being pointed to at runtime (polymorphism).

#### Can a constructor be `virtual`?

No, `virtual` function relies on a concrete object to call the correct function, the constructor is used to construct an object. That is if a constructor is `virtual`, the compiler would not know which constructor gets called because there is no object yet.

### What is `static`?

+ Static member variable
  + that variable is shared across all instances of the class
  + only one copy in the memory shared by all instances
  + defined in the implementation file, outside the class declaration
+ Static member function
  + that function belongs to class itself, not specific to one object
  + can be invoked using class name, without creating an instance
+ Static local variable in a function
  + similar to C
+ Static global variable
  + similar to C



## Pointers ##

### Pointer vs. Reference

| Pointers | Reference |
|----------|-----------|
| can be initialized to `nullptr` | must be initialized when declared, can't be null |
| can be reassigned to a different memory location | can't be reassigned once bound to an object |
| manual memory management using `new`/`delete` | alias to the object, lifecycle is automatically managed |
| more flexible when using | primarily used for function parameters |

Additional notes [pointers](../pointers/README.md)

> C doesn't have the concept of reference.

### Smart Pointers

For raw pointer, dynamic allocation `new T` must be followed by a manual deallocation `delete T`, same as the `new T[N]` followed by `delete[] T`. If a third-party library returns a pointer, we don't really know if it is dynamically-allocated data? If so, we also don't know who is responsible for the deallocation.

Smart pointer fixes the above issues by providing automatic memory management. When a smart pointer goes out of scope, its destructor gets triggered and memory cleanup is performed.

More notes on the smart pointer [smart pointer](../pointers/smart-pointer.md).

#### `std::unique_ptr`

A `std::unique_ptr` owns of the object it points to, and no other smart pointers can point the same object. When it goes out of scope, the object is deleted.

```c++
std::unique_ptr<Type> ptr1(new Type);
std::unique_ptr<Type> ptr2 = std::make_unique<Type>(...initializer...);

std::unique_ptr<Type> ptrCopy = ptr1; // Error! Copy is not allowed
void foo(std::unique_ptr<Type> p) {...}
foo(ptr1); // Error! ptr1 is passed by copy, but copy is not allowed
```

#### `std::shared_ptr`

A `std::shared_ptr` owns the object it points to, but it allows for multiple references. An internal reference counter keeps track of the number of references. When the counter is decreased to zero, i.e. all references are out of scope, the resource will be destroyed.

```c++
std::shared_ptr<Type> ptr1(new Type);
std::shared_ptr<Type> ptr2 = std::make_shared<Type>(...initializer...);
```

But *circular reference* issue may occur for the `std::shared_ptr`. When `amy` goes out of scope, its destructor cannot clean up the memory because `bill->companion` still points to the `amy` object, likewise for `bill` goes out of scope. `std::weak_ptr` will rescue this.

```c++
struct Player
{
  std::shared_ptr<Player> companion;
  ~Player() { std::cout << "~Player\n"; }
};

int main()
{
  std::shared_ptr<Player> amy = std::make_shared<Player>();
  std::shared_ptr<Player> bill  = std::make_shared<Player>();

  amy->companion = bill; // (1)
  bill->companion  = amy; // (2)
}
```

#### `std::weak_ptr`

A `std::weak_ptr` is similar to a `std::shared_ptr` but it doesn't increase the reference count. It is a non-owning reference to an object that is managed by another `std::shared_ptr`.

```c++
std::shared_ptr<Type> p_shared = std::make_shared<Type>(...initializer...);
std::weak_ptr<Type> p_weak1(p_shared);  // created out of a shared ptr
std::weak_ptr<Type> p_weak2(p_weak1);   // created out of another weak ptr
```

`std::weak_ptr` must be converted to a `std::shared_ptr` using `lock()` before we try to access the managed object. The conversion fails if the managed object is deleted. Or, we can use `expire()` to check validity of the pointer.

```c++
/* acquire a shared_ptr from the weak_ptr */
if (std::shared_ptr p = p_weak1.lock()) {
  std::cout << "Read object: " << *p << std::endl;
}
else {
  std::cout << "Object is deleted." << std::endl;
}

/* Or, use expire() */
if (!p_weak1.expired()) {
  std::cout << "p_weak1 is valid\n";
}
else {
  std::cout << "p_weak1 is expired\n";
}
```

`std::weak_ptr` solves the circular reference problem by simply replacing the strong reference in the class definition.



## Memory Management ##

### What is RAII?

Resource Acquisition Is Initialization. The basic idea is when an object is created, it automatically acquires all resources, when it is destroyed, it automatically releases those resources.

This ensures the resources are properly managed, for example:
+ memory
+ file handles
+ network connections

RAII is typically implemented using class constructor and destructor.

```c++
class MyClass {
private:
  int* data;
  std::unique_ptr<int[]> arr;
public:
  MyClass(int size) : data(new int[size]) {
    arr = std::make_unique<int[]>(size); // allocate/acquire memory
  }
  ~MyClass() {
    delete[] data; // deallocate/release memory
    // smart point destroy by itself when going out of scope
  }
};
```



## Templates ##

[http://users.cis.fiu.edu/~weiss/Deltoid/vcstl/templates](http://users.cis.fiu.edu/~weiss/Deltoid/vcstl/templates)

The source code contains one copy of template function/class, but compiler will expand the template to multiple copies according to the `typename`.

Function *overloading* and *template* are examples of polymorphism feature of OOP. The difference is *overloading* functions do **similar** operations, but *template* are used when functions need to do **identical** operations.

```c++
template <typename T>
T myMax(T x, T y)
{
  return (x > y)? x: y;
}

template<class T, class U = char>
class A  {
public:
  T x;
  U y;
  A() { cout << "Constructor Called" << endl; }
};
 
int main()  {
  cout << myMax<int>(3, 7) << endl;
  A<char> a;  // This will call A<char, char> by default
}
```

**Non-type** parameters are mainly used for specifying max/min values or other constant values. They must be `const`.

```c++
template <class T, int max>
int arrMin(T arr[], int n) {...}
  
int main()
{
  int arr1[]  = {10, 20, 15, 12};
  int n1 = sizeof(arr1)/sizeof(arr1[0]);

  // Second template parameter to arrMin must be a constant
  cout << arrMin<int, 10000>(arr1, n1) << endl;
}
```

**Static variables** in the template will keep a copy of local static variable for each (`typename`, `class`) instantiation of function template or class template.

```c++
template <typename T>
void fun(const T& x)
{
  static int i = 10;
  cout << ++i << endl;
  return;
}

template <class T>
class Test {  
private:
  T val; 
public:
  static int count = 0;
  Test() {
    count++;
  }
};

int main()
{    
  fun<int>(1);  // prints 11
  fun<int>(2);  // prints 12
  fun<double>(1.1); // prints 11

  Test<int> a;  // Test<int>::count is 1
  Test<int> b;  // Test<int>::count is 2 now
  Test<double> c;  // Test<int>::count is 1
  cout << Test<int>::count << endl;  // prints 2  
  cout << Test<double>::count << endl; //prints 1
}
```

**Template specialization** allows us to overwrite the template function/class with particular data type.

```c++
template <class T>
void fun(T a)
{
  cout << "The general template fun(): " << a << endl;
}
  
template<>
void fun(int a)
{
  cout << "Specialized Template for int type: " << a << endl;
}
  
int main()
{
  fun<char>('a');
  fun<int>(10);
}
```



## Lambda Expressions ##

It is a convenient way of defining an anonymous function object.

```c++
// capture list + optional parameters + opt trailing return type + lambda body
auto getOne = []() -> int { return 1;}
```

Sometimes it can be used to replace a function pointer.

```c++
bool isLargerThanThree (int n) {
    return (n > 3);
}

auto it1 = std::find_if(vec.begin(), vec.end(), isLargerThanThree);
auto it2 = std::find_if(vec.begin(), vec.end(), [](int n) {return n > 3;});
```

Capture list:
- `[]` - capture nothing
- `[=]` - capture local objects in scope by value
- `[&]` - capture local objects in scope by reference
- `[a, &b]` - capture the object `a` by value and `b` by reference
- `[this]` - capture `this` by reference

By default, *value-captures* cannot be modified inside the lambda because the compiler-generated method is marked as `const`. The `mutable` keyword allows modifying captured variables.

```c++
int x = 1;
auto f1 = [&x] { x = 2; }; // OK: x is a reference and modifies the original
auto f2 = [x] { x = 2; }; // ERROR: the lambda can only perform const-operations on the captured value
auto f3 = [x]() mutable { x = 2; }; // OK: the lambda can perform any operations on the captured value
```



## Modern C++ Features ##

References:
- GeeksForGeeks
- [Modern C++ Features](https://github.com/AnthonyCalandra/modern-cpp-features)

### C++11 ###

+ `std::move` semantics

First, `std::move` is a cheaper way to transfer resources. For example, moving a `std::vector` is just copying some pointers to the new vector, instead of copying every single element.

Second, `std::move` guarantees that the non-copyable types, such as `std::unique_ptr`, to properly transfer the ownership to another `std::unique_ptr`. See specific section **Smart Pointer**.

+ `static_assert` assertion

Static assertions are evaluated at compile time. `static_assert(x == y, "x != y");`

+ `auto` keyword

The type of the specified variable will be deduced by compiler according to the type of its initializer.

Extremely useful for readability, i.e. those complicated types (long declaration).

+ `decltype` operator

Returns the *declared type* of an expression passed to it. `const` qualifiers and references are maintained.

```c++
int a = 1;
decltype(a) b = a; // decltype(a) is int
```

+ Lambda expressions

See specific section **Lambda**.

+ `nullptr`

It is introduced to replace the `NULL` macro. `NULL` is constant integer zero, `(void*)0`. `nullptr` is a type of `nullptr_t`, which is an integer literal that evaluates to zero.

```c++
void func(int) {}
void func(bool)_{}
void func(int*) {}

func(NULL);    // error, ambiguous. NULL can be int/bool/int*
func(nullptr); // call the last func(int*)
```

+ `constexpr` expression

Constant expression specifier can be applied to variables, functions and class constructors, indicating the values and return values are constant. The constant expression is evaluated at the compile-time instead of run-time, which helps the program run faster and uses less memory.

+ `override` specifier

With the `override`, compiler will check the base class to see if there is a `virtual` function with the exact signature. If not, the compiler will throw an error. The parameter types and return types must be the same as well, no conversions are not allowed

```c++
class A {
  virtual void foo();
  void bar();
};

class B : A {
  void foo() override; // correct -- B::foo overrides A::foo
  void bar() override; // error -- A::bar is not virtual
  void baz() override; // error -- B::baz does not override A::baz
};
```

+ `final` specifier

It tells the compiler that a `virtual` function cannot be overridden in a derived class, or a class cannot be inherited from.

```c++
class A {
  virtual void foo();
};

class B : A {
  virtual void foo() final;
};

class C : B {
  virtual void foo(); // error, declaration of 'foo' overrides a 'final' function
};

class D final {};
class E : D {}; // error, base 'D' is marked 'final'
```

+ Deleted functions

`=delete` is an elegant and efficient way to disable the usage of a member function, preventing copies on the object.

```c++
class myClass {
  int x;

public:
  myClass(int x) : x{x} {};
  myClass(const myClass&) = delete;
  myClass& operator=(const myClass&) = delete;
  myClass(double) = delete;
};

myClass x {123};
myClass y = x;    // error, call to deleted copy constructor
y = x;      // error, operator= deleted
myClass z{1.2}    // error, conversion from double to int is disabled
```

+ Default functions

`=default` is an elegant and efficient way to provide a default implementation of a function, such as a constructor.

```c++
class myClass {
  myClass() = default;
  myClass(int x) : x{x} {}
  int x {1};
};
myClass a;        // a.x == 1
myClass a2 {123}; // a.x == 123
```

+ Range-based for loops

A convenient way to iterate over a container's elements.

```c++
std::array<int, 5> a {1, 2, 3, 4, 5};
for (int x : a) x *= 2;     // a == { 1, 2, 3, 4, 5 }
for (int& x : a) x *= 2;    // a == { 2, 4, 6, 8, 10 }
```

+ Trailing return types

An alternative syntax for specifying their return types.

```c++
int f() {
  return 123;
}
// is the same as
auto f() -> int {
  return 123;
}
```

Especially useful for the lambda expression or when certain return types cannot be resolved yet.

```c++
auto g = []() -> int {
  return 123;
};
template <typename T, typename U>
auto add(T a, U b) -> decltype(a + b) {
    return a + b;
}
```

+ `noexcept` specifier

It specifies whether a function could throw exceptions.

```c++
void func1() noexcept;        // does not throw
void func2() noexcept(true);  // does not throw
void func3() throw();         // does not throw
void func4() noexcept(false); // may throw
```

+ r-value references ???

+ New libraries
    * `std::chrono`
    * Unordered containers: `unordered_map`, `unordered_set



## Others ##

### Differences between copy constructor and copy assignment operator?

Both are used to copy objects, but the purpose and scenarios are different.

+ Copy constructor
  + to create a new object as a copy of an existing one, using its values
+ Copy assignment operator
  + an operator member function to assign the value of one object to another of the same class
```c++
class MyClass {
private:
  int num;
public:
  MyClass(const MyClass& other) {
    this->num = other.num;
  }
  MyClass& operator=(const MyClass& other) {
    if (this == &other) return *this;
    this->num = other.num;
    return *this;
  }
};
```

### Q1: What are class, struct, friend in C++?

### Q2: Difference between overload and override?

### stack vs. heap?

### new vs. malloc?

### how new/delete is implemented?

### c++ vs. java, e.g. garbage collection, application, language characteristics?
