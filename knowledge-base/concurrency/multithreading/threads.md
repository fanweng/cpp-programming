#concurrency/thread 

# Threads

## Callable Unit

A `std::thread` is an executable unit. Its work package, a *callable unit*, can be:
+ a named function
+ a function object
+ a lambda function

```c++
void helloFunction(){
  std::cout << "Hello from a function." << std::endl;
}

class HelloFunctionObject{
  public:
    void operator()() const {
      std::cout << "Hello from a function object." << std::endl;
    }
};
  
int main(){
  std::thread t1(helloFunction);
  
  HelloFunctionObject helloFunctionObject;
  std::thread t2(helloFunctionObject);

  std::thread t3([]{std::cout << "Hello from a lambda." << std::endl;});

  t1.join();
  t2.join();
  t3.join();
}
```

## Thread Lifetime

### Child Thread Management

Parent has two choices:
+ wait until its child is done with `t.join()`
	+ subsequent code relies on the result from child threads
+ detach itself from its child with `t.detach()`
	+ parent/child can run independently

Two exceptions:
+ `std::terminate`
	+ joinable thread is destroyed
		+ **joinable:** thread before `t.join()` or `t.detach()` is called
+ `std::system_error`
	+ invoke `t.join()` or `t.detach()` more than once on a thread

## Passing Arguments to Threads

`std::thread` is a variadic template that takes arbitrary number of arguments, three ways:
+ by copy
+ by move
+ by reference
	+ use a reference wrapper: `std::ref(obj)`
	+ **Be careful about the lifetime of the object**, otherwise it may result undefined behavior

```c++
void transferMoney(int amount, Account& from, Account& to) {}
std::thread t1(transferMoney, 50, std::ref(account1), std::ref(account2));
```
