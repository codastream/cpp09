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
#include <cstdlib>
#include <locale>
#include <cstring>

#define ISO "%Y-%m-%d"

typedef std::map<std::time_t, double> timeVal_t;

class BitcoinExchange {

	public:
		BitcoinExchange();
		virtual ~BitcoinExchange();
		BitcoinExchange(const BitcoinExchange& other);
		BitcoinExchange& operator=(const BitcoinExchange& other);
		
		void	printValues(const char* inputfile);

		class ParseException : public std::exception
		{
			private:
				std::string	_msg;
		
			public:
				ParseException(const std::string& msg);
				virtual ~ParseException() throw();
				virtual const char*	what() const throw();
		};

	private:
		timeVal_t	_input;
		timeVal_t	_rates;

		// static const char*	_expectedDateFormat;
		// void		_parseDate(std::string& s, std::tm *time, time_t* key);
		bool		_parseLine(std::string& s, char sep, timeVal_t& map, bool isInput);
		bool		_parseRates(const char* filename);
		void		_printFromInput(const char* filename);
		void		_printValue(std::tm time, double qty);
		double		_getRateForClosestLowerDate(const std::time_t& time);
};

#endif