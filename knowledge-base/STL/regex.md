# Regular Expressions

## Basics

Regular expression is a language to describe the text patterns, where the header `<regex>` is used.

Three main concepts: characters, groupings, quantifiers.

### Characters

| Description | Character |
|-------------|-----------|
| Escape character     | `\` |
| Any character        | `.` |
| Digit                | `\d` |
| Not a digit          | `\D` |
| Word character       | `\w` |
| Not a word character | `\W` |
| Whitespace           | `\s` |
| Not whitespace       | `\S` |
| Word boundary        | `\b` |
| Not a word boundary  | `\B` |
| Beginning of a string| `^` |
| End of a string      | `$` |

### Groupings

| Description | Character |
|-------------|-----------|
| Matches characters in brackets     | `[ ]`  |
| Matches characters not in brackets | `[^ ]` |
| Either or                          | `|`    |
| Capturing group                    | `( )`  |

### Quantifiers

| Description | Character |
|-------------|-----------|
| 0 or more character | `*` |
| 1 or more character | `+` |
| 0 or 1 character    | `?` |
| An exact number of characters | `{ }`                |
| Range of number of characters | `{Minimum, Maximum}` |

### Raw string literals

**Use raw string literals** for regular expressions to avoid ugly text.

```c++
/* first '\' indicates the special character next '\', second '\' indicates the special character '+' */
std::string regex("C\\+\\+");
/* in the raw string literal, '\' won't be interpreted as string, so 1st '\' is skipped */
std::string regexRaw(R"(C\+\+)");
```

### Normal three steps

```c++
/* Step 1: define regular expression */
std::string text {"C++ or c++"};
std::string regExpr(R"(c\+\+)");
std::regex rgx(regExpr);

/* Step 2: store the result of operation */
std::smatch result;
std::regex_search(text, result, rgx);

/* Step 3: process the result */
std::cout << result[0] << std::endl; // c++

/* set the case insensitive flag) */
std::regex rgxIns(regExpr, std::regex_constants::ECMAScript | std::regex_constants::icase);
std::regex_search(text, result, rgxIns);
std::cout << result[0] << std::endl; // C++
```

## Match results `std::match_results`

+ is the result of a `std::regex_match` or `std::regex_search`
+ a sequential container having at least one capturing group of a `std::sub_match` object
    - `std::sub_match` objects are sequences of characters

#### Four types of synonyms

```c++
typedef match_results<const char*> cmatch;
typedef match_results<const wchar_t*> wcmatch;
typedef match_results<string::const_iterator> smatch;
typedef match_results<wstring::const_iterator> wsmatch
```

#### `std::smatch` methods

```c++
smatch.size();      // Returns the number of capture groups.
smatch.empty();     // Returns if the search result has a capture group.
smatch[i];          // Returns the ith capture group.
smatch.length(i);   // Returns the length of the ith capture group.
smatch.position(i); // Returns the position of the ith capture group.
smatch.str(i);      // Returns the ith capture group as string.
smatch.prefix();
smatch.suffix();    // Returns the string before and after the capture group.
smatch.begin();
smatch.end();       // Returns the begin and end iterator for the capture groups.
smatch.format(...); // Formats std::smatch objects for the output.
```

#### `std::sub_match` methods

```c++
cap.matched();      // Indicates if this match was successful.
cap.first();
cap.end();          // Returns the begin and end iterator of the character sequence.
cap.length();       // Returns the length of the capture group.
cap.str();          // Returns the capture group as string.
cap.compare(other); // Compares the current capture group with another capture group.
```

```c++
std::vector<std::string> fnames {"foo.txt", "bar.txt", "baz.dat", "zoidberg"};
std::regex pieces_regex("([a-z]+)\\.([a-z]+)");
std::smatch pieces_match;

for (const auto &fname : fnames) {
    if (std::regex_match(fname, pieces_match, pieces_regex)) {
        for (size_t i = 0; i < pieces_match.size(); ++i) {
            std::ssub_match sub_match = pieces_match[i];
            std::string piece = sub_match.str();
        }
    }
}
// sub_match 0: foo.txt
// sub_match 1: foo
// sub_match 2: txt
// and for bar.txt, baz.dat
```

## Match `std::regex_match`

```c++
/* regex for a number, not including an exponent */
std::string numberRegEx(R"([-+]?([0-9]*\.[0-9]+|[0-9]+))");
/* regex holder */
std::regex rgx(numberRegEx);

const char* numChar {"2022"};
std::regex_match(numChar, rgx); // true

const std::string numStr {"3.1415"};
std::regex_match(numStr, rgx); // true

const std::vector<char> numVec {'-', '2', '.', '3', '8'};
std::regex_match(numVec, rgx); // true
```

## Search `std::regex_search`

```c++
std::regex time_rgx("([01]?[0-9]|2[0-3]):([0-5][0-9])");
std::smatch smatch;
std::string stime {"Now is 04:12"};
std::regex_search(stime, smatch, time_rgx);
// smatch[0]: 04:12
// smatch[1]: 04
// smatch[2]: 12
```

## Replace `std::regex_replace`

```c++
std::string str {"I am using C++0X."};
std::regex rgx(R"C\+\+0X");
std::string repWord {"C++17"};
std::string newStr = std::regex_replace(str, rgx, repWord); // I am using C++17
```

## Format `std::regex_format`

| Format Escape Sequence | Description |
|------------------------|-------------|
| `$$` | Returns the total match (0th capture group) |
| $`   | Returns the text before the total match     |
| `$'` | Returns the text after the total match      |
| `$i` | Returns the ith capture group               |

```c++
const std::string dest1 {"Tokyo, Japan"};
const std::string dest2 {"Shanghai, China"};
std::regex rgx("(.*), (.*)");
std::string mapText {"$1 is one of the cities in $2."};

std::string text1 = std::regex_replace(dest1, rgx, mapText); // Tokyo is one of the cities in Japan.

std::smatch smatch
if (std::regex_match(dest2, smatch, rgx)) {
    std::string text2 = smatch.format(mapText); // Shanghai is one of the cities in China.
}
```

## Repeated search with iterator

+ `std::regex_iterator`

```c++
std::string text {"To be or not to be, that is a question."};
std::regex rgx(R"(\w+)"); // regex for a word

std::sregex_iterator wordItBegin(text.begin(), text.end(), rgx); // get all words from text

const std::sregex_iterator wordItEnd; // default constructed to the end-of-sequence iterator
/* print out all words */
for (; wordItBegin != wordItEnd; ++wordItBegin) {
    std::cout << wordItBegin->str() << std::endl;
}
```

+ `std::regex_token_iterator`

They enable to specify which capture groups are of interest.

```c++
std::string text{"Pete Becker, The C++ Standard Library Extensions, 2006:Nicolai Josuttis, The C++ Standard Library, 1999:Andrei Alexandrescu, Modern C++ Design, 2001"};

std::regex regBook(R"((\w+)\s(\w+), ([\w\s\+]*), (\d{4}))");

/* if not specifying index, all capture groups are got */
std::sregex_token_iterator bookItBegin(text.begin(), text.end(), regBook);
const std::sregex_token_iterator bookItEnd;
while (bookItBegin != bookItEnd) {
  std::cout << *bookItBegin++ << std::endl;
}
// Pete Becker, The C++ Standard Library Extensions, 2006
// Nicolai Josuttis, The C++ Standard Library, 1999
// Andrei Alexandrescu, Modern C++ Design, 2001

/* 2nd and 4th capturing groups are got */
std::sregex_token_iterator bookItNameIssueBegin(text.begin(), text.end(), regBook, {2, 4});
const std::sregex_token_iterator bookItNameIssueEnd;
while (bookItNameIssueBegin != bookItNameIssueEnd) {
    std::cout << *bookItNameIssueBegin++ << ", ";
    std::cout << *bookItNameIssueBegin++ << std::endl;
}
// Becker, 2006
// Josuttis, 1999
// Alexandrescu, 2001

/* index -1 will get the text between matches, e.g. between ':' in the example below */
std::regex regBookNeg(":");
std::sregex_token_iterator bookItNegBegin(text.begin(), text.end(), regBookNeg, -1);
const std::sregex_token_iterator bookItNegEnd;
while ( bookItNegBegin != bookItNegEnd) {
    std::cout << *bookItNegBegin++ << std::endl;
}
// Pete Becker, The C++ Standard Library Extensions, 2006
// Nicolai Josuttis, The C++ Standard Library, 1999
// Andrei Alexandrescu, Modern C++ Design, 2001
```
