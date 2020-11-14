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
```

Member method version:

```c++
std::cout.setf(std::ios::boolalpha);
std::cout.setf(std::ios::noboolalpha);
```

* Integer: `dec`, `hex`, `oct`, `showbase`, ...
* Floating point: `fixed`, `scientific`, `setprecision`, ...
* Field width, justification, fill: `setw`, `left`, `right`, `internal`, `setfill`, ...
* Others: `endl`, `flush`, `ws`, ...

# Files

## Basics

Header files: `<iostream>`, `<fstream>`.

| Data Types | Description |
|------------|-------------|
| `ofstream` | create and write to files |
| `ifstream` | read from files |
| `fstream`  | create, read and write to files |

```c++
#include <iostream>
#include <fstream>
using namespace std;

void main() {
    ofstream myWriteFile("testfile.txt");   // create and open the file
    myWriteFile << "The story begins at 1984...";
    myWriteFile.close();

    string lineText;
    ifstream myReadFile("testfile.txt");    // open the file
    while (getline(myReadFile, lineText)) {
        cout << lineText;
    }
    myReadFile.close();
}
```