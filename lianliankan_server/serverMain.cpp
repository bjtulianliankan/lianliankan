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
	//_CrtDumpMemoryLeaks();
	return 0L;
}

//主函数入口
int main(void) {
	//先加载用户数据库中的信息
	if (!UserDatabase::databaseToUsers()) {
		LOG("用户数据初始化失败")
		system("pause");
		return -1;
	}
	LOG("用户数据库初始化成功")

	//先加载排行榜数据库中的信息
	if (!RankDatabase::databaseToRanking()) {
		LOG("排行榜数据库初始化失败")
		system("pause");
		return -1;
	}
	LOG("排行榜数据库初始化成功")

	Server* server = new Server();

	//服务器监听用户请求的线程
	HANDLE newThread = CreateThread(NULL, 0, start, server, 0, NULL);
	CloseHandle(newThread);

	//主线程监听服务端相应的操作
	string cmd = "";
	while (true) {
		//等待用户输入指令
		cin >> cmd;
		if (!strcmp(cmd.c_str(), HELP)) {
			HELPMSG
		} else if (!strcmp(cmd.c_str(), EXIT)) {
			server->close();
			delete server;
			//_CrtDumpMemoryLeaks();
			system("pause");
			return 0;
		} else {
			CMDERROR
		}
	}
}