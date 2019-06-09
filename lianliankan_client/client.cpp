/*
* Name: QinKuai
* Status: Unfinised
*/
#include "client.h"

//静态变量初始化
Client* Client::client = nullptr;

//隐藏的默认构造器
Client::Client() {}

//获取这个类的实例的唯一方法
Client* Client::getClientInstance() {
	if (Client::client == nullptr) {
		Client::client = new Client();
	}
	return Client::client;
}

//初始化网络服务
//内部方法
bool Client::init() {
	//开启Winsock
	//填写WSA信息
	WSADATA wsadata;
	int error = WSAStartup(0x0202, &wsadata);

	//查看是否有异常发生
	if (error) {
		return false;
	}

	//检查是否获得正确的WinSock版本
	if (wsadata.wVersion != 0x0202) {
		WSACleanup();
		return false;
	}

	//初始化Socket必须的信息
	//PF_INET == AF_INET
	this->addr.sin_family = PF_INET;
	this->addr.sin_port = htons(SERVER_PORT);
	inet_pton(PF_INET, SERVER_IP, &this->addr.sin_addr.s_addr);

	return true;
}

//开启网络服务
bool Client::start() {
	//网络参数初始化
	if (!this->init()) {
		return false;
	}

	this->socket1 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (this->socket1 == INVALID_SOCKET) {
		return false;
	}

	int result = connect(this->socket1, (SOCKADDR*)&this->addr, sizeof(this->addr));
	if (result == SOCKET_ERROR) {
		return false;
	}

	return true;
}

//关闭网络服务
bool Client::close() {
	if (this->socket1) {
		closesocket(this->socket1);
	}
	WSACleanup();
	return true;
}

//通过指定command来实现同一方法不同实现
NetMsg* Client::request(NetMsg*& msg, int target, int level) {
	//设置向服务器请求的信息
	msg->setCommand(target);

	switch (target) {
	case REGISTER:
		return this->send_register(msg);
	case LOGIN:
		return this->send_login(msg);
	case LOGOUT:
		return this->send_logout(msg);
	case USERDATA_UPDATE:
		return this->send_update(msg);
	case RANKING:
		return this->send_ranking(msg, level);
	default:
		return nullptr;
	}
}

//用户注册
NetMsg* Client::send_register(NetMsg*& msg) {
	NetMsg* client_recv = new NetMsg();
	
	if (!this->sendToServer(msg)) {
		return nullptr;
	}
	
	Sleep(100);

	if (!this->recvFromServer(client_recv)) {
		return nullptr;
	}
	return client_recv;
}

//用户登陆
NetMsg* Client::send_login(NetMsg*& msg) {
	NetMsg* client_recv = new NetMsg();

	if (!this->sendToServer(msg)) {
		return nullptr;
	}

	Sleep(100);

	if (!this->recvFromServer(client_recv)) {
		return nullptr;
	}
	return client_recv;
}

//用户登出
NetMsg* Client::send_logout(NetMsg*& msg) {
	NetMsg* client_recv = new NetMsg();

	if (!this->sendToServer(msg)) {
		return nullptr;
	}

	Sleep(100);

	if (!this->recvFromServer(client_recv)) {
		return nullptr;
	}
	return client_recv;
}

//用户更新数据
NetMsg* Client::send_update(NetMsg*& msg) {
	NetMsg* client_recv = new NetMsg();

	if (!this->sendToServer(msg)) {
		return nullptr;
	}

	Sleep(100);

	if (!this->recvFromServer(client_recv)) {
		return nullptr;
	}
	return client_recv;
}

//获取排行榜
NetMsg* Client::send_ranking(NetMsg*& msg, int level) {
	NetMsg* client_recv = new NetMsg();

	char* send_server = new char[BUFFER_SIZE];
	memset(send_server, 0, BUFFER_SIZE);

	int counter = 0;
	//序列化信息
	counter = msg->client_ranking_serialize(send_server, level);
	if (counter <= 0) {
		return nullptr;
	}

	//发送信息
	int result = send(this->socket1, send_server, BUFFER_SIZE, 0);
	if (result < 0) {
		return nullptr;
	}

	Sleep(100);

	//接收服务器端的回信
	char* recv_chars = new char[BUFFER_SIZE];
	memset(recv_chars, 0, BUFFER_SIZE);
	//接收信息
	result = recv(this->socket1, recv_chars, BUFFER_SIZE, 0);

	if (result < 0) {
		return nullptr;
	}
	//反序列化信息
	int recvLevel = 0;
	counter = client_recv->client_ranking_deserialize(recv_chars, recvLevel);
	if (counter <= 0 || recvLevel != level) {
		return nullptr;
	}
	return client_recv;
}

bool Client::sendToServer(NetMsg*& msg) {
	char* send_server = new char[BUFFER_SIZE];
	memset(send_server, 0, BUFFER_SIZE);

	int counter = 0;
	//序列化信息
	counter = msg->serialize(send_server, DEFAULT_MODE);
	if (counter <= 0) {
		return false;
	}

	//发送信息
	int result = send(this->socket1, send_server, BUFFER_SIZE, 0);
	if (result < 0) {
		return false;
	}
	delete[] send_server;
	return true;
}

bool Client::recvFromServer(NetMsg*& msg) {
	int result = 0;
	int counter = 0;
	char* recv_chars = new char[BUFFER_SIZE];
	memset(recv_chars, 0, BUFFER_SIZE);
	//接收信息
	result = recv(this->socket1, recv_chars, BUFFER_SIZE, 0);

	if (result < 0) {
		return false;
	}
	//反序列化信息
	counter = msg->deserialize(recv_chars, SECRET_MODE);
	if (counter <= 0) {
		return false;
	}
	delete[] recv_chars;
	return true;
}
