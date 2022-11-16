#concurrency/thread-safe #concurrency/thread   

# Thread-safe Initialization of Data

If the variable is never modified there is no need for synchronization by using an expensive lock or an atomic.

Three ways to initialize variables in a thread-safe way:

## 1. [constant expressions](../../keywords/constexpr.md)

## 2. Function `std::call_once` with Flag `std::once_flag`

+ `std::call_once` function registers a callable
+ `std::once_flag` ensures that only one registered function using the same flag will be invoked

```c++
class MySingleton{
  private:
    static std::once_flag initOnceFlag;
    static MySingleton* instance;
    MySingleton() = default;
    ~MySingleton() = default;

  public:
    MySingleton(const MySingleton&) = delete;
    MySingleton& operator=(const MySingleton&) = delete;  

    static MySingleton* getInstance(){
      std::call_once(initOnceFlag, MySingleton::initSingleton);
      return instance;
    } 

    static void initSingleton(){
      instance = new MySingleton();
    }
};

MySingleton* MySingleton::instance = nullptr;
std::once_flag MySingleton::initOnceFlag;

int main(){
  std::cout << "MySingleton::getInstance(): "<< MySingleton::getInstance() << std::endl;
  std::cout << "MySingleton::getInstance(): "<< MySingleton::getInstance() << std::endl;
}
```

## 3. A Static Variable with Block Scope

[Static](../../keywords/static.md) variable with block scope will be created exactly once and lazily at the moment of the usage, which is useful for an elegant implementation of the singleton pattern (*NOTE: compiler must support thread-safe semantic for static variable*).

```c++
class MySingleton {
public:
  static MySingleton& getInstance(){
    static MySingleton instance;
    return instance;
  }

private:
  MySingleton() = default;
  ~MySingleton() = default;
  MySingleton(const MySingleton&)= delete;
  MySingleton& operator=(const MySingleton&)= delete;
};

int main(){
  MySingleton::getInstance();
}
```

## (?) 4. initialize the variable in the main thread before creating child threads
