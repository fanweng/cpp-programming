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

## Compile the Test

```
$ g++ -o test01 test01.cpp -lgtest -lgtest_main -lphtread
```

## References

[GTest GitHub page](https://github.com/google/googletest)

[CodesBay Google C++ Test Framework YouTube playlist](https://www.youtube.com/playlist?list=PL_dsdStdDXbo-zApdWB5XiF2aWpsqzV55)