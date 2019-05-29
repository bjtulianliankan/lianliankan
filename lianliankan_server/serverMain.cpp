/*
* Name: QinKuai
* Status: Unfinished
*/
#include "server.h"
#include "netMsg.h"
#include "userDatabase.h"

using namespace std;

DWORD WINAPI start(LPVOID para) {
	static_cast<Server*>(para)->start();
	return 0L;
}


//���������
int main(void) {
	//�ȼ����û����ݿ��е���Ϣ
	if (!UserDatabase::databaseToUsers()) {
		LOG("�û����ݳ�ʼ��ʧ��")
		return -1;
	}

	Server* server = new Server();

	server->start();

	User* user = new User("QinKuai", "123456");
	user->setCoins(10000);
	user->setScore(1213);
	user->setClearGameNumber(2);
	user->setGameScore(1000, 1);
	user->setGameScore(1100, 2);

	char* chars = new char[BUFFER_SIZE];
	memset(chars, 0, BUFFER_SIZE);

	NetMsg* netMsg = new NetMsg();
	netMsg->addUserToList(*user);

	netMsg->serialize(chars, DEFAULT_MODE);



	NetMsg* newMsg = new NetMsg();

	newMsg->deserialize(chars, DEFAULT_MODE);

	server->userdataUpdate(newMsg);
	
	delete user;
	delete netMsg;
	delete newMsg;
	////�����������û�������߳�
	//HANDLE newThread = CreateThread(NULL, 0, start, server, 0, NULL);
	//CloseHandle(newThread);

	////���̼߳����������Ӧ�Ĳ���
	//string cmd = "";
	//while (true) {
	//	//�ȴ��û�����ָ��
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