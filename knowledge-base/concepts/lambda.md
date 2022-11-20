#lambda

# Lambda Expression

```c++
[ capture clause ] (parameters) -> return_type  
{   
   definition of method   
} 
```

## Capture Clause

Lambda expression can access to variables from the enclosing scope:
+ capture by reference
	+ `[&]`: capture all external variables by reference
+ capture by value
	+ `[=]`: capture all external variables by value
+ mixed capture
	+ `[a, &b]`: capture `a` by value, capture `b` by reference
