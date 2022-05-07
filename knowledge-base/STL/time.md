# Time Library

`<chrono>` library.

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
