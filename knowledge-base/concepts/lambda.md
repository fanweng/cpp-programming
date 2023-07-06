#lambda

# Lambda Expression

> A lambda function, or lambda, is a function without a name.

```c++
[ capture clause ] (parameters) -> optional_return_type  
{   
   definition of method   
} 
```

+ A lambda
	+ should be short and concise
	+ should be self-explanatory, because it doesn't have a name
	+ cannot be overloaded

## Capture Clause

Lambda expression can access to variables from the enclosing scope:
+ capture nothing: `[]`
+ capture by reference
	+ `[&]`: capture all external variables by reference
	+ `[&a]`: capture variable `a` by reference
+ capture by value
	+ `[=]`: capture all external variables by value
	+ `[a]`: capture variable `a` by value
+ mixed capture
	+ `[a, &b]`: capture `a` by value, capture `b` by reference
	+ `[=, &a]`: default capture by value, only capture `a` by reference
	+ `[&, a]`: default capture by reference, only capture `a` by value
+ capture the enclosing class by value
	+ `[this]`: can access the member variables/functions of the enclosing class
