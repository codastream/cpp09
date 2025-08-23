#cpp 09

## iterators
iterators provide a homogenic interface when working with containers. in other word, they help decouple algorithms from container implementations (eg traversal of a container implemented with contiguous memory vs one implemented as a binary tree).

### 5 kinds of iterator
|				|input	|output		|forward|bidirectional	|random access	|
|---			|--- 	|---   		|---	|---			|---		|
|read			|✅	   |❌		|✅		|✅  			|✅  	|
|write			|❌		|✅		|✅  	|✅  			|✅  	|
|multiple pass	|❌		|❌		|✅  	|✅  			|✅  	|
|++i / ++i		|✅  	|✅  	|✅  	|✅  			|✅  |
|--i / --i		|✅  	|✅  	|❌  	|✅  			|✅  |
|*i	/ ->		|✅  	|✅  	|✅  	|✅  			|✅  |
|== / !=		|✅  	|✅  	|✅  	|✅  			|✅  |
|provided with		|  	|  	|  	|list, set, map..  			|vector, deque  |
|pointer like arithmentic i += n, it[n], it1 - it2, <, ..|❌|❌|❌|❌|✅|

### reverse iterator

```c++
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v; for (int i=0;i<5;++i) v.push_back(i);
    for (std::vector<int>::reverse_iterator it = v.rbegin(); it != v.rend(); ++it) {
        std::cout << *it << " ";
    }
}
```

### iterator invalidation

- `vector`
 - `push_back` may reallocate and invalidate all iterators
 - `insert` invalidates at the point
 - `erase` invalidate after the point
- `deque`
 - `insert`, `erase` may invalidate many
  - `push_back`, `push_front` may invalidate
- `list`, associative containers (`set`, `map`)
 `insert`, `erase` only invalidate iterators to erased elements

### uses of iterators

- for normal or reverse traversal
- most of std::algorithm (find, count, for_each, copy, sort...) are range-based and work on iterators although some containers require a specific implementation. See also _iterator adaptators_ that call container functions with iterators (useful for ranges) : `back_inserter`, `front_inserter`, `inserter`


### caveats

- don't dereference end()
- don't mix const_iterator and iterator
- don't read from output iterator
- always reassign or recompute iterators after invalidating modifications
- don't erase in a loop
- dont do arithmetic pointer like operation on non random access iterators. Use `std::advance` 

### improvements in c++11/14

- range-based for
- auto for iterator types
- cbegin / cend / crbegin / crend for const iterators
- new iterator adapters : `std::move_iterator`, `std::make_move_iterator`

## recap on file i/o
```c++
#include <fstream>
#include <iostream>
// io stream with input mode
std::fstream fs("file.ext", std::ios::in);
// alternative
std::fstream fs;
fs.open("file.ext", std::ios::in);

// more simple : input stream
std::ifstream ifs("file.ext");

// testing errors
if (!fs) {...}
if (!fs.is_open()) {...}
```

https://en.cppreference.com/w/cpp/io/basic_ifstream.html

## recap on string processing
```c++
#include <string>
#include <sstream>
std::string line;
std::stringstream ss(line);

// we can specify a delimiter other than \n
std::getline(ss, line);
std::getline(ss, line, ',');

// tokenization on whitespaces using string stream
std::string token1, sep, token2;
ss >> token1 >> sep >> token2;
```

## recap on conversion
- `std::stod`, ...

## time formatting

- format string syntax : Y-m-d
- we use a `std::tm` struct to access separate values of year, month, ...

## time measure

```c++
#include <ctime>

clock_t start = clock();
clock_t end = clock();
double elapsed = double(end / start) / CLOCKS_PER_SEC;
```

## usage of map
- specific methods : `key_comp`, `value_comp`
https://en.cppreference.com/w/cpp/container/map.html 
https://cplusplus.com/reference/utility/make_pair/

## usage of stack
- specific methods : `push`, `pop`, `top`
https://en.cppreference.com/w/cpp/container/stack.html 

## usage of vector
https://en.cppreference.com/w/cpp/container/vector.html

## usage of deque
- specific methods : `push_front`, `pop_front` (same as list)
https://en.cppreference.com/w/cpp/header/deque.html

## usage of list
- specific methods : `merge`, `splice`, `remove`, `remove_if`, `reverse`, `unique`, `sort`
https://en.cppreference.com/w/cpp/header/list.html

## recap on algorithm

Merge insertion sort was created in 1959 by two mathematicians (Lester Ford)[https://en.wikipedia.org/wiki/L._R._Ford_Jr.] and (Selmer Johnson)[https://en.wikipedia.org/wiki/Selmer_M._Johnson]. It was aiming at minimizing the number of comparisons, and achieves to do so for small datasets (< 12 elems) for which it reaches O(n logn), the theoretical minimal comparison number.

First step (merge) consists of presorting values by pairs of min and max. Second step (insert) consists of sorting the max elements, then carefully inserting min elements in a certain order. The Jacobstahl sequence grows in the same proportion as the sorted list, granting an homogenous distribution and coincidentally limiting the number of comparisons. The algorithm is recursive.

## Sources
https://cplusplus.com/reference/iomanip/get_time


## Evolutions in c++11
- `std::chrono`

## Evolutions in c++20
- `<format>` can be used instead of `<iomanip>`
- `<chrono>` with `parse`