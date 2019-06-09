#pragma once
/*
* Name: QinKuai
* Status: Finished
*/

#ifndef LOG_H
#define LOG_H
#include <string>
#include <ctime>
#include <iostream>

#define PROG_RELEASE

const static std::string wdays[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

std::string getTime();

#ifndef PROG_RELEASE
#define LOG(logMsg) std::cout << "[LOG][" <<getTime()<< "][" << __FILE__ << "][" << __LINE__ << "]" << logMsg << std::endl;
#else
#define LOG(logMsg) std::cout<< "[LOG]["<<getTime()<<"]["<<logMsg<<"]"<<std::endl;
#endif

#endif // !LOG_H
