#concurrency 

# Concurrency

C++11 introduced multithreading by supporting the well-defined memory model and a standardized threading interface.

C++14 added reader-writer locks to the multithreading facilities.

C++17 makes most of the STL algorithms available in a parallel implementation.

C++20 introduces atomic smart pointers, extensions of futures, coroutines, transactional memory, and task blocks.

## Memory Model

[Memory Model](memory-model/memory-model.md)

+ Atomic operations
    - operations can be performed without interruption
+ Partial ordering of operations
    - the sequence of operations that must not be reordered
+ Visible effects of operations
    - guarantee when operations on shared variables are visible in other threads

## Multithreading Interfaces

### 1. Threads

A [thread](multithreading/threads.md) `std::thread` is an independent unit of program execution. Its work package, a *callable unit*, can be:
+ a named function
+ a function object
+ a [lambda](../concepts/lambda.md) function

It is a variadic template, which can receive an arbitrary number of arguments by copy or reference.

### 2. Synchronization Primitives for Shared Data

[Shared data](multithreading/shared-data.md): Developers have to coordinate the access to a shared variable if more than one thread is using it at the same time and the variable is mutable.

### 3. Thread-local data

Declaring a variable as [thread-local](multithreading/thread-local-data.md) ensures that each thread gets its own copy, no-sharing.

### 4. Synchronization Mechanisms for Threads

#### Condition variables

[Condition variables](multithreading/condvar.md) enable threads to be synchronized via **messages**.

### 5. Tasks

Also known as *Promises*. [Task](multithreading/task.md) provides a higher level of abstraction than native threads. While explicitly creating a thread, a task is simply a job you start. C++ runtime will automatically handle the lifetime of the task.

In most cases, tasks is a less error-prone way to synchronize threads.

## Best Practices

### General

+ Minimize the data sharing of mutable data
	+ performance increasing
		+ Amdahl's law
	+ safety on data races
+ Minimize waiting
	+ performance
	+ chance of deadlock
+ Prefer immutable data initialized in a thread-safe way
+ Prefer local variables, or thread-local data to minimize the synchronization cost
+ Tools
	+ Static  code analysis: *[CppMem](cppmem.md)*, etc
	+ Dynamic enforcement: *ThreadSanitizer*, etc
+ Code review

### For Memory Model

+ Don't use `volatile` for synchronization
+ Don't program lock-free
	+ error-prone if not an expert
+ If have to be lock-free
	+ sharing an atomic Boolean or an atomic counter
	+ using a thread-safe or lock-free container to support consumer/producer scenario
+ Don't build your own abstraction, use guarantees of the language

### For Threads

+ Use task instead of thread
+ Be extremely careful if to `detach()`
	+ the lifetime of the object in main thread and detached thread
+ Use an atomic joining thread
	+ create your own wrapper around `std::thread`
		+ check in the constructor
		+ join in the destructor

### For Shared Data

+ Pass data per default by copy
	+ pass by reference is error-prone and expensive in terms of synchronization
+ Minimize the time holding a lock
+ Put a mutex into a lock, instead of using a primitive mutex
+ Use `std::lock` or `std::scoped_lock` for locking more mutexes atomically
+ Never call unknown code while holding a lock

### For Condition Variables

+ Don't use condvar without a predicate
+ Better to use promise/future instead of condvar
