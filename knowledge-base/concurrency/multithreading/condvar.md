# Condition Variables

`std::condition_variable` enables synchronization via messages:
+ one thread acts as a sender
+ the other as a receiver of the message, waiting for the notification
+ use cases
	+ sender-receiver
	+ producer-consumer
	+ ...
+ interfaces
	+ `cv.notify_one()`
		+ wake up one of the waiting thread (not specified which one)
	+ `cv.notify_all()`
	+ `cv.wait(lock, optional_predicate)`
		+ when *blocked waiting*, lock is **released**
		+ when *waken up*, lock is **acquired**
		+ checks if predicate evaluates to true in a while loop
			+ `while(!predicate()){ wait(lock); }`
	+ ...

```c++
std::mutex mtx;
std::condition_variable condVar;
bool dataReady{false};

void waitingForWork() {
	std::cout << "Worker: waiting for work" << std::endl;
	std::unique_lock<std::mutex> lk(mtx);
	condVar.wait(lk, []{ return dataReady; });
	// do work
	std::cout << "Worker: done work" << std::endl;
}

void setDataReady() {
	{
		std::lock_guard<std::mutex> lk(mtx);
		dataReady = true;
	}
	std::cout << "Sender: data is ready" << std::endl;
	condVar.notify_one();
}

int main() {
	std::thread t1(waitingForWork);
	std::thread t2(setDataReady);
	t1.join();
	t2.join();
	return 0;
}
```

## Caveats

> Solution is using the optional predicate when `wait()`

### 1. Lost Wakeup

Sender sends the notification before the receiver gets to a wait state... Notification gets lost... Receiver will wait forever.

### 2. Spurious Wakeup

Receiver wakes up although no notification happens, at least in POSIX Threads and the Windows API.
