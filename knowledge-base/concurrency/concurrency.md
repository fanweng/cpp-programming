# Concurrency

C++11 introduced multithreading by supporting the well-defined memory model and a standardized threading interface.

C++14 added reader-writer locks to the multithreading facilities.

C++17 makes most of the STL algorithms available in a parallel implementation.

C++20 introduces atomic smart pointers, extensions of futures, coroutines, transactional memory, and task blocks.

### Memory Model

[Memory Model](memory-model/memory-model.md)

+ Atomic operations
    - operations can be performed without interruption
+ Partial ordering of operations
    - the sequence of operations that must not be reordered
+ Visible effects of operations
    - guarantee when operations on shared variables are visible in other threads

### Fundamentals

#### Threads

A `std::thread` is an independent unit of program execution. Its work package, a *callable unit*, can be:
+ a named function
+ a function object
+ a lambda function

It is a variadic template, which can receive an arbitrary number of arguments by copy or reference.

#### Shared data

Developers have to coordinate the access to a shared variable if more than one thread is using it at the same time and the variable is mutable.

#### Mutexes

A mutex (mutual exclusion) guarantees that only one thread can access a shared variable at any given time.

#### Locks

Mutex is encapsulated in a lock to release the mutex automatically. A lock implements the RAII idiom by binding a mutex's lifetime to its own.

#### Thread-safe initialization of data

If shared data is read-only, it's sufficient to initialize it in a thread-safe way.

#### Thread local data

Declaring a variable as thread-local ensures that each thread gets its own copy, no-sharing.

#### Condition variables

Condition variables enable threads to be synchronized via messages.

#### Tasks

Task has a lot in common with a thread. While explicitly creating a thread, a task is simply a job you start. C++ runtime will automatically handle the lifetime of the task.