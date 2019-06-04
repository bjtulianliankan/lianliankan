/*
* Name: QinKuai
* Status: Finised
*/
#include "tool.h"

int numberLength(int number) {
	if (number == 0) {
		return 1;
	}
	int digit = 0;
	while (number != 0) {
		number /= 10;
		digit++;
	}
	return digit;
}

int charNumberInChars(const char* chars, char c) {
	int number = 0;
	int i = 0;
	while (chars[i]) {
		if (chars[i] == c) {
			number++;
		}
	}
	return number;
}

