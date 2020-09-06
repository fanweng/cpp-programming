# Keywords

## Access Specifiers

Access specifiers define how the members (attributes and methods) of a class can be accessed.

* `public`: members are accessible from outside the class.
* `private`: members cannot be accessed or viewed from outside the class.
* `protected`: members cannot be accessed from outside the class, however, they can be accessed in **inherited** classes.

## `virtual`

A `virtual` member function is declared within a base class and is overriden by a derived class. They are mainly used to achieve **Runtime Polymorphism** and the function resolution is done at runtime. Virtual functions should be accessed using pointer or reference of base class type to achieve runtime polymorphism.