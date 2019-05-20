/*
* Name: QinKuai
* Status: Finished
*/

#ifndef NETMSG_H
#define NETMSG_H
#include <list>
#include "stdafx.h"
#include "user.h"
#include "common.h"

class NetMsg {
public:
	NetMsg();
	NetMsg(std::list<User>&, int);
	~NetMsg();
	//序列化
	//将操作指令和用户对象
	//转化为字符串以实现网络传输
	int serialize(char*&);
	//反序列化
	//将网络传输过来的字符串
	//转化为一个包含操作指令和用户对象的对象
	int deserialize(const char*);

	void addUserToList(User&);
	std::list<User>& getUsers();
	void setUsers(std::list<User>&);
	int getCommand();
	void setCommand(int);
private:
	//内含的某一用户
	std::list<User> users;
	//用户列表中的用户数
	int list_size;
	//指定的操作类型
	int command;
};

#endif // !NETMSG_H

