#include "BitcoinExchange.hpp"
#include "util.hpp"

/************************************************************
*				🥚 CONSTRUCTORS & DESTRUCTOR				*
************************************************************/

BitcoinExchange::BitcoinExchange(void) : _input(), _rates() 
{
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& inst) : _input(inst._input), _rates(inst._rates)
{
}

BitcoinExchange::~BitcoinExchange(void) 
{
}

/************************************************************
*				➕ OPERATORS									*
************************************************************/

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& inst) 
{
	if (this != &inst)
	{
		_input = inst._input;
		_rates = inst._rates;
	}
	return (*this);
}

/*************************************************************
*		        🛠️ FUNCTIONS								*
*************************************************************/

// static void ltrim(std::string& s)
// {
// 	std::string::iterator it = s.begin();
// 	while (it != s.end() && std::isspace(*it))
// 		++it;
// 	s.erase(s.begin(), it);
// }

// static void rtrim(std::string& s)
// {
// 	std::string::reverse_iterator it = s.rbegin();
// 	while (it != s.rend() && std::isspace(*it))
// 		++it;
// 	s.erase(it.base(), s.end());
// }

// static void trim(std::string& s)
// {
// 	ltrim(s);
// 	rtrim(s);
// }

static void	_puterr(const std::string& s, const std::string& faulty)
{
	const std::string& msg = faulty.empty() ? s : s + " => " + faulty;
	std::cerr << RED << "Error: " << msg << NC << std::endl;
}

/// @brief checks decimal value
/// @param s 
/// @param converted 
/// @return false if number can't be converted to a positive decimal value < INT MAX
static bool	_tryConvertDouble(const std::string& s, double* converted, bool isInput)
{
	char* end;
	bool is_err = true;
	double value = std::strtod(s.c_str(), &end);
	if (isInput && value < 0)
		_puterr("not a positive number", "");
	else if (isInput && value > std::numeric_limits<int>::max())
		_puterr("too large a number", "");
	else
		is_err = false;
	if (is_err)
		return false;
	*converted = value;
	return true;
}

static std::string _tmToString(const std::tm& time, const char* format)
{
	char buffer[100];
	if (std::strftime(buffer, sizeof(buffer), format, &time) == 0)
		throw std::runtime_error("conversion error");
	return std::string(buffer);
}

// static std::string _timeToString(const std::time_t& key, const char* format)
// {
// 	const std::tm* time = std::localtime(&key);
// 	if (!time)
// 		throw std::runtime_error("conversion error");
// 	return (_tmToString(*time, format));
// }

static bool	_isLeapYear(std::tm time)
{
	if (time.tm_year % 400 == 0)
		return true;
	else if (time.tm_year % 100 == 0)
		return false;
	else if (time.tm_year % 4 == 0)
		return true;
	else
		return false;
}

static bool _isValidDate(std::tm time)
{
	if (!_isLeapYear(time) && time.tm_mon == 1 && time.tm_mday >= 29)
		return false;
	return true;
}

static void _parseDate(std::string& s, std::tm* time, time_t* key)
{
	const char*	dateStr = s.c_str();
	if (!strptime(dateStr, ISO, time))
		throw std::runtime_error("conversion error");
	*key = std::mktime(time);
	if (*key == -1)
		throw std::runtime_error("conversion error");
}

bool	BitcoinExchange::_parseLine(std::string& s, char sep, timeVal_t& dic, bool isInput)
{
	size_t	sepIndex = s.find(sep);
	if (sepIndex == std::string::npos)
		return false;
	std::string	date = s.substr(0, sepIndex);
	std::tm time = {};
	std::time_t key;
	_parseDate(date, &time, &key);
	if (!_isValidDate(time))
		return false;
	std::string	value = s.substr(sepIndex + 1, s.npos);
	double converted;
	_tryConvertDouble(value, &converted, isInput);
	dic.insert(std::pair<std::time_t, double>(key, converted));
	return true;
}

static bool	checkHeader(bool isInput, std::ifstream& ifs)
{
	std::string line;
	std::getline(ifs, line);

	if (!isInput && line != "date,exchange_rate")
		return false;
	else if (isInput)
	{
		std::stringstream ss(line);
		std::string title1, sep, title2;
		ss >> title1 >> sep >> title2;
		if (title1 != "date" || sep != "|" || title2 != "value")
			return false;
	}
	return true;
}

/// @brief 
/// @throw ParseException
/// @param filename 
/// @param isInput 
/// @return 
bool	BitcoinExchange::_parseRates(const char* filename)
{
	std::ifstream fs(filename);
	if (!fs.is_open())
		throw std::runtime_error("error opening file");
	if (!checkHeader(false, fs))
		_puterr("bad header", "");
	size_t linenb = 2;
	for (std::string line; std::getline(fs, line);)
	{
		_parseLine(line, ',', _rates, false);
		linenb++;
	}
	fs.close();
	return true;
}

void BitcoinExchange::_printValue(std::tm time, double qty)
{
	std::string	fm_time;
	time_t		key;
	double		rate;
	double		totalValue;
	
	fm_time = _tmToString(time, ISO);
	key = std::mktime(&time);
	if (key == -1)
		throw std::runtime_error("conversion error");
	rate = _getRateForClosestLowerDate(key);
	totalValue = qty * rate;
	std::cout << CYAN << fm_time << NC << " => " << qty << " = " << BLUE << totalValue << NC << std::endl;
}

void	BitcoinExchange::_printFromInput(const char* filename)
{
	double qtyD;
	std::ifstream fs(filename);
	if (!fs.is_open())
		throw std::runtime_error("error opening file");
	if (!checkHeader(false, fs))
		_puterr("bad header", "");
	size_t linenb = 2;
	for (std::string line; std::getline(fs, line);)
	{
		std::stringstream ss(line);
		std::string dateS, sep, qtyS;
		ss >> dateS >> sep >> qtyS;
		std::tm time = {};
		std::time_t key;
		_parseDate(dateS, &time, &key);
		if (!_isValidDate(time) || sep.empty())
			_puterr("bad input", line);
		if (_tryConvertDouble(qtyS, &qtyD, true))
			_printValue(time, qtyD);
		linenb++;
	}
}

double	BitcoinExchange::_getRateForClosestLowerDate(const std::time_t& time)
{
	timeVal_t::iterator	it;

	it = _rates.lower_bound(time);
	if (it == _rates.end())
		return 0;
	else
		return it->second;
}

void	BitcoinExchange::printValues(const char* inputfile)
{
	try
	{
		_parseRates("data.csv");
	}
	catch(const std::exception& e)
	{
		std::cerr << "error loading rates due to " << e.what() << NC << std::endl;
	}

	try
	{
		_printFromInput(inputfile);
	}
	catch(const std::exception& e)
	{
		std::cerr << "error reading input due to " << e.what() << NC << std::endl;
	}
}

/*************************************************************
*		    👁️‍ GETTERS and SETTERS				 			*
*************************************************************/

/*************************************************************
*		    ❗ NESTED EXCEPTION CLASS						*
*************************************************************/

BitcoinExchange::ParseException::ParseException(const std::string& msg) : _msg(msg.c_str()) {}

BitcoinExchange::ParseException::~ParseException() throw() {}

const char*	BitcoinExchange::ParseException::what(void) const throw()
{
	return _msg.c_str();
}