#concurrency/memory-model 

# Relaxed Semantic

This only guarantees that the operations on the same atomic variable in the same thread won't be reordered.

In the following counter example, the sequence of operations doesn't matter, i.e. different threads can increment the counter in any order but each operation is still atomic.

```c++
std::atomic<int> count = {0};

void add(){
    for (int n = 0; n < 1000; ++n) {
        count.fetch_add(1, std::memory_order_relaxed);
    }
}

int main(){
    std::vector<std::thread> v;
    for (int n = 0; n < 10; ++n) {
        v.emplace_back(add);
    }
    for (auto& t : v) {
        t.join();
    }
    std::cout << "Final counter value is " << count << '\n';
```
