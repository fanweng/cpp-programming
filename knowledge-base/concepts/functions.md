# Functions

## Function Parameters

+ Formal parameters
	+ variables defined in the function definition
	+ aka. **parameters**
+ Actual parameters
	+ variables passed to the function when it is called
	+ aka. **arguments**

## Function Overloading

Functions with different parameters (number of parameters or their types) can co-exist with the same function name.

## Callable Units

+ Functions
	+ is a block of code
+ [Function objects or Functors](../STL/components/functions-functors.md)
	+ is an instance of a class for which the call operator (`operator()`) is overloaded
		+ is an object that behaves like a function, but having a *state*
	+ [lambda](lambda.md) functions
		+ are function objects implicitly created by the compiler
