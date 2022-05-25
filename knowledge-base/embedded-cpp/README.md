# Embedded C++ Programming

## Specialties in Embedded Programming

+ high performance requirements up to real-time
+ [safety-critical systems](./safety-critical-system.md)
+ reduced resources as RAM and CPU power
+ more parallel tasks

## Industry Guidelines

+ MISRA C++
+ AUTOSAR C++
+ C++ Core Guidelines

## Performance Report

#### Technical Report on C++ Performance (2006)

> modern C++ features are not accounted for

+ Namespaces
    - **no** significant overhead size and performance
+ Type converting
    - `const_cast`, `static_cast`, `reinterpret_cast` are **equivalent** to C predecessor in size and performance
    - `dynamic_cast` in runtime has **some** overhead, no similar C predecessor
+ Inheritance
    - Class
        * **equivalent** to `struct` if without virtual functions
        * has overhead of a **pointer** and a **virtual function table** if having virtual functions
    - Function calls
        * non-virtual, non-static, non-inline functions are **equivalent** to free function calls
        * virtual function is **equivalent** to a free function with the help of a pointer stored in a table
        * virtual functions of a class template have **overhead** in size
        * inline function has **significant benefits** in performance, similar to C macros
    - Multiple inheritance
        * **overhead** in time and/or space
+ Runtime type information (RTTI)
    - **additional** 40 bytes for each class
    - `typeid` call is **quite slow**
    - `dynamic_cast` is **slow**
+ Exception handling
    - code strategy
        * move and manage **additional** data structures for dealing with exceptions
        * has **overhead** in size for stack and in runtime even without throwing an exception
    - table strategy
        * has execution context in a table
        * **no** overhead in size nor in runtime if no exception was thrown
        * hard to implement
+ Templates
    - naive usage can cause **code bloat**
    - modern compiler can massively reduces the number of template instantiations
