/*
* Name: QinKuai
* Status: Unfinished
*/
#include "server.h"
#include "netMsg.h"
#include "userDatabase.h"
#include "rankDatabase.h"

using namespace std;

DWORD WINAPI start(LPVOID para) {
	static_cast<Server*>(para)->start();
	return 0L;
}


void test() {
	User* user1 = new User("QinKuai", "123456");
	user1->setClearGameNumber(3);
	user1->setGameScore(1200, 1);
	user1->setGameScore(1000, 2);
	user1->setGameScore(1100, 3);

	user1->updateScore();
	UserDatabase::updateToDatabase(*user1);
	delete user1;
}

//主函数入口
int main(void) {
	//先加载用户数据库中的信息
	if (!UserDatabase::databaseToUsers()) {
		LOG("用户数据初始化失败")
		return -1;
	}
	LOG("用户数据库初始化成功")

	//先加载排行榜数据库中的信息
	if (!RankDatabase::databaseToRanking()) {
		LOG("排行榜数据库初始化失败")
		return -1;
	}
	LOG("排行榜数据库初始化成功")

	RankBase::getRankBase()->showAll();

	char* chars = new char[BUFFER_SIZE];
	memset(chars, 0, BUFFER_SIZE);
	//test register
	//test();

	User* user2 = new User("Test1", "123456");
	/*user2->setClearGameNumber(3);
	user2->setGameScore(4000, 1);
	user2->setGameScore(10000, 2);
	user2->setGameScore(5000, 3);

	user2->updateScore();*/

	NetMsg *netmsg1 = new NetMsg();
	//netmsg1->addUserToList(*user1);
	netmsg1->addUserToList(*user2);
	netmsg1->setCommand(RANKING);
	netmsg1->client_ranking_serialize(chars, 1);

	int level = 0;
	NetMsg *recv = new NetMsg();
	recv->server_ranking_deserialize(chars, level);

	Server* server = new Server();

	//server->userRegister(recv);
	//server->userdataUpdate(recv);
	char* send_buf = new char[BUFFER_SIZE];
	memset(chars, 0, BUFFER_SIZE);


	NetMsg* send_back = server->userRanking(recv, level);
	send_back->server_ranking_serialize(send_buf, level);

	NetMsg* client_recv = new NetMsg();
	int recv_level = 0;
	client_recv->client_ranking_deserialize(send_buf, recv_level);

	cout << recv_level << endl;
	for (int i = 0; i < client_recv->getUsers().size(); i++) {
		cout << client_recv->getUsers()[i].getUserName() << ": " << client_recv->getUsers()[i].getScore() << endl;
	}

	//RankBase::getRankBase()->showAll();

	server->close();
	delete[] chars;
	delete[] send_buf;
	delete server;
	delete recv;
	delete netmsg1;
	delete send_back;
	delete client_recv;


	//Server* server = new Server();

	//server->start();

	////服务器监听用户请求的线程
	//HANDLE newThread = CreateThread(NULL, 0, start, server, 0, NULL);
	//CloseHandle(newThread);

	////主线程监听服务端相应的操作
	//string cmd = "";
	//while (true) {
	//	//等待用户输入指令
	//	cin >> cmd;
	//	if (!strcmp(cmd.c_str(), HELP)) {
	//		HELPMSG
	//	} else if (!strcmp(cmd.c_str(), EXIT)) {
	//		server->close();
	//		delete server;
	//		return 0;
	//	} else {
	//		CMDERROR
	//	}
	//}

	getchar();
	return 0;
}