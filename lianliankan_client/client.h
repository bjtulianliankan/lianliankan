/*
* Name: QinKuai
* Status: Unfinised
*/
#pragma once
#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <string.h>
#include "common.h"
#include "netMsg.h"

//for test
#include <iostream>

class Client {
public:
	//只能通过这个方法获取这个类的实例
	static Client* getClientInstance();
	//开启网络
	bool start();
	//关闭网络
	bool close();
	//通过指定string来实现同一方法不同实现
	NetMsg* request(NetMsg*&, int, int);
private:
	//维护一个这个类的静态实例
	static Client* client;
	//隐藏的构造器
	Client();
	//初始化网络相关参数
	bool init();

	//用户注册
	NetMsg* send_register(NetMsg*&);
	//用户登录
	NetMsg* send_login(NetMsg*&);
	//用户登出
	NetMsg* send_logout(NetMsg*&);
	//用户更新数据
	NetMsg* send_update(NetMsg*&);
	//获取排行榜
	NetMsg* send_ranking(NetMsg*&, int);

	//向后端发送NetMsg
	bool sendToServer(NetMsg*&);
	//接收后端的NetMsg
	bool recvFromServer(NetMsg*&);

	//TCP socket的地址结构
	SOCKADDR_IN addr;
	//socket的值
	SOCKET socket1;
};

#endif // !CLIENT_H

