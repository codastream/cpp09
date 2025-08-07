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

static bool	_tryConvertDouble(const std::string& s, double* converted)
{
	char* end;
	double value = std::strtod(s.c_str(), &end);
	if (value < 0 || value > std::numeric_limits<double>::max())
		return false;
	*converted = value;
}

static std::string _tmToString(const std::tm& time, const char* format)
{
	char buffer[100];
	std::strftime(buffer, sizeof(buffer), format, &time);
	return std::string(buffer);
}

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
	if (!_isLeapYear(time) && time.tm_mon == 2 && time.tm_mday >= 28)
		return false;
	return true;
}

bool	BitcoinExchange::_parseDate(std::string& s, tm* time)
{
	std::istringstream is(s);
	is >> std::get_time(time, _expectedDateFormat);
	if (is.fail())
	{
		std::stringstream ss;
		std::string msg;
		ss << RED << "error parsing date with expected format " << _expectedDateFormat << NC << std::endl;
		ss >> msg;
		throw ParseException(msg);
	}
	return true;
}

bool	BitcoinExchange::_parseLine(std::string& s, char sep, int linenb, timeVal_t& dic)
{
	int	sepIndex = s.find(sep);
	if (sepIndex == std::string::npos)
	{
		std::stringstream ss;
		std::string msg;
		ss << RED << "error parsing line " << linenb << " : missing separator " << sep << NC << std::endl;
		ss >> msg;
		throw ParseException(msg);
	}
	std::string	date = s.substr(0,sepIndex);
	std::tm time = {};
	if (!_parseDate(date, &time))
		return false;
	if (!_isValidDate(time))
	{
		std::stringstream ss;
		std::string msg;
		ss << RED << "invalid date " << _tmToString(time, _expectedDateFormat) << NC << std::endl;
		ss >> msg;
		throw ParseException(msg);
	}
	std::string	value = s.substr(sepIndex + 1, s.npos);
	double converted;
	if (!_tryConvertDouble(value, &converted))
	{
		std::stringstream ss;
		std::string msg;
		ss << RED << "value out of double bounds" << NC << std::endl;
		ss >> msg;
		throw ParseException(msg);
	}
	if (dic.find(time) != dic.end())
		{
		std::stringstream ss;
		std::string msg;
		ss << RED << "duplicate entries for same date " << _tmToString(time, _expectedDateFormat) << NC << std::endl;
		ss >> msg;
		throw ParseException(msg);
	}
	dic.insert(std::pair<std::tm, double>(time, converted));
}
/// @brief 
/// @throw ParseException
/// @param filename 
/// @param isInput 
/// @return 
bool	BitcoinExchange::_parseFile(const std::string& filename, bool isInput)
{
	std::ifstream fs(filename);
	if (!fs.is_open())
	{
		std::stringstream ss;
		std::string msg;
		ss << RED << "error opening file " << filename << NC << std::endl;
		ss >> msg;
		throw ParseException(msg);
	}
	size_t linenb = 1;
	char sep = ',';
	timeVal_t dic = _rates;
	if (isInput)
	{
		dic = _input;
		sep = '|';
	}
	for (std::string line; std::getline(fs, line);)
	{
		if (!_parseLine(line, sep, linenb, dic))
			return false;
		linenb++;
	}
	fs.close();
}

timeVal_t	BitcoinExchange::_parseRates(const std::string& datafile)
{
	_parseFile(datafile, false);
}
timeVal_t	BitcoinExchange::_parseInput(const std::string& inputfile)
{
	_parseFile(inputfile, true);
}

double	BitcoinExchange::_getRateForClosestLowerDate(const std::tm& time)
{
	double				rate;
	timeVal_t::iterator	it;

	it = _rates.find(time);
	if (it == _rates.end())
		return *it;
	
}

void	BitcoinExchange::printValues(const std::string& inputfile)
{
	try
	{
		_rates = _parseRates("../data.csv");
	}
	catch(const std::exception& e)
	{
		std::cerr << "error loading rates due to " << e.what() << std::endl;
	}
	
	try
	{
		_input = _parseInput(inputfile);
	}
	catch(const std::exception& e)
	{
		std::cerr << "error reading input due to " << e.what() << std::endl;
	}

	for (timeVal_t::iterator it = _input.begin(); it != _input.end(); it++)
	{

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

const char*		BitcoinExchange::ParseException::what(void) const throw()
{
	return _msg;
}