/*
* Name: QinKuai
* Status: Finished
*/
#pragma once
#ifndef NETMSG_H
#define NETMSG_H

#define DEFAULT_MODE 0
#define SECRET_MODE 1

#include <vector>
#include "user.h"
#include "common.h"

class NetMsg {
public:
	NetMsg();
	NetMsg(std::vector<User>&, ushort);
	~NetMsg();
	//序列化
	//将操作指令和用户对象
	//转化为字符串以实现网络传输
	//第二个参数表示序列化的模式
	//DEFAULT_MODE序列化用户所用信息
	//SECRET_MODE序列化用户除密码以外的所有信息
	//RANKING_MODE只序列化用户的比分信息
	int serialize(char*&, int);

	//后台用以序列化排行榜
	//产生排行榜的string-int对应串
	//而int表示指定产生的排行榜的局数
	//0表示总分数
	//之后的整数表示对应局数
	int server_ranking_serialize(char*&, int);

	//客户端用以序列化排行榜请求信息
	//int 表示gameLevel
	int client_ranking_serialize(char*&, int);

	//反序列化
	//将网络传输过来的字符串
	//转化为一个包含操作指令和用户对象的对象
	//第二个参数表示序列化的模式
	//DEFAULT_MODE反序列化用户所用信息
	//SECRET_MODE反序列化用户除密码以外的所有信息
	//RANKING_MODE只反序列化用户的比分信息
	int deserialize(const char*m, int);

	//后台用以解析前台的排行榜请求
	int server_ranking_deserialize(char*&, int&);

	//前台用以解析后台发送的排行榜消息
	//反序列化string-int串
	//获取到排行榜的用户列表
	//将获取到的分数直接加载到score属性中
	//int&获取请求的关卡序号
	//客户端传入int可以检验服务器查找的gameLevel是否对上
	int client_ranking_deserialize(char*&, int&);

	//解析出字符串中的command类型
	//返回command
	ushort command_deserialize(char*&);

	void addUserToList(User&);
	std::vector<User>& getUsers();
	void setUsers(std::vector<User>&);
	ushort getCommand();
	void setCommand(ushort);
private:
	//内含的某一用户
	std::vector<User> users;
	//用户列表中的用户数
	ushort list_size;
	//指定的操作类型
	ushort command;
};

#endif // !NETMSG_H

