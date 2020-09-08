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