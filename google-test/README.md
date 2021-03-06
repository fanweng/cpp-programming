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

## Concepts

**Test Program** contains one or more **test suites** to reflect the structure of the tested code.

**Test Suite** contains one or more **subtests**.

**Test Fixture** is used when multiple **subtests** need to share common objects and subroutines.

A subtest result can be *success*, *non-fatal failure* (program continues), or *fatal failure* (aborts the currect subtest).

|  Fatal Assertions  | Non-fatal Assertions   | Verifies   |
|--------------------|------------------------|------------|
| `ASSERT_TRUE(cond)`  | `EXPERCT_TRUE(cond)`  | `cond` is true  |
| `ASSERT_FALSE(cond)` | `EXPERCT_FALSE(cond)` | `cond` is false |
| `ASSERT_EQ(val1, val2)` | `EXPECT_EQ(val1, val2)` | `val1 == val2` |
| `ASSERT_NE(val1, val2)` | `EXPECT_NE(val1, val2)` | `val1 != val2` |
| `ASSERT_LT(val1, val2)` | `EXPECT_LT(val1, val2)` | `val1 < val2`  |
| `ASSERT_LE(val1, val2)` | `EXPECT_LE(val1, val2)` | `val1 <= val2` |
| `ASSERT_GT(val1, val2)` | `EXPECT_GT(val1, val2)` | `val1 > val2`  |
| `ASSERT_GE(val1, val2)` | `EXPECT_GE(val1, val2)` | `val1 >= val2` |
| `ASSERT_STREQ(str1,str2);`     | `EXPECT_STREQ(str1,str2);`     | the two C strings have the same content   		         |
| `ASSERT_STRNE(str1,str2);`     | `EXPECT_STRNE(str1,str2);`     | the two C strings have different contents 		         |
| `ASSERT_STRCASEEQ(str1,str2);` | `EXPECT_STRCASEEQ(str1,str2);` | the two C strings have the same content, ignoring case   |
| `ASSERT_STRCASENE(str1,str2);` | `EXPECT_STRCASENE(str1,str2);` | the two C strings have different contents, ignoring case |

### How to write a test: AAA

**Arrange**: set up everything required to run the test

**Act**: run the test

**Assert**: verify the output

Take [test/test01.cpp](./test/test01.cpp) as the example.

### Test Fixture `TEST_F`

A test fixture is a class that inherits from `::testing::Test` and its internal state is accessible to the tests that use it. The `SetUp()` function will be called before executing each `TEST_F` and `TearDown()` is executed after each `TEST_F`.

Take [test/test02.cpp](./test/test02.cpp) as the example.

## Compile the Test

```
$ g++ -o test01 test01.cpp -lgtest -lgtest_main -lphtread
```





# Google Mock

When writing a test, sometimes it's not feasible to rely on real objects entirely, e.g. data base connect function. A **mock object** implements the same interface as a real object, and lets you specify at **runtime** how it will be used and what it should do (which methods to be called? in which order? how many times? with what arguments? what will return? etc.)

Using gMock as a testing tool - cut off the test's outbound dependencies and probe the interaction between your module and its collaborators.

## Installation

gMock is included in the same development package of gTest. Compile the gmock source code and copy the libraries to */usr/lib/* directory.

```
$ sudo apt-get install cmake
$ sudo apt-get install libgtest-dev
$ cd /usr/src/gmock
$ sudo cmake CMakeLists.txt
$ sudo make
$ sudo cp libgmock.a libgmock_main.a /usr/lib/
```

## Mock Functions

Take [mock/mock01.cpp](./mock/mock01.cpp) as the example.

#### `MOCK_METHOD`

`MOCK_METHOD` macro needs 3 or 4 parameters. The 4th parameter accepts a list of qualifiers, e.g. `const`, `override`, etc. Old-style uses `MOCK_METHODn` and `n` represents the number of arguments.

```c++
MOCK_METHOD(ReturnType, MethodName, (Args..));
MOCK_METHOD(ReturnType, MethodName, (Args..), (Specs...));
```

## Compile the Test

```
$ g++ -o mock01 mock01.cpp -lgtest -lgtest_main -lgmock -lphtread
```





# References

[GTest GitHub page](https://github.com/google/googletest)

[GMock Github doc](https://github.com/google/googletest/tree/master/googlemock/docs)

[gMock Cookbook](https://chromium.googlesource.com/external/github.com/google/googletest/+/HEAD/googlemock/docs/cook_book.md)

[gMock Cheatsheet](https://chromium.googlesource.com/external/github.com/google/googletest/+/HEAD/googlemock/docs/cheat_sheet.md)

[CodesBay Google C++ Test Framework YouTube playlist](https://www.youtube.com/playlist?list=PL_dsdStdDXbo-zApdWB5XiF2aWpsqzV55)