/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpetit <fpetit@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 13:21:53 by fpetit            #+#    #+#             */
/*   Updated: 2025/08/07 14:16:51 by fpetit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"
#include "util.hpp"

bool	checkArgs(int ac, std::string **av)
{
	if (ac != 2)
		std::cerr << RED << "usage: ./btc <data.csv>" << NC << std::endl;
	std::string filename = *av[1];
	if (filename.length() <= 4 || filename.compare(filename.length() - 4, 4, ".txt"))
	{
		std::cerr << RED << "provided file should be a .txt" << NC << std::endl;
		return false;
	}
	return true;
}

int	main(int ac, std::string **av)
{
	if (!checkArgs(ac, av))
		return 1;
	
}
