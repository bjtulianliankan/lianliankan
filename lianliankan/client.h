/*
* Name: QinKuai
* Status: Unfinished
*/

#ifndef CLIENT_H
#define CLIENT_H
#include "common.h"

class Client {
public:
	Client();
	~Client();
	
	//面向Server的操作
	bool userRegister();
	bool registerBack();
	bool userLogin();
	bool loginBack();
	bool userLogout();
	bool logoutBack();
	bool userUpdate();
	bool updateBack();
	bool ranking();
	bool rankingBack();
private:
	//TCP socket的地址结构
	SOCKADDR_IN addr;
	//socket的值
	SOCKET client_socket;
};

#endif // !CLIENT_H

