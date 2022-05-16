# Text Processing

## I. `std::string`

#### Conversion between C string and C++ string

```c++
std::string str{"abcdefg"};
const char* cString= str.c_str(); // including the terminating null character

char buffer[3];
str.copy(buffer, 3); // copy to C string buffer without null terminator "\0"

std::string str2(buffer, buffer3); // abc
```

#### Overloaded `+` operator

```c++
std::string wrong = "1" + "1";              // error
std::string right = std::string("1") + "1"; // 11
```

#### Element access

```c++
std::string str {"0123456789"};
str.front();
str.back();
str[9];     // 9, no boundary check
str.at(20); // boundary check, throw an exception
```

#### I/O

```c++
std::ifstream file("./test.txt");
std::vector<std::string> lines;
std::string line;
while (std::getline(file, line)) { // default delimiter is new line
    lines.push_back(line);
}
```

#### Search

```c++
str= {"01234567898"};
std::string str2{"98"};
str.find('8');          // 8
str.rfind('8');         // 10, from reverse
str.find('8', 9);       // 10, since 9th element
str.find(str2);         // 9, find the entire str2
str.find_first_of("732");   // 2, find one of the 7/3/2
str.find_last_of("732");    // 7, find one of the 7/3/2
str.find_first_not_of("732");   // 0, find not one of the 7/3/2
str.find_last_not_of("732");    // 10, find not one of the 7/3/2
```

#### Modifying operations

```c++
std::string str1 {"123456"};
std::string str2 {"7890"};
str1.assign(str2, 1, std::string::npos);    // str1="890"
str1.erase(str1.begin() + 1, str1.begin() + 2);  // st1 = "80"

str1.assign(10, '#');                               // ##########
str1.replace(0, 1, "a");                            // a#########
str1.replace(0, 5, "abcdefg", 1, 5);                // bcdef#####
str1.replace(str1.begin()+5, str1.end(), "*****");  // bcdef*****
```

#### Numeric conversion

+ Convert into `std::string`: `std::to_string()`
+ Convert into numbers: `std::stoi(str, idx=nullptr, base=10)`, `std::stol()`, `std::stof()`, `std::stod()`, etc.
    * ignores leading spaces
    * optionally returns the index of the first invalid character
    * optionally to specify the base

## II. `std::string_view`

A `std::string_view` is a **non-owning** reference to a `std::string`.
+ a view of a sequence of characters
+ a kind of restricted `string` that supports only immutable operations
+ creation or copying of `string_view` will NOT allocate new memory

Common methods that are the same with `std::string`:
+ Element access: `operator[]`, `at`, `front`, `back`, `data`
+ Capacity: `size`, `length`, `max_size`, `empty`
+ Search: `find`, etc.

Unique methods:
+ `remove_prefix`: shrink string start forward
+ `remove_suffix`: shrink string end backward

## III. Regular Expression Library `regex`
