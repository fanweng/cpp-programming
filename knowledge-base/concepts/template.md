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
