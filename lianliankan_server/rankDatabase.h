/*
* Name: QinKuai
* Status: Finished
*/
#pragma once

#ifndef RANKDATABASE_H
#define RANKDATABASE_H

#include "databaseCommon.h"
#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn/exception.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>

#include "rankBase.h"
#include "log.h"

class RankDatabase {
public:
	//将数据库中的数据加载到RankBase中
	static bool databaseToRanking();
	//更新数据库中的排行榜数据
	//int表示对应的gameLevel
	//std::string表示新的排行信息
	//使用用户ID进行排行
	static bool updateToDatabase(int, std::string);

	//关闭数据库连接
	static void close();

private:
	RankDatabase();
	//创建与排行榜数据库的连接
	static bool init();

	//数据库相关配置
	static sql::Driver *driver;
	static sql::Connection *conn;
	static sql::Statement* stmt;
	static sql::ResultSet* res;
};

#endif // !RANKDATABASE_H

