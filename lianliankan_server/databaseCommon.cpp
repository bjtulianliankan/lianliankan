/*
* Name: QinKuai
* Status: Finished
*/
#include "databaseCommon.h"

//实现int和string的转换
//方便翻译为sql语句
std::string intToString(const int number) {
	std::stringstream ss;
	ss << number;
	return ss.str();
}