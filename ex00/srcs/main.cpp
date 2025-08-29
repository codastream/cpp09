/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpetit <fpetit@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 13:21:53 by fpetit            #+#    #+#             */
/*   Updated: 2025/08/29 14:14:11 by fpetit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"
#include "util.hpp"

bool	checkArgs(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << RED << "usage: ./btc <input.csv>" << NC << std::endl;
		return false;
	}
	std::string filename = av[1];
	if (filename.length() <= 4 || filename.compare(filename.length() - 4, 4, ".csv"))
	{
		std::cerr << RED << "please provide an input.csv" << NC << std::endl;
		return false;
	}
	return true;
}

// TODO check closest lower date
int	main(int ac, char **av)
{
	if (!checkArgs(ac, av))
		return 1;
	BitcoinExchange btc;
	try
	{
		btc.printValues(av[1]);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}	
}
