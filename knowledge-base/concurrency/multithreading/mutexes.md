# Mutexes

A mutex (mutual exclusion) guarantees that only one thread can access the **critical section** at any given time.

> `std::cout`, `std::cin`, `std::cerr`, `std::clog` are thread-safe. They are not a *data race*, but they can be *race condition* in which the sequence of prints are undefined

## Types

C++14:
1. `mutex`
2. `recursive_mutex`
3. `timed_mutex`
4. `recursive_timed_mutex`
5. `shared_timed_mutex`

## Methods

1. `m.lock()`
2. `m.unlock()`
3. `m.try_lock()`
4. ... ...

## Deadlock

Two or more threads are blocked because each other waits for the release of a resource before it can release its own resource.

```c++
struct CriticalData {
	std::mutex mtx;
};

void deadlockFunc(CriticalData& a, CriticalData& b) {
	a.mtx.lock();
	std::cout << "Get the first mutex" << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(1));

	b.mtx.lock();
	std::cout << "Get the second mutex" << std::endl;

	// processing a and b
	a.mtx.unlock();
	b.mtx.unlock();
}

void main() {
	CriticalData c1;
	CriticalData c2;
	std::thread t1([&]{deadlock(c1, c2);});
	std::thread t2([&]{deadlock(c2, c1);});
	t1.join();
	t2.join();
}
```

## Best Practices

```c++
std::mutex m;
m.lock();
sharedVariable = getVar(); // <= prone to deadlock
m.unlock()
```

+ Avoid calling function while holding a lock
	+ if function throws an exception, the mutex will not be released after the function
	+ if function is from a library getting updated, or function is rewritten, there is a risk of a deadlock
+ Never calling an unknown function while holding a lock
	+ if function tries to lock mutex again and the mutex is not a recursive mutex, the behavior is undefined - mostly result in a deadlock
