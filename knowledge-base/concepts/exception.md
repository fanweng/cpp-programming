# Exception Handling

## Basic concepts

Exception handling indicates the extraordinary situation has been detected or occurred, and program can deal with the situation in a user-defined manner. Examples are insufficient resources, invalid operators, range violations, underflows/overflows, illegal data, etc.

- Exception: an object signals an error has occurred
- Throw/Raise an exception: the place where the error occurred don't know how to handle it, code throw an exception describing the error to another part of code that knows how to handle the error
- Catch/Handle an exception: code handles the error, may or may not terminate the program

## Syntax

- `throw`
    * throws an exception, followed by an argument

- `try { code_that_may_throw_an_exception }`
    * contains the code to run that may throw an exception
    * once the code throws an exception, `try` block exits
    * the thrown exception is handled by a `catch` handler, if no catch handler defined, the program terminates

- `catch (Exceptoin ex) { exception_handling_code }`
    * contains the code to handle the exception
    * can have multiple `catch` handlers
    * may or may not cause the program to terminate

```c++
try {
    if (total == 0)             // try block
        throw 0;                // throw the exception, exit the try block
    average = sum / total;      // won't execute if total==0
}
catch (int &ex) {               // exception handler
    std::cerr << "Can't divide by zero!" << std::endl;
}
// program continues...
```