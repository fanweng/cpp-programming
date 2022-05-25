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
