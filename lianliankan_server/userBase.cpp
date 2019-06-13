/*
* Name: QinKuai
* Status: Finished
*/

#include "userBase.h"

//静态变量初始化
UserBase* UserBase::userBaseInstance = nullptr;

UserBase::UserBase() {
	this->users.resize(0);
}

//保证获取到单实例
UserBase* UserBase::getUserBase() {
	if (UserBase::userBaseInstance == nullptr) {
		UserBase::userBaseInstance = new UserBase();
	}
	return UserBase::userBaseInstance;
}

//通过用户名和密码添加用户
void UserBase::addUser(std::string username, std::string passwd) {
	User user(username, passwd);
	this->users.push_back(user);
}

//通过对象添加用户
void UserBase::addUser(User& user) {
	this->users.push_back(user);
}

//通过用户名查询用户
User* UserBase::findUser(std::string username) {
	for (std::list<User>::iterator it = this->users.begin(); it != this->users.end(); it++) {
		if (!strcmp(it->getUserName().c_str(), username.c_str())) {
			return &(*it);
		}
	}
	return nullptr;
}

//通过用户名和密码查询用户
User* UserBase::findUser(std::string username, std::string passwd) {
	for (std::list<User>::iterator it = this->users.begin(); it != this->users.end(); it++) {
		if (!strcmp(it->getUserName().c_str(), username.c_str())) {
			if (!strcmp(it->getPassword().c_str(), passwd.c_str())) {
				return &(*it);
			}
		}
	}
	return nullptr;
}

//通过用户的ID查找到用户
User* UserBase::findUser(int id) {
	if (id <= 0 && id > this->users.size()) {
		return nullptr;
	}
	for (std::list<User>::iterator it = this->users.begin(); it != this->users.end(); it++) {
		if (it->getID() == id) {
			return &(*it);
		}
	}
	return nullptr;
}

//获取当前用户列表的长度
int UserBase::getUsersLength() {
	return this->users.size();
}
