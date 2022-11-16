#keyword 

# Constant Expressions

`constexpr` specifier evaluates an object or function at compile time and the expression can be used in other constant expressions. The main idea is to improve the performance by doing the computation at compile time rather than run time.

## User-defined Type

Restrictions:
+ must not have **virtual methods** or a **virtual base class**
+ constructor must be empty and itself be a `constexpr`
+ methods, which should be callable at compile time, must be `constexpr`

```c++
class MyDouble{
  private:
    double myVal1;
    double myVal2;
  public:
    constexpr MyDouble(double v1,double v2):myVal1(v1),myVal2(v2){}
    constexpr double getSum() const { return myVal1 + myVal2; }
};

void main() {
  constexpr double myStatVal = 2.0;
  constexpr MyDouble myStatic(10.5, myStatVal);
  constexpr double sumStat= myStatic.getSum();
}
```

## Difference with `const`

`const` may be initialized at compile time or run time. Therefore, it cannot be used for initializing a compile-time constant.

```c++
const     double PI1 = 3.14;
constexpr double PI2 = 3.14;

constexpr double AREA1 = PI1 * 1^2; // error
constexpr double AREA2 = PI2 * 1^2; // OK
```

## Difference with `inline`

Both are for performance improvement. `inline` only expands the function at compile time which saves time of function call overhead, the expression is still evaluated at run time.
