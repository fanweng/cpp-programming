# Functions

The STL includes classes that overload the function call operators. Instances of such classes are *function objects* or *functors*.

## Callable Units

- functions
- function objects
    + user-defined
    + predefined: `std::less<T>()`, `std::bit_and<T>()`,  `std::logical_and<T>()`, `std::multiplies<T>()`, etc.
- [lambda](../../concepts/lambda.md) functions

```c++
// Function
void square1(int& i) { i = i * i; }
// Function object
struct square2 {
    void operator()(int& i) { i = i * i; }
};
std::vector<int> v {1,2,3};
std::for_each(v.begin(), v.end(), square1);     // 1,4,9
std::for_each(v.begin(), v.end(), square2());   // 1,16,81
// Lambda function <- preferred
std::for_each(v.begin(), v.end(), [](int& i){i=i*i;}); // 1,256,6561
```

## `std::bind()` and `std::function()`

```c++
/* bind() enables to create new function objects
 * function() takes these temporary function objects and binds them to a variable
 */
double divideMe(double a, double b) {
    return a/b;
}
// _1 is assigned to a, _2 is assigned to b
std::function<double(double, double)> myDiv1 = std::bind(divideMe, std::placeholders::_1, std::placeholders::_2);
// 20 is assigned to a, _1 is assigned to b
std::function<double(double)> myDiv2 = std::bind(divideMe, 20, std::placeholders::_1);
// divideMe(20, 2) == myDiv1(20, 2) == myDiv2(2)
```
