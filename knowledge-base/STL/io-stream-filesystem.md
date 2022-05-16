# I/O Stream and Filesystem

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
    * `clog`: standard log stream, buffered



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



# State of Streams

| Flags | Query of the Flag | Description |
|------------|--------------|-------------|
| `std::ios::goodbit` | `stream.good()` | no bit is set          |
| `std::ios::eofbit`  | `stream.eof()`  | end-of-file bit is set |
| `std::ios::failbit` | `stream.fail()` | error, false formatted reading, EOF, error opening, etc. |
| `std::ios::badbit`  | `stream.bad()`  | undefined behavior     |

The state of a stream can be read and set: `clear()`, `rdstate()`, `setstate(flag)`, etc.



# Input Functions

+ `is.get(ch)`: read one character
+ `is.get(buf, num)`: read at most `num` characters into `buf`
+ `is.getline(buf, num, delim)`: read at most `num` characters into `buf` until `delim`
+ `is.gcount()`: return number of characters extracted from `is` by last operation
+ `is.ignore(size, delim)`: ignore `size` characters until `delim`
+ `is.peek()`: get one character from `is` without consuming it
+ `is.unget()`: push the last read character back to `is`
+ `is.putback(ch)`: push `ch` character onto `is`



# Format Specifier

#### Display boolean

+ `std::boolalpha`: display the boolean as a word
+ `std::noboolalpha`: display the boolean as a number (default)

#### Set field width and fill character

+ `std::setw(val)`: set field width to `val`
+ `std::setfill(c)`: set the fill character to `c`, default is space

#### Alignment of text

+ `std::left`, `std::right`, etc.

#### Positive sign and letter case

+ `std::showpos`, `std::noshowpos` (default), `std::uppercase`, `std::lowercase`

#### Numeric base

+ `std::oct`, `std::dec` (default), `std::hex`, `std::showbase`, `std::noshowbase` (default)

#### Floating point numbers

+ `std::setprecision(val)`, `std::showpoint`, `std::noshowpoint` (default), `std::fixed`, `std::scientific`, etc.



# File Streams

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

#### Random access

| Navigating in a file stream | Description |
|-----------------------------|-------------|
| `file.tellg()`    | return the **read** position of `file` |
| `file.seekg(pos)` | set the the **read** position to `pos` |
| `file.seekg(off, rpos)` | set the the **read** position to offset relative to `pos` |
| `file.tellp()`    | return the **write** position of `file` |
| `file.seekp(pos)` | set the the **write** position to `pos` |
| `file.seekp(off, rpos)` | set the the **write** position to offset relative to `pos` |

+ `rpos` have three values: `std::ios::beg`, `std::ios::cur`, `std::ios::end`
+ `off` has to be a number.

> If randomly accessing, C++ runtime doesn't check the file boundaries!



# String Streams

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
