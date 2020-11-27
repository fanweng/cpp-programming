# C++ Standard Template Library (STL)

STL is a set of C++ template classes to provide common data structures and functions.

Four STL components:
- Algorithms
- Containers
- Functions
- Iterators

## 1. Algorithms

The header `<algorithm>` defines many commonly used algorithms for processing sequences of elements from containers, e.g. `find`, `max`, `sort`, `accumulate`, `count`, etc.

Many algorithms requires extra information in order to do their work. For example, to use `find()` on a custom class object, that class must have `operator==` defined.
- Functors (function objects)
- Function pointers
- **Lambda expressions** (introduced by C++11) <- recommended

A complete list of common and useful algorithms can be found at [https://en.cppreference.com/w/cpp/algorithm](https://en.cppreference.com/w/cpp/algorithm).

### Non-modifying

```c++
/* for_each() applies a unary function to each element */
std::vector<int> nums = {1,2,3};
// Functor
struct Print_Functor {
    void operator() (const int& n) {
        std::cout << n;
    }
};
Print_Functor print; // function object
// Function pointer
void print(const int& n) {
    std::cout << n;
}
// Lambda expression
auto print = [](const int& n) {std::cout << n;};
std::for_each(nums.begin(), nums.end(), print);

/* find() the 1st element in the container */
/* return an iterator pointing to the located element or end() */
auto it = std::find(nums.begin(), nums.end(), 2);   // if found 2, it!=nums.ends()

/* search() the 1st occurrence of the sequence of elements */
std::vector<int> v1 = {9,1,2,3,4,1,1};
auto it = std::search(v1.begin(), v1.end(), nums.begin(), nums.end());
if (it != v1.end()) {
    std::cout << "nums is presented at index " << std::distance(v1.begin(), it) << std::endl;
}

/* min_element(), max_element() returs the min/max element */
std::vector<int>::iterator it = std::min_element(v1.begin(), v1.end());

/* count() the number of specific element */
std::vector<int>::iterator it = std::count(v1.begin(), v1.end(), 1);
```

### Modifying

```c++
/* copy() a range of elements to another range */
std::copy(src_vector.begin(), src_vector.end(), dest_vector.begin());

/* fill() a range of elements with specific value */
std::fill(v.begin(), v.end(), -1);

/* reverse() elements in a range */
std::reverse(v.begin(), v.end());

/* swap() interchanges the values of two elements */
std::swap(a, b);

/* move() efficiently transfers the resource to another object */
/* move operation leaves the moved-from object valid but with unspecified value */
/* after moved, the moved-from object should be destroyed or assigned a new value */
std::string str = "hello";
std::vector<string> v;
v.push_back(str);       // copy str to vector
v.push_back(move(str)); // content of str is moved to vector, after which str might be empty
```

### Sorting and Searching

```c++
/* sort() uses default operator < */
std::sort(v.begin(), v.end());
std::sort(v.begin(), v.end(), greater<int>());   // use operator >, descending order

/* binary_search() finds the matching value */
bool found = std::binary_search(v.begin(), v.end(), val);

/* merge() combines two sorted ranges into one sorted range */
std::merge(v1.begin(), v1.end(), v2.begin(), v2.begin(), dst.begin());
std::merge(v1.begin(), v1.end(), v2.begin(), v2.begin(), back_inserter(dst));
```

## 2. Containers

*Containers* or *container classes* store objects and data. Collections of objects or primitive types, e.g. `array`, `vector`, `deque`, `set`, `map`, etc. 

Containers used in programming: dynamic arrays (vector), queues (queue), stacks (stack), heaps (priority_queue), linked lists (list), trees (set), associative arrays (map)...

- Sequence containers
    * array, vector, list, forward_list, deque
- Associative containers
    * set, multiset, map, multimap
- Unordered associative containers
    * unordered_set, unordered_multiset, unordered_map, unordered_multimap
- Container adaptors
    * stack, queue, priority_queue

Each container has member functions: some are specific to the container, others are available to all containers (see below).
![Containers Common Member Functions](./containers-common-methods.png)

> Q: What type of elements can be stored in containers?
> - A copy of the element will be stored in the container
> - Element should be copyable (copy constructor), assignable (copy assignment), and moveable (move constructor, move assignment)
> - ordered associative containers must be able to compare elements (operator<, operator==)

#### `pair`

```c++
std::pair<int, char> p1 = std::make_pair(1, 'a');
std::pair<int, pair<int, char>> p2 = {1, {2, 'b'}};
std::cout << p1.first << p1.second << std::endl;  // 1, a
std::cout << p2.second.second << std::endl;       // b
```

#### `tuple`

*Tuple* is an object that can hold a number of elements.

```c++
std::tuple<char, int, float> t;
t = std::make_tuple('a', 29, 1.78);

std::cout << get<0>(t) << std::endl;  // access the 1st element
std::get<2>(t) = 1.86;           // modify the 3rd element

std::tie(c_val, i_val, f_val) = t;   // unpack tuple values into separate variables
std::cout << c_val << i_val << f_val << std::endl;    // a, 29, 1.86
std::tie(c_val, ignore, f_val) = t;  // ignore the 2nd element
```

#### `array`

`std::array` has a **fixed size**, is able to random access the element at a constant time. It provides access to the underlying raw array. Use the `std::array` object whenever possible instead of using raw arrays. All iterators are available and they won't invalidate.

```c++
std::array<int, 5> arr1 {1, 2, 3, 4, 5};
std::array<int, 5> arr2 {10, 20, 30, 40, 50};

arr1.empty();       // false (0)
arr1.size();        // 5

arr1.at(0);         // 1, has boundary check
arr1[0];            // 1, no boundary check
arr1.front();       // 1
arr1.back();        // 5

arr1.fill(10);      // fill all to 10
arr2.swap(arr1);    // swap two arrays
int *ptr = arr1.data(); // get the address to the raw array
```

#### `vector`

`std::vector` likes an array but has a **dynamic size**, and elements are stored in the contiguous memory space. It has constant time of random access to element, constant time insertion/deletion at the back, linear time of insertion/removal of any element. All iterators are available but may invalidate when resizing.

```c++
/* Initialization */
std::vector<int> vec1{1,2,3,4,5};
std::vector<int> vec2(N, 0); // init a vector with N elements of 0
std::vector<std::ector<int>> matrix(N, std::vector<int>(M, 0)); // init NxM matrix filled with all 0

/* Size */
vec1.size();        // number of elements, 5
vec1.capacity();    // the actual memory space allocated that may larger than size, 5
vec1.max_size();    // max size that system allows a vector to have

vec1.push_back(6);  // add 6 to the back
vec1.capacity();    // system may allocate more space than required, let's say 10
vec1.reserve(40);   // reserve additional space for vector, now capacity() == 50
vec1.shrink_to_fit();   // this force size() == capacity(), now capacity() == 6

/* Indexing */
vec1.at(0);         // boundary check, 1
vec1[0];            // no boundary check, 1
vec1.front();       // 1
vec1.back();        // 6

/* Modification */
vec1.push_back(7);  // add 7 to the back
vec1.pop_back();    // remove the last element

vec1.erase(vec1.begin(), vec1.begin()+2);   // 4,5
vec1.clear();       // remove all

/* Common methods */
std::vector<int> vec3{1,2,7,8};
std::vector<int> vec4{4,5,6};
auto it = std::find(vec3.begin(), vec3.end(), 7);   // get iterator
vec3.insert(it, 3); // 1,2,3,7,8
std::insert(it, vec4.begin(), vec4.end()); // 1,2,3,4,5,6,7,8

std::vector<int> vec5{1,2,3,4};
std::vector<int> vec6{5,6};
std::copy(vec6.begin(), vec6.end(), std::back_inserter(vec5)); // vec5 == {1,2,3,4,5,6}

std::vector<int> vec7{1,2,3,4};
std::vector<int> vec8{0};
std::copy_if(vec7.begin(), vec7.end(), std::back_inserter(vec8), [](int x){ return x%2 == 0; }); // vec8 == {0,2,4}

std::vector<int> vec9{1,2,3};
std::vector<int> vec10{1,2,3};
std::vector<int> vec11;
std::transform(vec9.begin(), vec9.end(), vec10.begin(), std::back_inserter(vec11), [](int x, int y){ return x*y; }); // vec11 == {1,4,9}
```

> Q: `push_back(MyClass{argu1, argu2})` vs. `emplace_back(argu1, argu2)`?
> Difference 1 input argument: when vector type is a user-defined type with multiple arguments for constructor, `push_back()` requires we pass an object of that type to it. But we can simply pass the arguments of the constructor directly to `emplace_back()`.
> Difference 2 efficiency: for built-in types, two methods have no difference. But for user-defined types, `push_back()` requires a temporary object to be created and destroyed while `emplace_back()` avoids using the temporary object.

#### `deque`

`std::deque` likes a vector but its elements are NOT stored in contiguous memory. The underlying implementation is like a linked-list vectors so it supports constant time of insertion/deletion at the front as well at back. Other characteristics are similar to vector.

Common methods: `push_back()`, `pop_back()`, `push_front()`, `pop_front()`, `emplace_back()`, `emplace_front()`

#### `list`

STL implement the *list* as a *doubly linked list* with dynamic size. Random element access is NOT provided. Insertion and deletion of elements anywhere in the container is constant time. All iterators are available but they invalidate when corresponding element is deleted.

Common methods: `push_back()`, `pop_back()`, `push_front()`, `pop_front()`, `emplace_back()`, `emplace_front()`

```c++
std::list<int> l1{1,2,3,4,5};
std::list<int> l2(10,0);
l1.size();          // 5
l1.max_size();      // a very large number
l1.front();         // 1
l1.back();          // 5

auto it1 = std::find(l1.begin(), l1.end(), 3);
l1.insert(it1, 10);         // insert element before the iterator: 1,2,10,3,4,5
l1.erase(it1);              // remove the iterator element: 1,2,10,4,5
l1.resize(2);               // remove the elements exceeding the size: 1,2
l1.resize(5);               // fill the list with 0: 1,2,0,0,0
```

#### `forward_list`

STL implement the *list* as a *singly linked list* with dynamic size. It has less overhead than a `std::list`. Random element access is NOT provided. Insertion and deletion of elements anywhere in the container is constant time. Reverse iterators are NOT available. All usable iterators invalidate when corresponding element is deleted.

Common methods: `push_front()`, `pop_front()`, `emplace_front()`. No concept of **back**.

```c++
std::forward_list<int> l1{1,2,3,4,5};
auto it1 = std::find(l1.begin(), l1.end(), 3);
l1.insert_after(it1, 10);   // insert element after the iterator: 1,2,3,10,4,5
l1.erase_after(it1);        // remove the element after iterator: 1,2,3,4,5
```

#### `queue`

*Queue* is FIFO.

```c++
std::queue<int> q;
q.push(1);
q.pop();
```

#### `priority_queue`

*Priority queue* provides a constant time extraction of the largest element but logarithmic insertion. The internal implementation is *binary-max-heap*. We cannot iterate on the priority queue, only getting the top element and pop it.

```c++
std::priority_queue<int> pq;
pq.push(1);
pq.top();
pq.pop();
std::priority_queue<int, std::vector<int>, std::greater<int>> pq_min;  // binary-min-heap
```

#### `stack`

*Stack* is LIFO, like a pile of books.

```c++
std::stack<int> s;
s.push(1);
s.pop();
```

#### `set`

*Set* has no duplicate elements, maintaining the elements sorted internally so it's not necessary to call `sort()` method again and again after adding a new element. It's ordered by key, that is for a user-defined class, it must supply with an overloaded `operator<` for comparison. All iterators are available but they invalidate when corresponding element is deleted.

No concept of **front** or **back**.

> `insert()` returns a `std::pair<iterator, bool>`: iterator points to the inserted element or to the duplicate in the set, boolean indicates success or duplicate.

```c++
std::set<int> s1{1,1,1,2,3,4,5};        // 1,2,3,4,5
s1.size();  // 5
ret = s1.insert(1);     // ret=(iterator_to_1, false_because_1_is_dup)

s1.erase(5);            // 1,2,3,4
auto it = s1.find(4);   // 1,2,3
if (it != s1.end())
    s1.erase(it);

s1.count(1);            // easy way to check if 1 exists
s1.clear();             // remove all
s1.empty();             // check if empty
```

`multi_set`: sorted by key, allowing duplicates, all iterators are available.
`unordered_set`: unordered elements, no duplicates, elements cannot be modified (must erase and then insert a new one), no reverse iterators.
`unordered_multiset`: unordered elements, allowing duplicates, no reverse iterators.

#### `map`

A *map* stores elements as key/value pairs, ordered by key, no duplicates because keys are unique. Random access to the element value is available via its key. Internally it is implemented as *Binary Search Tree*. All iterators are available but they invalidate when corresponding element is deleted.

```c++
std::map<int, int> m = {{1,100}, {2,99}};
/* Three ways of insertion, O(logN) */
m.insert(std::make_pair(3,113));
m.insert(std::pair<int, int>(4,21));
m[5] = 89;

/* Update an element value */
m[5] = 79;
m.at(5) = 69;

/* Remove an element */
m.erase(5);
auto it = m.find(4);
if (it != m.end())
    m.erase(it);

/* Check if 4 exists */
m.count(4);
if (m.find(4) != m.end())
    std::cout << "Found 4!";

m.clear();  // remove all
m.empty();  // check if empty
```

`multi_map`: ordered by key, allowing duplicates, all iterators are available.
`unordered_map`: A *unordered_map* is the same as *map* in terms of the functionalities. But it is implemented as an *array* internally. The **key** will be passed into a **hashing function** and the **index** to the corresponding **value** is calculated. The insertion `O(1)` is faster than *map*'s `O(logN)`. No duplicates, no reverse iterators.
`unordered_multimap`: unordered elements, allowing duplicates, no reverse iterators.

## 3. Iterators

*Iterators* are used for working upon a sequence of elements from containers, e.g. forward, reverse, by value, by reference, constant, etc. *Iterators* work like *pointers* by design, most of the container classes can be traversed with iterators.

1. Iterators must be declared based on the container type that they will iterate over
2. Iterator `.begin()` method points to the **first element**
3. Iterator `.end()` method points to the location **after the last element**

![Iterator Operations](./iterator-operations.png)

```c++
std::vector<int> vec {1,2,3};
std::vector<int>::iterator it1 = vec.begin();           // declare with container type
for (auto it2 = vec.begin(); it != vec.end(); ++it) {   // declare with auto
    std::cout << *it << " ";
}
std::advance(it1, 2);   // move iterator to the right, or negative value to move to the left
```

#### Other iterators

- `const_iterator`: cannot change the values that iterators point to
- `reverse_iterator`: reverse the direction of a regular iterator
- `const_reverse_iterator`

#### Iterator invalidation

Iterator is like a raw pointer that could be invalidated, pointing to junk data. In the following code, no compiler error and no warning will be reported. Developer should watch for such errors and prevent them, e.g. `insert()`, `erase()`, `resize()`.

```c++
for (auto it = map.begin(); it != map.end(); ++it) {
    map.erase(it->first); // map has been restructured and iterator still thinks itself is healthy
}
```

## 4. Functions

The STL includes classes that overload the function call operators. Instances of such classes are *function objects* or *functors*.