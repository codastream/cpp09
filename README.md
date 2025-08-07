#cpp 09

## time formatting

- format string syntax
	- %Y for 
- we use a `std::tm` struct to access separate values of year, month, ...
- parsing errors should be checked with `std::sstream::fail()`

```c++
#include <iomanip>	// get_time
#include <sstream>	// isstream
#include <ctine>	// std::tm

std::string datestr = "2000-02-28";
std::tm t = {};
std::istringstream is(datestr);
ss >> std::get_time(&t, "%Y-%m-%d");

```

## usage of map

https://cplusplus.com/reference/utility/make_pair/

## Sources
https://cplusplus.com/reference/iomanip/get_time


## Evolutions in c++11

## Evolutions in c++20
- `<format>` can be used instead of `<iomanip>`
- `<chrono>` with `parse`