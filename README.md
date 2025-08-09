#cpp 09

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

## Sources
https://cplusplus.com/reference/iomanip/get_time


## Evolutions in c++11
- `std::chrono`

## Evolutions in c++20
- `<format>` can be used instead of `<iomanip>`
- `<chrono>` with `parse`