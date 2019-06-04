/*
* Name: QinKuai
* Status: Finished
*/

#pragma once
#ifndef USERDATABASE_H
#define USERDATABASE_H

#include "databaseCommon.h"
//#include <jdbc/mysql_connection.h>
#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn/exception.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>

#include "user.h"
#include "userBase.h"
#include "log.h"

class UserDatabase {
public:
	//将数据库中的数据拉到UserBase中
	//实现用户数据管理于list中
	static bool databaseToUsers();
	//添加用户信息
	static bool addToDatabase(User &user);
	//删去用户信息
	static bool deleteToDatabase(User &user);
	//更新用户信息
	static bool updateToDatabase(User &user);
	//关闭数据库连接
	static void close();
private:
	UserDatabase();
	//创建用户数据库对象时初始化与数据库的连接
	static bool init();

	//数据库相关配置
	static sql::Driver *driver;
	static sql::Connection *conn;
	static sql::Statement *stmt;
	static sql::ResultSet *res;
};

#endif // !USERDATABASE_H
