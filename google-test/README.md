# Google Test

Google Test (GTest) is the Google's C++ test framework based on the xUnit architecture.

## Installation

Install the cmake and gtest development package. Compile the gtest source code and copy the libraries to */usr/lib/* directory.

```
$ sudo apt-get install cmake
$ sudo apt-get install libgtest-dev
$ cd /usr/src/gtest
$ sudo cmake CMakeLists.txt
$ sudo make
$ sudo cp libgtest.a libgtest_main.a /usr/lib/
```

## Compile the Test

```
$ g++ -o test01 test01.cpp -lgtest -lgtest_main -lphtread
```

## References

[GTest GitHub page](https://github.com/google/googletest)