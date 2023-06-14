#concurrency/lock 

# Locks

A lock automatically binds its [mutexes](mutexes.md) in the constructor and release it in the destructor. A lock implements the RAII idiom by binding a mutex's lifetime to its own.

It considerably reduces the risk of deadlock because the runtime takes care of the mutex.

## `std::lock_guard`

```c++
{
  std::mutex m,
  std::lock_guard<std::mutex> lockGuard(m);
  sharedVariable = getVar();
}
```

+ lifetime of `std::lock_guard` ends when passing the closing curly bracket
	+ its destructor is called, and mutex is released
+ if `getVar()` throws an exception, `std::lock_guard` also releases the mutex

> **Note**
> 1. ideal for the usage that only needs to lock/unlock once.
> 2. synchronization is slower than [atomic](../memory-model/atomics.md)

## `std::unique_lock`

In addition to `std::lock_guard`, `std::unique_lock` offers:
+ create without an associated mutex
+ create without locking the associated mutex
+ explicitly and repeatedly set/release the lock of the associated mutex
	+ `lk.lock()`
	+ `std::lock(lk1, lk2, ...)`
	+ `lk.release()`
+ move the mutex
	+ `lk1.swap(lk2)`
	+ `std::swap(lk1, lk2)`
+ try to lock the mutex
	+ `lk.try_lock()`
+ delay the lock on the associated mutex

> ideal for the usage that needs frequently lock/unlock operations.

### Solution to Deadlock

The `deadlockFunc()` in the [mutexes](mutexes.md) deadlock example can be revised to:

```c++
void deadlockFunc(CriticalData& a, CriticalData& b) {
  unique_lock<mutex> guard1(a.mtx, defer_lock); // not locked immediatedly
  cout << "Thread: " << this_thread::get_id() << " first mutex" << endl;

  this_thread::sleep_for(chrono::milliseconds(1));

  unique_lock<mutex> guard2(b.mtx, defer_lock); // not locked immediatedly
  cout << "  Thread: " << this_thread::get_id() << " second mutex" <<  endl;

  cout << "    Thread: " << this_thread::get_id() << " get both mutexes" << endl;

  lock(guard1, guard2); // lock all in one atomic step
  // do something with a and b
} // out of scope, two mutexes get released
```

## `std::shared_lock`

`std::shared_lock` has the same interfaces as a `std::unique_lock`, but behaves differently with a `std::shared_timed_mutex`.

Many threads can share one `std::shared_timed_mutex` and therefore it can implement a **reader-writer lock**, i.e. many threads can read on the critical section at the same time but only one thread is allowed to write.

```c++
std::map<std::string, int> teleBook{{"amy", 1911}, {"ben", 1832}, {"clark", 2028}};
std::shared_timed_mutex teleBookMutex;

void addToTeleBook(const std::string& name, int tel) {
	// this lock enforces the exclusive access for writing operation
	std::lock_guard<std::shared_timed_mutex> writerLock(teleBookMutex);
	std::cout << "\nSTARTING UPDATE " << name;
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	teleBook[name]= tel;
	std::cout << " ... ENDING UPDATE" << std::endl;
}

void printNumber(const std::string& name) {
	// this lock allows sharing the critical section for reading operation
	std::shared_lock<std::shared_timed_mutex> readerLock(teleBookMutex);
	std::cout << name << ": " << teleBook[name] << std::endl;
}

void main() {
	std::thread r1([]{ printNumber("ben"); });
	std::thread r2([]{ printNumber("amy"); });
	std::thread w1([]{ addToTeleBook("david", 1380); });

	// this may have data race with w1
	// r3 could read before w1 writes the data, e.g. print out "david: 0"
	std::thread r3([]{ printNumber("david"); });
	r1.join();
	r2.join();
	r3.join();
	w1.join();
}
```
