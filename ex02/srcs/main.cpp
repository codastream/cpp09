/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpetit <fpetit@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 13:21:53 by fpetit            #+#    #+#             */
/*   Updated: 2025/08/10 23:44:04 by fpetit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"
#include "util.hpp"

bool	checkArgs(int ac, char **av)
{
	long vall;
	int	val;
	char *end;

	if (ac < 2)
	{
		std::cerr << RED << "usage: ./RPN <(> operand1 operand2 operator <)> ..." << NC << std::endl;
		return false;
	}
	for (int i = 1; i < ac; i++)
	{
		vall = std::strtol(av[i], &end, 10);
		if (vall > std::numeric_limits<int>::max() || vall < 0)
			return false;
		val = vall;
	}
	return true;
}

int	main(int ac, char **av)
{
	if (!checkArgs(ac, av))
	{
		puterr();
		return 1;
	}
	PmergeMe mi;
	mi.initVec(--ac, ++av);
	mi.doSort();
}
