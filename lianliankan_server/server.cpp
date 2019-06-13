/*
* Name: QinKuai
* Status: Finished
*/
#include "server.h"

Server::Server() {}

Server::~Server() {}

/*
初始化服务器基础配置
*/
bool Server::init() {
	//开启Winsock
	//填写WSA信息
	WSADATA wsadata;
	int error = WSAStartup(0x0202, &wsadata);

	//查看是否有异常发生
	if (error) {
		LOG("Winsock连接异常")
		return false;
	}
	if (wsadata.wVersion != 0x0202) {
		//清除Winsock
		WSACleanup();
		LOG("Winsock版本异常")
		return false;
	}

	//初始化TCP socket的结构
	//PF_INET == AF_INET
	this->addr.sin_family = PF_INET;
	this->addr.sin_port = htons(SERVER_PORT);
	inet_pton(PF_INET, SERVER_IP, &this->addr.sin_addr.s_addr);

	//创建socket
	this->socket1 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//创建socket失败
	if (socket1 == INVALID_SOCKET) {
		LOG("Socket创建失败")
		return false;
	}

	//绑定Socket
	if (bind(this->socket1, (LPSOCKADDR)&this->addr, sizeof(this->addr)) == SOCKET_ERROR) {
		LOG("Socket无法绑定")
		return false;
	}	

	int ret = listen(this->socket1, 1);

	if (ret == SOCKET_ERROR) {
		LOG("Socket监听失败")
		return false;
	}

	return true;
}

/*
创建服务器对象之后
调用该方法
初始化服务器相关信息
并开始监听客户端请求
*/
void Server::start() {
	if (!this->init()){
		LOG("服务器初始化失败")
		return;
	}

	LOG("服务器初始化完成")

	//正式开始监听客户端信息
	//缓存区
	char* recv_buf = new char[BUFFER_SIZE];
	//返还客户端缓存区
	char* send_buf = new char[BUFFER_SIZE];
	//若请求排行榜
	//则存入关卡序号信息
	int level = 0;

	while (true) {
		//socekt断开直接退出服务器运行
		if (!this->socket1) {
			delete[] recv_buf;
			break;
		}
		//缓存区初始化或是缓存区清零
		memset(recv_buf, 0, BUFFER_SIZE);
		memset(send_buf, 0, BUFFER_SIZE);

		SOCKADDR_IN client_addr;
		socklen_t client_length = sizeof(client_addr);
		LOG("服务器待机中")
		SOCKET client = accept(this->socket1, (struct sockaddr*)&client_addr, &client_length);
		if (client == SOCKET_ERROR) {
			LOG("服务器网络断开")
			break;
		}
		char log[INET_ADDRSTRLEN];
		std::string str = inet_ntop(PF_INET, &client_addr.sin_addr, log, sizeof(log));
		str += "连入服务器";
		LOG(str)
		int len = recv(client, recv_buf, BUFFER_SIZE, 0);
		
		//创建反序列化对象
		//确定客户端请求类型
		NetMsg *client_request = new NetMsg();
		//将客户端请求信息
		//首先进行command的解析
		//初步确定请求类型
		int command = client_request->command_deserialize(recv_buf);
		if (command !=RANKING ) {
			//反序列化加载到对象中
			client_request->deserialize(recv_buf, DEFAULT_MODE);
		} else {
			//解析用户发来的排行榜请求
			client_request->server_ranking_deserialize(recv_buf, level);
		}

		//返还客户端时的序列化对象指针
		NetMsg *send_back = nullptr;

		//此时的client_request已经时解析好的NetMsg对象了
		switch (command) {
		case REGISTER:
			//服务器端实现用户注册
			 send_back = userRegister(client_request);

			send_back->serialize(send_buf, SECRET_MODE);
			//向客户端返还注册情况
			send(client, send_buf, BUFFER_SIZE, 0);
				
			delete client_request;
			delete send_back;
			continue;
		case LOGIN:
			//服务器端实现用户登陆
			 send_back = userLogin(client_request);

			 send_back->serialize(send_buf, SECRET_MODE);
			 //向客户端返还登陆情况
			 send(client, send_buf, BUFFER_SIZE, 0);
			 
			 delete client_request;
			 delete send_back;
			continue;
		case LOGOUT:
			//服务器端实现用户登出
			 send_back = userLogout(client_request);

			 send_back->serialize(send_buf, SECRET_MODE);
			 //向客户端返还登出情况
			 send(client, send_buf, BUFFER_SIZE, 0);

			 delete client_request;
			 delete send_back;
			continue;
		case USERDATA_UPDATE:
			//服务器端实现用户数据更新
			 send_back = userdataUpdate(client_request);

			 send_back->serialize(send_buf, SECRET_MODE);
			 //向客户端返还用户数据更新情况
			 send(client, send_buf, BUFFER_SIZE, 0);
			 
			 delete client_request;
			 delete send_back;
			continue;
		case RANKING:
			//服务器端实现排名返还
			send_back = userRanking(client_request, level);

			send_back->server_ranking_serialize(send_buf, level);
			//向客户端返还排名情况
			send(client, send_buf, BUFFER_SIZE, 0);

			delete client_request;
			delete send_back;
			continue;
		}
	}

	delete[] recv_buf;
	delete[] send_buf;
}

/*
服务器停用方法
*/
void Server::close() {
	if (this->socket1) {
		closesocket(this->socket1);
	}
	WSACleanup();
	Sleep(100);
	//关闭数据库连接
	UserDatabase::close();
	RankDatabase::close();
	LOG("数据库连接正常关闭")
	//清除在线存储的信息
	delete UserBase::getUserBase();
	delete RankBase::getRankBase();
	LOG("服务器正常退出")
}

/*
服务端注册方法
*/
NetMsg* Server::userRegister(NetMsg* msg) {
	User *user = &msg->getUsers().front();
	//服务器端的本地LOG消息
	std::string log = user->getUserName();
	LOG(log + " 请求注册")

	NetMsg* send_back = new NetMsg();

	//查询UserBase中是否含有相同用户名的用户
	if (UserBase::getUserBase()->findUser(user->getUserName()) != nullptr) {
		send_back->setCommand(REGISTER_FAIL);
		log.append(" 用户名已存在，注册失败");
		LOG(log)
		return send_back;
	}

	//确定新注册用的ID
	user->setID(UserBase::getUserBase()->getUsersLength() + 1);

	//写入数据库中
	bool result = UserDatabase::addToDatabase(*user);
	if (!result) {
		send_back->setCommand(REGISTER_FAIL);
		log.append(" 写入用户数据库失败，注册失败");
		LOG(log)
		return send_back;
	}

	//添加到UserBase上
	UserBase::getUserBase()->addUser(*user);

	//添加到RankBase上
	RankBase::getRankBase()->insertToRankBase(user->getID());

	//写入排行榜数据库
	std::string str("");
	//i表示gameLevel
	for (int i = 0; i <= GAMEAMOUNT; i++) {
		str = RankBase::getRankBase()->scoreRankToString(i);
		result = RankDatabase::updateToDatabase(i, str);
		if (!result) {
			send_back->setCommand(REGISTER_FAIL);
			log.append(" 写入排行榜数据库失败，注册失败");
			LOG(log)
			return send_back;
		}
	}
	
	send_back->setCommand(REGISTER_SUCCESS);
	log.append(" 注册成功");
	LOG(log)
	return send_back;
}

/*
服务端登陆方法
*/
NetMsg* Server::userLogin(NetMsg* msg) {
	User* user = &msg->getUsers().front();
	//服务器端的本地LOG消息
	std::string log = user->getUserName();
	LOG(log + " 请求登陆")

	NetMsg* send_back = new NetMsg();

	//从UserBase中维护的list中查询用户是否存在
	User* user_send_back = UserBase::getUserBase()->findUser(user->getUserName(), user->getPassword());
	if (user_send_back == nullptr) {
		send_back->setCommand(LOGIN_FAIL);
		log.append(" 用户名错误或密码错误，登陆失败");
		LOG(log)
		return send_back;
	}

	//查找成功后有这个账号之后确认用户登陆
	user_send_back->setLoginStatus(true);

	//并将查找到的用户类返还客户端
	send_back->setCommand(LOGIN_SUCCESS);
	send_back->addUserToList(*user_send_back);

	log.append(" 登陆成功");
	LOG(log)

	return send_back;
}

/*
服务端登出方法
*/
NetMsg* Server::userLogout(NetMsg* msg) {
	User* user = &msg->getUsers().front();
	//服务器端的本地LOG消息
	std::string log = user->getUserName();
	LOG(log + " 请求登出")

	NetMsg* send_back = new NetMsg();

	//该用户的登陆状态设置为登出
	User* user_in_server = UserBase::getUserBase()->findUser(user->getUserName(), user->getPassword());
	if (user_in_server == nullptr) {
		send_back->setCommand(LOGOUT_FAIL);
		log.append("用户名或密码错误，登出失败");
		LOG(log)
		return send_back;
	}
	user_in_server->setLoginStatus(false);

	//设置登出成功
	send_back->setCommand(LOGOUT_SUCCESS);

	log.append(" 登出成功");
	LOG(log)
	return send_back;
}


/*
服务端数据更新方法
*/
NetMsg* Server::userdataUpdate(NetMsg* msg) {
	User* user = &msg->getUsers().front();
	//服务器端的本地LOG消息
	std::string log = user->getUserName();
	LOG(log + " 请求数据更新")

	NetMsg* send_back = new NetMsg();

	//建立指向该用户的指针
	//同时用以check是否有该用户
	User* user_in_server = UserBase::getUserBase()->findUser(user->getUserName(), user->getPassword());

	if (user_in_server == nullptr) {
		send_back->setCommand(USERDATA_UPDATE_FAIL);
		log.append(" 用户名或密码错误，数据更新失败");
		LOG(log)
		return send_back;
	}
	
	//更新数据库中的用户数据
	bool result = UserDatabase::updateToDatabase(*user);

	if (!result) {
		log.append(" 用户数据库异常，数据更新失败");
		LOG(log)
		send_back->setCommand(USERDATA_UPDATE_FAIL);
		return send_back;
	}

	//更新用户信息
	//道具相关信息
	user_in_server->setCoins(user->getCoins());
	user_in_server->setReconstructItemAmount(user->getReconstructItemAmount());
	user_in_server->setTimeDelayItemAmount(user->getTimeDelayItemAmount());
	//更新分数相关信息
	user_in_server->setScore(user->getScore());
	user_in_server->setClearGameNumber(user->getClearGameNumber());
	for (int i = 1; i <= GAMEAMOUNT; i++) {
		user_in_server->setGameScore(user->getGameScore(i), i);
	}

	//添加到RankBase上
	RankBase::getRankBase()->updateToRankBase(user_in_server->getID());

	//写入RankDatabase中
	//写入排行榜数据库
	std::string str("");
	//i表示gameLevel
	for (int i = 0; i <= GAMEAMOUNT; i++) {
		str = RankBase::getRankBase()->scoreRankToString(i);
		result = RankDatabase::updateToDatabase(i, str);
		if (!result) {
			send_back->setCommand(USERDATA_UPDATE_FAIL);
			log.append(" 写入排行榜数据库失败，数据更新失败");
			LOG(log)
			return send_back;
		}
	}

	//设置数据更新成功
	send_back->setCommand(USEREDATA_UPDATE_SUCCESS);

	log.append(" 数据更新成功");
	LOG(log)
	return send_back;
}


/*
服务器排行榜请求
*/
NetMsg* Server::userRanking(NetMsg* msg, int level) {
	User* user = &msg->getUsers().front();
	NetMsg *send_back = new NetMsg();
	
	std::string log = user->getUserName();
	LOG(log + " 请求用户排行榜")

	//建立指向该用户的指针
	//同时用以check是否有该用户
	User* user_in_server = UserBase::getUserBase()->findUser(user->getUserName());

	if (user_in_server == nullptr) {
		send_back->setCommand(RANKING_FAIL);
		log.append(" 用户名或密码错误，获取排行榜失败");
		LOG(log)
		return send_back;
	}

	std::vector<int> ranking = RankBase::getRankBase()->getRankByLevel(level);
	for (std::vector<int>::iterator it =ranking.begin(); it < ranking.end(); it++) {
		User* newUser = new User("", "");
		int id = *it;
		//排行榜在此处没有对应用户
		//返回空字符串和0
		if (id == 0) {
			newUser->setUserName("NULL");
			newUser->setRankScore(0, level);
		} else {
			User* user_in_server = UserBase::getUserBase()->findUser(id);
			newUser->setID(id);
			newUser->setRankScore(user_in_server->getRankScore(level), level);
			newUser->setUserName(user_in_server->getUserName());
		}

		send_back->addUserToList(*newUser);
	}

	
	send_back->setCommand(RANKING_SUCCESS);
	log.append(" 请求排行榜数据成功");
	LOG(log)
	return send_back;
}