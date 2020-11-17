# C++ Standard Template Library (STL)

STL is a set of C++ template classes to provide common data structures and functions.

Four STL components:
- Algorithms
- Containers
- Functions
- Iterators

## 1. Algorithms

The header `<algorithm>` defines many commonly used algorithms for processing sequences of elements from containers, e.g. `find`, `max`, `sort`, `accumulate`, `count`, etc.

### Non-modifying

```c++
/* for_each() allows any unary function */
vector<int> nums = {1,2,3};
auto print = [](const int& n) {cout << n;};
for_each(nums.begin(), nums.end(), print);

/* find() the 1st element in the container */
auto it = find(nums.begin(), nums.end(), 2);   // if found 2, it!=nums.ends()

/* search() the 1st occurrence of the sequence of elements */
vector<int> v1 = {9,1,2,3,4};
auto it = search(v1.begin(), v1.end(), nums.begin(), nums.end());
if (it != v1.end()) {
    cout << "nums is presented at index " << distance(v1.begin(), it) << endl;
}

/* min_element(), max_element() returs the min/max element */
vector<int>::iterator it = min_element(v1.begin(), v1.end());
```

### Modifying

```c++
/* copy() a range of elements to another range */
copy(src_vector.begin(), src_vector.end(), dest_vector.begin());

/* fill() a range of elements with specific value */
fill(v.begin(), v.end(), -1);

/* reverse() elements in a range */
reverse(v.begin(), v.end());

/* swap() interchanges the values of two elements */
swap(a, b);

/* move() efficiently transfers the resource to another object */
/* move operation leaves the moved-from object valid but with unspecified value */
/* after moved, the moved-from object should be destroyed or assigned a new value */
string str = "hello";
vector<string> v;
v.push_back(str);       // copy str to vector
v.push_back(move(str)); // content of str is moved to vector, after which str might be empty
```

### Sorting and Searching

```c++
/* sort() uses default operator < */
sort(v.begin(), v.end());
sort(v.begin(), v.end(), greater<int>());   // use operator >, descending order

/* binary_search() finds the matching value */
bool found = binary_search(v.begin(), v.end(), val);

/* merge() combines two sorted ranges into one sorted range */
merge(v1.begin(), v1.end(), v2.begin(), v2.begin(), dst.begin());
merge(v1.begin(), v1.end(), v2.begin(), v2.begin(), back_inserter(dst));
```

### Numeric

```c++
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


#### `pair`

```c++
pair<int, char> p1 = make_pair(1, 'a');
pair<int, pair<int, char>> p2 = {1, {2, 'b'}};
cout << p1.first << p1.second << endl;  // 1, a
cout << p2.second.second << endl;       // b
```

#### `tuple`

*Tuple* is an object that can hold a number of elements.

```c++
tuple<char, int, float> t;
t = make_tuple('a', 29, 1.78);

cout << get<0>(t) << endl;  // access the 1st element
get<2>(t) = 1.86;           // modify the 3rd element

tie(c_val, i_val, f_val) = t;   // unpack tuple values into separate variables
cout << c_val << i_val << f_val << endl;    // a, 29, 1.86
tie(c_val, ignore, f_val) = t;  // ignore the 2nd element
```

#### `vector`

*Vector* is an array but with a lot of extra functionanlities.

```c++
vector<int> v(N, 0);    // init a vector with N elements of 0
vector<vector<int>> matrix(N, vector<int>(M, 0));    // init NxM matrix filled with all 0
```

#### `list`

STL implement the *list* as a *doubly linked list*.

```c++
list<int> l;
l.push_back(1);
l.push_front(2);
```

#### `forward_list`

STL implement the *list* as a *singly linked list*.

#### `queue`

*Queue* is FIFO.

```c++
queue<int> q;
q.push(1);
q.pop();
```

#### `priority_queue`

*Priority queue* provides a constant time extraction of the largest element but logarithmic insertion. The internal implementation is *binary-max-heap*. We cannot iterate on the priority queue, only getting the top element and pop it.

```c++
priority_queue<int> pq;
pq.push(1);
pq.top();
pq.pop();
priority_queue<int, vector<int>, greater<int>> pq_min;  // binary-min-heap
```

#### `stack`

*Stack* is LIFO, like a pile of books.

```c++
stack<int> s;
s.push(1);
s.pop();
```

#### `set`

*Set* maintains elements sorted internally so it's not necessary to call `sort()` method again and again after adding a new element.

#### `unordered_set`

#### `map`

A *map* maps the keys to values so that the values can be accessed via their keys. Internally it is implemented as *Binary Search Tree*.

```c++
map<int, int> m = {{1,100}, {2,99}};
// O(logN) for insertion
m.insert(make_pair(3,113));
m[4] = 89;
```

#### `unordered_map`

A *unordered_map* is the same as *map* in terms of the functionalities. But it is implemented as an *array* internally. The **key** will be passed into a **hashing function** and the **index** to the corresponding **value** is calculated. The insertion `O(1)` is faster than *map*'s `O(logN)`.

## 3. Iterators

*Iterators* are used for working upon a sequence of elements from containers, e.g. forward, reverse, by value, by reference, constant, etc.

## 4. Functions

The STL includes classes that overload the function call operators. Instances of such classes are *function objects* or *functors*.