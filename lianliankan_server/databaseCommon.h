/*
* Name: QinKuai
* Status: Finished
*/
#pragma once

#ifndef DATABASECOMMON_H
#define DATABASECOMMON_H

#define DB_SUCCESS true;
#define DB_FAIL false;

#include <string>
#include <sstream>

//实现int和string的转换
//方便翻译为sql语句
std::string intToString(const int);

#endif // !DATABASECOMMON_H

