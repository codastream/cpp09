NAME		:=	PmergeMe

SRCS_FILES:=	main.cpp\
				util.cpp\
				PmergeMe.cpp\

include ../ex00/common.mk

printstep: CXXFLAGS += -DPRINTSTEP=1
printstep: clean all

.PHONY: all clean fclean re $(DIRS) printstep
