# Safety-Critical Systems

### Initialization with `{}`

Initialization with `{}` prohibits narrowing conversion (implicit conversion of arithmetic values).

```c++
int i1(3.14); // 3
int i2{3.14}; // error: narrowing conversion of ‘3.14’ from ‘double’ to ‘int’

class MyData {
public:
    MyData(std::initializer_list<int>) {
        std::cout << "constructor with initializer_list" << std::endl;
    }
};
MyData{1,2,3}; // use the constructor with initializer_list
```

### `auto` type deduction

Automatic type deduction is extremely convenient:
+ save unnecessary typing
+ compiler doesn't make human error
+ easy to refactor code if type information is not required

### Strongly-typed enumerations

In order to calculate or output the enumerators, we must convert explicitly the enumerators into integral types.

```c++
enum class NumBases: uint8_t { // default is int, if not specified uint8_t
    one=1,
    ten=10,
    hundred=100
};
std::cout << "201 = " << 2 * static_cast<uint8_t>(NumBases::hundred) +
                            0 * static_cast<uint8_t>(NumBases::ten) +
                            1 * static_cast<uint8_t>(NumBases::one);
```

### Use `nullptr`

Literal `0` can be either `(void *)0` as a null pointer, or `0` as a number.

Macro `NULL` is *implicitly* converted to `long int`, but on the other hand, it is a constant pointer.

```c++
template<class P>
void functionTemplate(P p) {
    int* a = p;
}

int main() {
    int* a = 0;
    int* b = NULL;
    int* c = nullptr;
    functionTemplate(0);          // error, invalid conversion of P='int' to 'int*'
    functionTemplate(NULL);       // error, invalid conversion of P='long int' to 'int*'
    functionTemplate(nullptr);    // ok

    bool d {nullptr};             // ok, d is false
    int e = nullptr;              // error, invalid conversion to 'int'
}
```

### User-defined literals

Due to automatic mapping of the user-defined literal to the literal operator, we can implement type-safe arithmetic.

```c++
namespace Distance {
class MyDistance {
public:
    MyDistance(double i) : m(i) {}

    friend MyDistance operator+(const MyDistance& a, const MyDistance& b) {
        return MyDistance(a.m + b.m);
    }
    friend MyDistance operator-(const MyDistance& a, const MyDistance& b) {
        return MyDistance(a.m - b.m);
    }
    friend std::ostream& operator<<(std::ostream &out, const MyDistance& myDist) {
        out << myDist.m << " m";
        return out;
    }
private:
    double m;
};

    namespace Unit {
        MyDistance operator"" _km(long double d) {
            return MyDistance(1000*d);
        }
        MyDistance operator"" _m(long double m) {
            return MyDistance(m);
        }
        MyDistance operator"" _cm(long double c) {
            return MyDistance(c/100);
        }
    }
}

using namespace Distance::Unit;
std::cout << "1.0_km + 2.0_m - 40.0_cm: " << 1.0_km + 2.0_m - 40.0_cm << std::endl; // 1001.6m
```

### Built-in literals

| Type                        | Example    |
|-----------------------------|------------|
| Binary                      | `0b10`     |
| `std::string`               | `"Hello"s` |
| `complex<double>`           | `5i`       |
| `complex<long double>`      | `5il`      |
| `complex<float>`            | `5if`      |
| `std::chrono::hour`         | `5h`       |
| `std::chrono::minutes`      | `5min`     |
| `std::chrono::seconds`      | `5s`       |
| `std::chrono::milliseconds` | `5ms`      |
| `std::chrono::microseconds` | `5us`      |
| `std::chrono::nanoseconds`  | `5ns`      |

### Assertions at compile time

`static_assert` evaluates constant expressions at compile-time, which allows to check if ported code on the new platform supports the requirements.

```c++
template<class T>
struct Add {
    static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");
};
Add<int> addInt = Add<int>();                   // ok
Add<std::string> addStr = Add<std::string>();   // assertion

static_assert(sizeof(void*) >= 8, "64-bit addressing is required");
```
