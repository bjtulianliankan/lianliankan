/*
* Name: QinKuai
* Status: Unfinised
*/
#include "tool.h"

int numberLength(int number) {
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

