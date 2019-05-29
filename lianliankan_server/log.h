#pragma once
/*
* Name: QinKuai
* Status: Finished
*/

#ifndef LOG_H
#define LOG_H
#include <iostream>

#define PROG_RELEASE

#ifndef PROG_RELEASE
#define LOG(logMsg) std::cout << "[LOG][" << __DATE__ << " " << __TIME__ << "][" << __FILE__ << "][" << __LINE__ << "]" << logMsg << std::endl;
#else
#define LOG(logMsg) std::cout<< "[LOG]["<<__DATE__<<" "<<__TIME__<<"]["<<logMsg<<"]"<<std::endl;
#endif

#endif // !LOG_H
