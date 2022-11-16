#concurrency/memory-model 

# Acquire Release Semantic

There is no global synchronization between threads:
+ only a synchronization between atomic operations on the same atomic variable
	+ a write operation on one thread synchronizes with a read operation on another thread, on the same atomic variable
+ acquire operations
	+ reading of an atomic variable 
		+ `load()`
		+ `test_and_set()`
	+ acquire of a lock
	+ creation of a thread
	+ waiting on a condition variable
+ release operations
	+ `store()` and `clear()` on an atomic variable
	+ release a lock
	+ `join()` call on a thread
	+ notification of a condition variable

## Transitive

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
