#keyword 

# `thread_local`

Thread-local data, also known as thread-local storage:
+ created for each thread separately
+ behaves like static data because it’s bound for the lifetime of the thread
+ created at its first usage
+ thread-local data belongs exclusively to the thread

```c++
std::mutex coutMutex;
thread_local std::string s("hello from ");

void addThreadLocal(std::string const& s2){
  s += s2;
  // protect std::cout
  std::lock_guard<std::mutex> guard(coutMutex);
  std::cout << s << std::endl;
  std::cout << "&s: " << &s << std::endl;
}

void main(){
  std::thread t1(addThreadLocal, "t1"); // hello from t1, &s is different from t2
  std::thread t2(addThreadLocal, "t2"); // hello from t2, &s is different from t1
  t1.join();
  t2.join();
}
```
