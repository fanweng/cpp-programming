# Streams

- Stream is an interface between the program and the input/output devices
    * sequence of bytes

- Common header files:
    * `<iostream>`: definitions for formatted input/output from/to streams
    * `<fstream>`: definitions for formatted input/output from/to file streams
    * `<iomanip>`: definitions for manipulators used to format stream I/O

- Global stream objects:
    * initialized before main executes
    * `cin`: standard input stream, connected to keyboard by default, buffered
    * `cout`: standard output stream, default to console, buffered
    * `cerr`: standard error stream, unbuffered
    * `clog`: standard log stream, unbuffered

# Stream Manipulators

Streams have many *member functions* or *manipulators* to control the formatting. They can be used on input/output streams.

```c++
std::cout.width(10);        // member function
std::cout << std::setw(10); // manipulator
```

## Common stream manipulators

#### Boolean

Default displaying of boolean value is 1/0, but sometimes displaying true/false is more appropriate.

Manipulator version:

```c++
std::cout << std::noboolalpha;  // further output will be 1 or 0
std::cout << std::boolalpha;    // further output will be true or false
std::cout << std::resetiosflags(std::ios::boolalpha);   // reset to default
```

Member method version:

```c++
std::cout.setf(std::ios::boolalpha);
std::cout.setf(std::ios::noboolalpha);
```

#### Integer

Default integer value is displayed as `dec`/`noshowbase`/`nouppercase`/`noshowpos`.

Besides the manipulators shown below, we can also use `std::cout.setf()` and `std::resetiosflags()`.

```c++
int num{255};
std::cout << std::showbase;                 // show prefixes "0x" and "0"
std::cout << std::dec << num << std::endl;  // 255
std::cout << std::hex << num << std::endl;  // 0xff
std::cout << std::oct << num << std::endl;  // 0377
```

#### Floating point

Default floating point value is displayed as `setprecision(6)`/not-`fixed`/`noshowpoint`/`nouppercase`/`noshowpos`. For example, `double num{123456789.987654321}` is shown as `1.23457e+008` with rounding.

Also, `std::cout.unsetf(flag1 | flag2)` and `std::resetiosflags(std::ios::floatfield)` can be used to reset flags or reset to defaults.

- `fixed`: count the precision from the decimal point
- `showpoint`: show trailing zeros up to the precision
- `scientific`: force a scientific format

#### Align and fill

Field width, justification, fill: `setw`, `left`, `right`, `internal`, `setfill`, ...

Note that those manipulators ONLY affect the object next to them.

```c++
std::cout << std::setw(10) << std::left << num << str << std::endl; // formats only affect "num"
```

#### Others

`endl`, `flush`, `ws`, ...

# Files

## Basics

Header files: `<iostream>`, `<fstream>`.

| Data Types | Description |
|------------|-------------|
| `ofstream` | create and write to files |
| `ifstream` | read from files |
| `fstream`  | create, read and write to files |

Checking file status: `f_obj.is_open()`, `f_obj.eof()`, ...

| `std::ios::open_mode` | Description |
|-----------------------|-------------|
| `app`    | **append**, set stream's position indicator to the end of stream before each output operation | 
| `ate`    | **at end**, set stream's position indicator to the end of the stream on opening |
| `binary` | **binary**, consider stream as binary rather than text |
| `in`     | **input**, allow input operation |
| `out`    | **output**, allow output operation |
| `trunc`  | **truncate**, discard current content, assuming length of zero at opening |

```c++
#include <iostream>
#include <fstream>

void main() {
    // Output to a file
    std::ofstream myWriteFile("testfile.txt");   // create and open the file
    myWriteFile << "The story begins at 1984...";
    /* Another way of writing to a file */
    // char c{'a'};
    // myWriteFile.put(c);
    myWriteFile.close();

    // Input from a file
    std::string lineText;
    std::ifstream myReadFile("testfile.txt");    // open the file
    if (!myReadFile) {
        std::cerr << "Failed to open file" << std::endl;
        return 1;
    }
    /* Another way of opening a file */
    // std::ifstream myReadFile;
    // std::string filename{"testfile.txt"};
    // myReadFile.open(filename);

    while (std::getline(myReadFile, lineText)) {
        std::cout << lineText;
    }
    /* Another way of reading a file */
    // char c{};
    // while (myReadFile.get(c)) {
    //     std::cout << c;
    // }
    // std::cout << std::endl;

    myReadFile.close();
}
```

# Strings

String streams allow to read/write strings in memory as we read/write files, especially useful for data validation.

Header: `<sstream>`

Classes: `stringstream`, `istringstream`, `ostringstream`

```c++
#include <iostream>
#include <sstream>

/* Read/Input from a string stream */
int num{};
double height{};
std::string name{};
std::string info{"Mike 23 1.97"};
std::istringstream iss{info};       // create an iss and connect to the string object
iss >> name >> num >> height;

/* Write/Output to a string stream */
num = 41;
height = 2.17;
name = "Dirk";
std::ostringstream oss{};
oss << name << " " << nume << " " << height;
std:cout << oss.str() << std::endl;

/* Validate input with string stream */
int value{};
std::string input{};
std::cout << "Enter an integer: ";
std::cin >> input;
std::stringstream ss{input};
if (ss >> value) {
    std::cout << "An integer was entered";
}
else {
    std::cout << "Input doesn't have an integer";
}
std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard the remaning content in the input buffer
```