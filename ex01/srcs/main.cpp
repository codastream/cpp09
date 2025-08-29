/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpetit <fpetit@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 13:21:53 by fpetit            #+#    #+#             */
/*   Updated: 2025/08/29 10:57:18 by fpetit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"
#include "util.hpp"

bool	checkArgs(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << RED << "usage: ./RPN <(> operand1 operand2 operator <)> ..." << NC << std::endl;
		return false;
	}
	std::string arg = av[1];
	if (arg.size() < 5)
	{
		std::cerr << RED << "usage: ./RPN <(> operand1 operand2 operator <)> ..." << NC << std::endl;
		return false;
	}
	return true;
}

int	main(int ac, char **av)
{
	if (!checkArgs(ac, av))
		return 1;
	RPN calc;
	return (calc.compute(av[1]));
}
