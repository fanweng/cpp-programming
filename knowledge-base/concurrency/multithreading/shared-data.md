#concurrency 

# Shared Data

Shared/mutable data across multiple threads is prone to **data races**.

## Mutexes

A [mutex](mutexes.md) (mutual exclusion) guarantees that only one thread can access a shared variable at any given time.

## Locks

Mutex is encapsulated in a [lock](locks.md) to release the mutex automatically. A lock implements the RAII idiom by binding a mutex's lifetime to its own.

## Thread-safe initialization of data

If shared data is read-only, never modified, it's sufficient to initialize it in a [thread-safe](thread-safe-initialization) way.
