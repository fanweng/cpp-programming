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