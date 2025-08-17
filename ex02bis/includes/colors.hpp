#ifndef COLORS_H
#define COLORS_H

#include <iostream>
#include <string>

// reset
#define NC				"\033[0m"

// foreground colors
#define BLACK			"\033[30m"
#define RED				"\033[31m"
#define GREEN			"\033[32m"
#define YELLOW			"\033[33m"
#define BLUE			"\033[34m"
#define PURPLE			"\033[35m"
#define CYAN			"\033[36m"
#define WHITE			"\033[37m"
#define DEFAULT			"\033[39m"

// background colors
#define BG_BLACK		"\033[40m"
#define BG_RED			"\033[41m"
#define BG_GREEN		"\033[42m"
#define BG_YELLOW		"\033[43m"
#define BG_BLUE			"\033[44m"
#define BG_PURPLE		"\033[45m"
#define BG_CYAN			"\033[46m"
#define BG_WHITE		"\033[47m"
#define BG_DEFAULT		"\033[49m"

// bold colors
#define B_WHITE			"\e[1;37m"
#define B_CYAN			"\e[1;36m"
#define B_PURPLE		"\e[1;35m"
#define B_BLUE			"\e[1;34m"

// italic colors
#define I_WHITE			"\e[3;37m"

// color with effect
#define PURPLE_BLINK	"\033[5;35m"
#define GREEN_BLINK		"\033[5;32m"

// effect
#define NORMAL			"\033[0m"
#define BOLD_ON			"\033[1m"
#define DIM_ON			"\033[2m"
#define UNDERLINED_ON	"\033[4m"
#define BLINK_ON		"\033[5m"
#define REVERSE_ON		"\033[7m"
#define HIDE_ON			"\033[8m"
	
#define BOLD_OFF		"\033[22m"
#define DIM_OFF			"\033[21m"
#define UNDERLINED_OFF	"\033[24m"
#define BLINK_OFF		"\033[25m"
#define REVERSE_OFF		"\033[27m"
#define HIDE_OFF		"\033[28m"
#define ERASE_LINE		"\033[K"

#endif