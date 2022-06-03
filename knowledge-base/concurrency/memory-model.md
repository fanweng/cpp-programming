# Memory Model

Contract: Strong -> Weak

Single threading (one control flow) -> Multithreading (tasks > threads > condition variables) -> Atomic (sequential consistency > acquire-release semantic > relaxed semantic)

## Atomic

### Memory model

1. Strong Memory Model - Sequential Consistency
+ the instructions of a program are executed in source code order
+ there is a *global order* of all operations on **all threads**

2. Weak Memory Model - Relaxed Semantic
+ only guarantee that operations on one specific data type in the **same thread** cannot be reordered

3. Between Strong and Weak - Acquire-release Semantic
+ establish an ordering between *read* and *write* operations on the same atomic variable with **different threads**

### `std::atomic_flag`

+ Interfaces
    - `clear()`: set the value to `false`
    - `test_and_set()`: set the value to `true`
+ The **only** lock-free atomic
    - a non-blocking algorithm is lock-free if there is guaranteed system-wide progress
    - other atomics use *mutex* internally
+ It's the building block for higher level thread abstractions

Example of a spin lock implemented by `std::atomic_flag`:

```c++
class Spinlock{
private:
    std::atomic_flag flag;
public:
    Spinlock() : flag(ATOMIC_FLAG_INIT){}
    void lock() {
        while( flag.test_and_set() );
    }
    void unlock() {
        flag.clear();
    }
};
Spinlock spin;

void workOnResource() {
    spin.lock();
    // shared resource
    spin.unlock();
    std::cout << "Work done" << std::endl;
}

int main() {
    std::thread t(workOnResource);
    std::thread t2(workOnResource);
    t.join();
    t2.join();
}

```

### `std::atomic`

> **volatile**: the object won't be allowed to have reading/writing operation optimization
> **atomic**: the object is meant for a thread-safe reading/writing

Example of a kind of condition variable implemented by `std::atomic`:

```c++
std::vector<int> mySharedWork;
std::atomic<bool> dataReady(false);

void waitingForWork() {
    std::cout << "Waiting " << std::endl;
    while (!dataReady.load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    mySharedWork[1] = 2;
    std::cout << "Work done " << std::endl;
}

void setDataReady() {
    mySharedWork = {1, 0, 3};
    dataReady = true;
    std::cout << "Data prepared" << std::endl;
}

int main() {
    std::thread t1(waitingForWork);
    std::thread t2(setDataReady);
    t1.join();
    t2.join();
}
// Waiting -> Data prepared -> Work done -> mySharedWork={1,2,3}
```

## Variants of Memory Model

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

The acquire-release semantic is transitive, which means if acquire-release semantics between threads (a,b) and threads (b,c), we get an acquire-release semantic between (a,c).

```c++
std::vector<int> mySharedWork;
std::atomic<bool> dataProduced(false);
std::atomic<bool> dataConsumed(false);

void dataProducer() {
    mySharedWork = {1,0,3};
    dataProduced.store(true, std::memory_order_release);
}

void deliveryBoy(){
    while(!dataProduced.load(std::memory_order_acquire));
    dataConsumed.store(true, std::memory_order_release);
}

void dataConsumer(){
    while(!dataConsumed.load(std::memory_order_acquire));
    mySharedWork[1] = 2;
}

int main() {
  std::thread t1(dataConsumer);
  std::thread t2(deliveryBoy);
  std::thread t3(dataProducer);
  t1.join();
  t2.join();
  t3.join();
  return 0;
}
