NAME		:=	PmergeMe

SRCS_FILES:=	main.cpp\
				utils.cpp\
				debug.cpp\
				PmergeMe.cpp\

include ../ex00/common.mk

debug: CXXFLAGS += -DDEBUG=1
debug: clean all

.PHONY: all clean fclean re $(DIRS) printstep
