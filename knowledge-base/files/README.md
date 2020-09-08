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