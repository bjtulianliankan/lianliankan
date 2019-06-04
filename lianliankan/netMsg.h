/*
* Name: QinKuai
* Status: Finished
*/
#pragma once
#ifndef NETMSG_H
#define NETMSG_H

#define DEFAULT_MODE 0
#define SECRET_MODE 1
#define RANKING_MODE 2

#include <list>
#include "user.h"
#include "common.h"

class NetMsg {
public:
	NetMsg();
	NetMsg(std::list<User>&, ushort);
	~NetMsg();
	//序列化
	//将操作指令和用户对象
	//转化为字符串以实现网络传输
	//第二个参数表示序列化的模式
	//DEFAULT_MODE序列化用户所用信息
	//SECRET_MODE序列化用户除密码以外的所有信息
	//RANKING_MODE只序列化用户的比分信息
	int serialize(char*&, int);
	//反序列化
	//将网络传输过来的字符串
	//转化为一个包含操作指令和用户对象的对象
	//第二个参数表示序列化的模式
	//DEFAULT_MODE反序列化用户所用信息
	//SECRET_MODE反序列化用户除密码以外的所有信息
	//RANKING_MODE只反序列化用户的比分信息
	int deserialize(const char*m, int);

	void addUserToList(User&);
	std::list<User>& getUsers();
	void setUsers(std::list<User>&);
	ushort getCommand();
	void setCommand(ushort);
private:
	//内含的某一用户
	std::list<User> users;
	//用户列表中的用户数
	ushort list_size;
	//指定的操作类型
	ushort command;
};

#endif // !NETMSG_H

