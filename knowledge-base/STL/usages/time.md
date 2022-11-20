# Time Library

`<chrono>` library

## Time Point and Duration

```c++
auto begin= std::chrono::high_resolution_clock::now();
// ... ...
auto duration = std::chrono::high_resolution_clock::now() - begin;

auto timeInSeconds = std::chrono::duration<double>(duration).count();
auto timeInMinutes = std::chrono::duration_cast<std::chrono::minutes>(duration).count();
```

## Clocks

+ `std::chrono::system_clock`

System time, which you can synchronize with the external clock

+ `std::chrono::steady_clock`

Clock, which can not be adjusted

+ `std::chrono::high_resolution_clock`

System time with the greatest accuracy

## Calendar Time

```c++
// time point based on the internal system clock
std::chrono::time_point<std::chrono::system_clock> sysTimePoint = std::chrono::system_clock::now();

// convert to time_t object
std::time_t tp = std::chrono::system_clock::to_time_t(sysTimePoint);
// convert to calendar time
std::string sTp = std::asctime(std::gmtime(&tp));
std::cout << "Time now: " << sTp << std::endl
```
