# Numeric Functions

## I. Random Numbers Library `<random>`

1. Random number generator
2. Distribution of the generated random numbers

```c++
std::random_device seed;
std::mt19937 generator(seed());
generator.min(); // get the min random number
generator.max(); // get the max random number

std::uniform_int_distribution<> unDis(0, 20);   // distribution between 0 to 20
int myNum = unDis(generator);   // generate a random number
```

## II. Math Function Library

Logarithmic, exponential, trigonometric functions, etc.
