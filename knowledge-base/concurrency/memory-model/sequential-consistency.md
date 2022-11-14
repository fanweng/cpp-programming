# Sequential Consistency

The key is that all operations on all threads obey a universal lock:
+ intuitive
+ intuitive comes with a price: more system overhead to keep threads in sync

```c++
std::string work;
std::atomic<bool> ready(false);

void consumer() {
  while(!ready.load()){}
  std::cout<< work << std::endl;    
}

void producer() {
  work= "done";
  ready=true;
}

int main(){
  std::thread prod(producer);
  std::thread con(consumer);
  prod.join();
  con.join();
}
```
