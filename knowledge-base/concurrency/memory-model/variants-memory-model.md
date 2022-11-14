# Variants of Memory Models

Synchronization and ordering constraints in C++:

```c++
enum memory_order {
    // relaxed
    memory_order_relaxed,   // no sync and ordering constraints

    // acquire-release
    memory_order_consume,   // read operation
    memory_order_acquire,   // read
    memory_order_release,   // write
    memory_order_acq_rel,   // read-modify-write

    // sequential consistency
    memory_order_seq_cst    // read-modify-write, default atomic operation
}
```

## Sequential Consistency

+ [Sequential Consistency](sequential-consistency.md)

## Acquire Release Semantic

+ [Acquire Release Semantic](acquire-release-semantic.md)

## Relaxed Semantic

+ [Relaxed Semantic](relaxed-semantic.md)
