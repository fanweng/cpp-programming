# Algorithms

The header `<algorithm>` defines many commonly used algorithms for processing sequences of elements from containers, e.g. `find`, `max`, `sort`, `accumulate`, `count`, etc.

Many algorithms requires extra information in order to do their work. For example, to use `find()` on a custom class object, that class must have `operator==` defined.
- Functors (function objects)
- Function pointers
- **[Lambda expressions](../../concepts/lambda.md)** (introduced by C++11) <- recommended

A complete list of common and useful algorithms can be found at [https://en.cppreference.com/w/cpp/algorithm](https://en.cppreference.com/w/cpp/algorithm).

## Non-modifying

```c++
/* for_each() applies a unary function to each element */
std::vector<int> nums = {1,2,3};
// Opt 1: Functor
struct Print_Functor {
    void operator() (const int& n) {
        std::cout << n;
    }
};
Print_Functor print; // function object
// Opt 2: Function pointer
void print(const int& n) {
    std::cout << n;
}
// Opt 3: Lambda expression
auto print = [](const int& n) {std::cout << n;};
// All three options can be used
std::for_each(nums.begin(), nums.end(), print);


/* find() the 1st element in the container */
/* return an iterator pointing to the located element or end() */
auto it = std::find(nums.begin(), nums.end(), 2);   // if found 2, it!=nums.ends()
/* find_if(), find_first_of(), adjacent_find() */


/* search() the 1st occurrence of the sequence of elements */
std::vector<int> v1 = {9,1,2,3,4,1,1};
auto it = std::search(v1.begin(), v1.end(), nums.begin(), nums.end());
if (it != v1.end()) {
    std::cout << "nums[] is presented at index " << std::distance(v1.begin(), it) << std::endl;
}


/* min_element(), max_element() return the iterator of min/max element */
std::vector<int>::iterator it = std::min_element(v1.begin(), v1.end());
/* min(), max(), minmax() return the values directly, lambda function enabled */
int intMin = std::min(-10, -5, [](int a, int b) { return std::abs(a) < std::abs(b); }); // -5
std::pair<int, int> pairint = std::minmax({3, 12, -1, -10}); // -10,12


/* count() the number of specific element */
std::vector<int>::iterator it = std::count(v1.begin(), v1.end(), 1);
/* count_if() */


/* std::all_of, std::any_of, std::none_of return condition over a range of elements */
std::vector<int> v2 {1, 2, 3, 4, 5, 6, 7, 8, 9};
std::any_of(v2.begin(), v2.end(), [](int i){return i%2;}); // true


/* forward() enables to write function templates where the arguments are identically forwarded */
template <typename T, typename... Args>
    T createT(Args&&... args) {
        return T(std::forward<Args>(args)...);
    }
struct MyData {
    MyData(int, double, char){};
};
MyData myData1 = createT<MyData>(1, 2.3, 'a');


/* std::equal() checks if both ranges are equal */
std::string str1{"Only For Testing Purpose."};
std::string str2{"Only For Testing purpose."}
std::equal(str1.begin(), str1.end(), str2.begin()); // false
std::equal(str1.begin(), str1.end(),
            str2.begin(),
            [](char c1, char c2){return std::toupper(c1) == std::toupper(c2);}); // true

/* std::lexicographical_compare() checks if first range is smaller than the second */

/* std::mismatch() finds the first position at which both ranges are not equal */
auto itr_pair = std::mismatch(str1.begin(), str1.end(), str2.begin());
// if str1 == str2, then itr_pair.first == str1.end()
*itr_pair.first;    // P
*itr_pair.second;   // p
```

### Reference Wrappers

Reference wrapper is a *copy-constructible* and *copy-assignable* wrapper for an object of type `&`.

```c++
template <typename T>
    void doubleMe(T t) {
        t *= 2;
}
int a = 1;
doubleMe(a); // a = 1
/* std::ref creates a non constant reference wrapper */
doubleMe(std::ref(a)); // a = 2


void invokeMe(const std::string& s){
  std::cout << s << ": const " << std::endl;
}
std::string s{"string"};
/* std::cref creates a constant reference wrapper */
invokeMe(std::cref(s));


/* std::reference_wrapper creates a reference object */
void foo() {
  std::cout << "foo invoked" << std::endl;
}
typedef void callableUnit();
std::reference_wrapper<callableUnit> refWrap(foo);
refWrap(); // foo invoked

int a = 2;
std::reference_wrapper<int> myInt(a);
std::cout << myInt << " " << myInt.get() << std::endl; // 2 2
```

## Modifying

```c++
/* copy() a range of elements to another range */
std::copy(src_vector.begin(), src_vector.end(), dest_vector.begin());


/* replace() elements in a range */
std::string str {"only for test"};
std::replace(str.begin(), str.end(), ' ', '1'); // only1for1test
str::string str2;
std::replace_copy_if(str.begin(), str.end(), std::back_inserter(str2), [](char c){return c == '1';}, '2'); // str2=only2for2test


/* remove() a range of elements */
std::vector<int> v {0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10};
auto fwdIt = std::remove_if(v.begin(), v.end(), [](int a){return a%2;});
v.erase(fwdIt, v.end()); // 0,2,4,6,8,10
/* or, a compact way */
// v.erase(std::remove_if(v.begin(), v.end(), [](int a){return a%2;}), v.end());


/* fill() a range of elements with specific value */
std::fill(v.begin(), v.end(), -1);
/* generate() a range of elements */
int genNext() {
    static int next {0};
    return ++next;
}
std::vector<int> v(8, 0);
std::generate_n(v.begin(), 5, genNext); // 1,2,3,4,5,8,8,8


/* move() a range of elements to destination */
std::vector<int> src {0,1,2,3,4,5,6,7};
std::vector<int> des(10);
std::move(src.begin(), src.end(), des.begin());
// src remains the same
// des = 0,1,2,3,4,5,6,7,0,0


/* transform() applies a unary/binary callable to a range and copies the modified to destination */
std::string str {"abcd"};
std::transform(str.begin(), str.end(), str.begin(), [](char c){ return std::toupper(c); }); // ABCD


/* rotate() makes the picked element as the new first element */
std::string str {"12345"};
std::rotate(str.begin(), str.begin()+2, str.end()); // 3,4,5,1,2


/* shuffle() */
std::vector<int> v {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::shuffle(v.begin(), v.end(), std::default_random_engine(seed));


/* unique() removes the duplicates */
std::vector<int> v {0, 0, 2, 3, 4, 0};
v.erase(std::unique(v.begin(), v.end()), v.end()); // 0,2,3,4


/* reverse() elements in a range */
std::reverse(v.begin(), v.end());


/* swap() interchanges the values of two elements */
std::swap(a, b);


/* partition() decompose a set into subsets */
bool isOdd(int i){ return (i%2); }
std::vector<int> vec{1, 4, 3, 8, 5, 6};
auto parPoint_it = std::partition(vec.begin(), vec.end(), isOdd);
// 1,3,5,4,8,6, and parPoint_it points to 4


/* make_heap() creates a heap */
std::vector<int> vec{4, 3, 2, 1, 5, 6, 7, 9, 10};
std::make_heap(vec.begin(), vec.end()); // 10,9,7,4,5,6,2,3,1
vec.push_back(100); // 10,9,7,4,5,6,2,3,1,100
std::is_heap_until(vec.begin(), vec.end()); // itr points to 100
std::push_heap(vec.begin(), vec.end()); // 100 10 7 4 9 6 2 3 1 5
std::pop_heap(vec.begin(), vec.end());  // 10 9 7 4 5 6 2 3 1 100
vec.resize(vec.size() - 1); // 10,9,7,4,5,6,2,3,1


/* next_permutation(), prev_permutation() return the next bigger or previous smaller permutation of the newly ordered range */


/* numeric operations: accumulate(), adjacent_difference(), inner_product(), partial_sum(), etc. */
```

#### `move()`

`move()` efficiently transfers the resource to another object:
- thread/lock object cannot be copied, only be moved
- move operation leaves the moved-from object valid but with unspecified value
- after moved, the moved-from object should be destroyed or assigned a new value
 
```c++
std::string str = "hello";
std::vector<string> v;
v.push_back(str);               // copy str to vector
v.push_back(std::move(str));    // content of str is moved to vector, after which str might be empty
/* move() also works for class which defines the move constructor/assignment */
class MyData{
    MyData(MyData&& m) = default; // move constructor
    MyData& operator=(MyData&& m) = default; // move assignment 
    MyData(const MyData& m) = default; // copy constructor
    MyData& operator=(const myData& m) = default; // copy assignment
}
```

## Sorting and Searching

```c++
/* sort() uses default operator < */
std::sort(v.begin(), v.end());
std::sort(v.begin(), v.end(), greater<int>());   // use operator >, descending order


/* binary_search() finds the matching value after sort() */
bool found = std::binary_search(v.begin(), v.end(), val);


/* merge() combines two sorted ranges into one sorted range */
std::merge(v1.begin(), v1.end(), v2.begin(), v2.begin(), dst.begin());
std::merge(v1.begin(), v1.end(), v2.begin(), v2.begin(), back_inserter(dst));
```

## Parallel and Vectorized Execution

The policy tag specifies whether a program should run:
+ `std::parallel::seq`: sequentially
+ `std::parallel::par`: in parallel on multiple threads
+ `std::paralell:par_unseq`: in parallel with vectorization

```c++
vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};

// standard sequential sort
std::sort(v.begin(), v.end());
std::sort(std::parallel::seq, v.begin(), v.end());

// permitting parallel execution
std::sort(std::parallel::par, v.begin(), v.end());

// permitting parallel and vectorised execution
std::sort(std::parallel::par_unseq, v.begin(), v.end());
```

> NOTE: parallel algorithm does not automatically protect you from **data races** and **deadlocks**
