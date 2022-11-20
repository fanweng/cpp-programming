#concurrency/task

# Tasks

Tasks behave like data channels between communication endpoints:
+ one endpoint is *promise: sender*
	+ put its result in the data channel
+ the other endpoint is *future: receiver*
	+ wait for the result and pick it up
+ these endpoints can exist in the **same** or in **different** threads

## Threads vs. Tasks

| **Criteria** | **[Threads](threads.md)** | **Tasks** |
|--------------|-------------|-----------|
| Participants | creator and child thread | promise and future |
| Communication | shared variable | communication channel (implicitly created), must NOT use `mutex` |
| Thread creation | obligatory | optional |
| Synchronization | via `join()` (waits) | `get()` call blocks |
| Exception in child thread | child and creator threads terminates | return value of the promise |
| Kinds of communication | values | values, notifications, and exceptions |

## `std::async()`

Behave like an asynchronous function call:
+ takes a callable
+ variadic template that allows arbitrary number of arguments
+ return a `future` object `fut`
	+ the handle for getting the result `fut.get()`

```c++
auto fut = std::async([]{ return 2000 + 11; });
std::cout << "fut.get(): " << fut.get() << std::endl;
```

C++ runtime decides if `std::async` is executed in a separate thread or not:
+ depending on
	+ available CPU cores
	+ utilization of system
	+ size of work package
+ runtime will automatically manage the lifetime of separate thread

### Start Policy

```c++
auto fut1 = std::async(std::launch::deferred, ... ); // evaluated only if needed
auto fut2 = std::async(std::launch::async, ... ); // evaluated immediately
```

## `std::packaged_task`

A wrapper for a callable in order for it to be invoked asynchronously.

```c++
class SumUp{
  public:
    int operator()(int beg, int end){
      long long int sum{0};
      for (int i = beg; i < end; ++i ) sum += i;
      return sum;
    }
};

int main(){
  SumUp sumUp1;
  SumUp sumUp2;

  // Step 1: wrap the tasks
  std::packaged_task<int(int, int)> sumTask1(sumUp1);
  std::packaged_task<int(int, int)> sumTask2(sumUp2);

  // Step 2: create the futures
  std::future<int> sumResult1 = sumTask1.get_future();
  auto sumResult2 = sumTask2.get_future();

  // push the tasks on the container
  std::deque<std::packaged_task<int(int,int)>> allTasks;
  allTasks.push_back(std::move(sumTask1));
  allTasks.push_back(std::move(sumTask2));

  int begin{1};
  int increment{2500};
  int end = begin + increment;  

  // Step 3: perform each calculation in a separate thread
  while (not allTasks.empty()){
    std::packaged_task<int(int, int)> myTask = std::move(allTasks.front());
    allTasks.pop_front();
    std::thread sumThread(std::move(myTask), begin, end);
    begin = end;
    end += increment;
    sumThread.detach();
  }

  // Step 4: pick up the results
  auto sum = sumResult1.get() + sumResult2.get();
}
```

> NOTE: `std::packaged_task` object is not copyable, same for all promise/futures/thread except the `std::shared_future`.

## Promises and Futures

A `std::promise` can `set_value()`, `set_exception()`, or simply a notification into the shared data channel.

A `std::future` can `get()` the information from the promise.

```c++
struct Div {
	void operator()(std::promise<int>&& intPromise, int a, int b) {
		try {
			if (b == 0) {
				std::string errMsg = "Illegal division by zero";
				throw std::runtime_error(errMsg);
			}
			intPromise.set_value(a/b);
		}
		catch (...) {
			intPromise.set_exception(std::current_exception());
		}
	}
};

void runDivision(int nom, int denom) {
	// define the promise
	std::promise<int> divPromise;
	// get the future
	std::future<int> divResult = divPromise.get_future();
	
	// calcualte the result in a separate thread
	Div div;
	std::thread prodThread(div, std::move(divPromise), nom, denom);
	
	// get the result or exception
	try {
		std::cout << divResult.get() << std::endl;
	}
	catch {
		std::cout << e.what() << std::endl;
	}

	divThread.join();
}

void main() {
	runDivision(10, 0);
	runDivision(10, 2);
}
```

### Condvar vs. Task

+ [Condition Variable](condvar.md)
	+ can synchronize threads multiple times
		+ difficult if only needing to sync once
	+ has critical section
	+ no error handling in the receiver
	+ error-prone to spurious wakeup and lost wakeup
+ Task
	+ send notification only once
	+ no critical section
	+ has error handling in the receiver
	+ no lock, thus isn't prone to spurious wakeup and lost wakeup

```c++
void doTheWork() {
  std::cout << "Processing shared data." << std::endl;
}

void waitingForWork(std::future<void>&& fut) {
    std::cout << "Worker: Waiting for work." << std::endl;
    fut.wait();
    doTheWork();
    std::cout << "Work done." << std::endl;
}

void setDataReady(std::promise<void>&& prom){
    std::cout << "Sender: Data is ready."  << std::endl;
    prom.set_value();
}

void main() {
  std::promise<void> sendReady;
  auto fut = sendReady.get_future();

  std::thread t1(waitingForWork, std::move(fut));
  std::thread t2(setDataReady, std::move(sendReady));

  t1.join();
  t2.join();
}
```

## `std::shared_future`

The future can create a `std::shared_future` by using `fut.share()`, which enables you to query the promise independently from other associated futures.

```c++
std::mutex mtx;

struct Div {
	void operator() (std::promise<int>&& intPromise, int a, int b) {
		intPromise.set_value(a/b);
	}
};

struct Requestor {
	void operator() (std::shared_future<int> shareFut) {
		// lock std::cout
		std::lock_guard<std::mutex> coutGuard(mtx);
		// get the thread id
		std::cout << "threadID: " << std::this_thread::get_id() << " result: " << sharedFut.get() << std::end;
	}
};

void main() {
	std::promise<int> divPromise:
	std::shared_future<int> divResult = divPromise.get_future();
	
	Div div;
	std::thread divThread(div, std::move(divPromise), 10, 2);
	
	Requestor req;
	std::thread reqT1(req, divResult);
	std::thread reqT2(req, divResult);

	divThread.join();
	reqT1.join();
	reqT2.join();
}
```
