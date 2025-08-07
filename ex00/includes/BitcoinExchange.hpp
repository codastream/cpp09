#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <iostream>
#include <string>
#include <map>
#include <ctime>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <limits>

typedef std::map<std::tm, double> timeVal_t;

class BitcoinExchange {

	public:
		BitcoinExchange();
		virtual ~BitcoinExchange();
		BitcoinExchange(const BitcoinExchange& other);
		BitcoinExchange& operator=(const BitcoinExchange& other);
		
		void	printValues(const std::string& inputfile);

	private:
		timeVal_t	_input;
		timeVal_t	_rates;

		const char*	_expectedDateFormat = "%Y-%m-%d";

		timeVal_t	_parseRates(const std::string& datafile);
		timeVal_t	_parseInput(const std::string& inputfile);
		bool		_parseDate(std::string& s, tm* time);
		bool		_parseLine(std::string& s, char sep, int linenb, timeVal_t& map);
		bool		_parseFile(const std::string& filename, bool isInput);
		double		_getRateForClosestLowerDate(const std::tm& time);
		class ParseException : public std::exception
		{
			private:
				const char* _msg;
		
			public:
				ParseException(const std::string& msg);
				virtual ~ParseException() throw();
				virtual const char*		what(void) const throw();
		};
};

#endif