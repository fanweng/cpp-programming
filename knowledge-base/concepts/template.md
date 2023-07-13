# Template

Template is a powerful tool which idea is to pass data type as a parameter so that we don't need to write the same code for different data type.

Templates are expanded at compiler time, similar to macros, but compiler does type checking before template expansion.

Keywords: `template`, `typename`/`class`

## Function Templates

> A generic function can be used for different data types.

### Instantiation

> The process of substituting the template parameters for the template arguments.

The compiler automatically:
+ create an instance of the function template
+ create a function template if the template parameters can be derived from the function arguments
+ if compiler can't deduce the template arguments, we have to specify them explicitly

```c++
template <typename T>
T myMax(T x, T y) {
    return (x > y)? x : y;
} // function won't be compiled at here, because T is unknown

cout << myMax<int>(3, 7) << endl;   // now compiler can generate appropriate function becuase T is int
cout << myMax(1.2, 3.8) << endl;    // often, compiler can deduce the T according to the inputs
```

### Overloading

```c++
void xchg(int& x, int& y) {...} // 1

template <typename T>
void xchg(T& x, T& y) {...} // 2

template <typename T>
void xchg(T& x, T& y, T& z) {...} // 3

int intA = 5, intB = 10, intC = 20;
double doubleA = 1.3, doubleB = 32.9;

xchg(intA, intB); // 1

xchg(doubleA, doubleB);   // 2
xchg<>(intA, intB);       // 2
xchg<int>(intA, intB);    // 2
xchg<double>(intA, intB); // error

xchg(intA, intB, intC); // 3
```

## Class Templates

If the class definition is independent of the data type, i.e. allowing *plug-in* any data type.

### Instantiation

> In contrast to a function template, class template can't deduce the template parameters. Each argument must be **explicitly** specified. (before C++17)

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

### Method Templates

Method templates can be defined inside or outside the class.

```c++
// inside
template <class T, int N>
class Array {
public:
	template <class T2>
	Array<T, N>& operator=(const Array<T2, N>& a);
};

// outside
template <class T, int N> // need to repeat the class template declaration for each method function
template <class T2>
Array<T, N>& Array<T, N>::operator=(const Array<T2, N>& a);
```

> **Destructor** and **Copy Constructor** cannot be templates.

### Inheritance

Derived class template cannot use the methods of the base class template directly.

```c++
template <typename T>
struct Base {
    void func() {...}
};

template <typename T>
struct Derived : Base<T> {
    // using Base<T>::func;
    void func2() {
        func(); // error, if not introducing name "using Base<T>::func"
        this->func(); // OK
        Base<T>::func(); // OK
    }
};
```

## Alias Templates

Alisa template is also known as template typedefs.

```c++
template <typename T, int Line, int Col>
class Matrix {...};

template <typename T, int Line>
using Square = Matrix<T, Line, Line>;

template <typename T, int Line>
using Vector = Matrix<T, Line, 1>;
```

## Variadic Templates

Variadic templates can have an arbitrary number of parameters by using parameter pack.

```c++
template <typename ... Args>
void variadicTemplate(Args ... args) {
	// function body
}
```

### Parameter Pack

+ By using the ellipse `...`
	+ `Args` become a template parameter pack
	+ `args` become a function parameter pack
+ Parameter packs can only be packed and unpacked
	+ if `...` is left from `Args`, the parameter pack will be **packed**
	+ if `...` is right from `Args`, the parameter pack will be **unpacked**

## Friend

Friends of a class template have unrestricted access to all members of that class template:
+ declaration of friendship can be made at an arbitrary place in the class template
+ access right in the class template have no influence
+ friendship cannot be inherited
+ friendship is not transitive

### General Friends

> The `typename` (`T`) of the class template should be different from the `typename` (`U`) of the friend template. If they are the same, the friendship is only granted for the instances of the same types.

```c++
template <typename T> int myFriendFunction(T);
template <typename T> class MyFriend;

template <typename T>
class GrantFriendshipClassTemplate {
	template <typename U> friend int myFriendFunction(U);
	template <typename U> friend class MyFriend;
};
```

### Special Friends

Friendship is limited to specific type of the template parameter.

```c++
template <typename T> int myFriendFunction(T);
template <typename T> class MyFriend;

template <typename T>
class GrantFriendshipClassTemplate {
	friend int myFriendFunction<>(double);
	friend class MyFriend<int>;
	friend class MyFriend<T>;
};
```

### Friend to Types

The friendship can be granted to a type parameter.

```c++
template <typename T>
class Array {
    friend T;
    // ...
};
Array<Account> myAccounts;
```

## Dependent Names

A dependent name is essentially a name that depends on a template parameter. It can be:
+ a type
+ a non-type
+ a template

Two-phase name lookup:
+ dependent names are resolved during *template instantiation*
+ non-dependent names are resolved during *template definition*

### ... When it is a Type

```c++
template <typename T>
void func() {
    std::vector<T>::const_iterator* p1; // error, * is interpreted as multiply
    typename std::vector<T>::const_iterator* p2; // OK
}
```

### ... When it is a Template

```c++
template <typename T>
struct S {
    template <typename U>
    void func() {}
};

template <typename T>
void func2() {
    S<T> s;
    s.func<T>(); // error, < is interpreted as a comparison operator
    s.template func<T>(); // OK
}
```
