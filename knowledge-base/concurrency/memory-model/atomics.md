#concurrency/memory-model/atomic 

# Atomics

## Memory model

[Variants of Memory Model](variants-memory-model.md)

1. Strong Memory Model - Sequential Consistency (Default)
+ the instructions of a program are executed in source code order
+ there is a *global order* of all operations on **all threads**

2. Weak Memory Model - Relaxed Semantic
+ only guarantee that operations on one specific data type in the **same thread** cannot be reordered

3. Between Strong and Weak - Acquire-release Semantic
+ establish an ordering between *read* and *write* operations on the same atomic variable with **different threads**

## Operations

https://en.cppreference.com/w/cpp/atomic

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
