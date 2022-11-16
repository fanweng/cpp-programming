#concurrency/memory-model 

# Memory Model

The foundation of multithreading is a well-defined memory model.

The **Contract** is between programmer and the system:
+ stronger the contract, the few liberties for the system to generate an optimized executable
+ from strong to weak
	+ Single threading (one control flow) ->
	+ Multithreading (tasks > threads > condition variables) ->
	+ Atomic (sequential consistency > acquire-release semantic > relaxed semantic)

## Atomics

[Atomics](atomics.md)

## Variants of Memory Model

[Variants of Memory Model](variants-memory-model.md)

Stricter the memory model is, less efficient the program will be:
+ Read operation: `memory_order_acquire` and `memory_order_consume`
+ Write operation: `memory_order_release`
+ Read-modify-write operation: `memory_order_acq_rel` and `memory_order_seq_cst`

`memory_order_relaxed` defines no synchronization and ordering constraints.

## Fences as Memory Barriers

[Memory Barrier](memory-barriers.md)

+ Full fence
+ Acquire fence
+ Release fence
