/*
* Name: QinKuai
* Status: Finished
*/

#pragma once
#ifndef USERBASE_H
#define USERBASE_H
#include <string>
#include <list>
#include "user.h"

//for test
#include <iostream>

//该类用以在服务器运行时直接将数据库中的用户数据
//拉到线上直接可以查找
class UserBase {
public:
	//保证实例只能被创建一次
	static UserBase* getUserBase();
	//通过用户名和密码添加用户
	void addUser(std::string, std::string);
	//直接添加用户对象
	void addUser(User& user);

	//查询用户
	User* findUser(std::string);
	User* findUser(std::string, std::string);
	User* findUser(int id);

	//获取当前用户列表的长度
	//并起到记录当前最大用户ID
	//用于新用户注册时确定ID
	int getUsersLength();

private:
	UserBase();
	//实现单实例
	static UserBase* userBaseInstance;
	//该实例中维护一个用户列表
	std::list<User> users;
};

#endif // !USERBASE_H
