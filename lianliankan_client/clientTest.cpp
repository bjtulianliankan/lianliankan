/*
JUST FOR TEST
*/

#include "client.h"
#include "user.h"
#include <iostream>
#include <string>

using namespace std;

int main(void) {
	NetMsg* send = new NetMsg();
	User* user = new User("ClientTest3", "123456");
	user->setClearGameNumber(2);
	user->setCoins(1000);
	user->setReconstructItemAmount(1);
	user->setTimeDelayItemAmount(2);
	user->setGameScore(1000, 1);
	user->setGameScore(1020, 2);
	user->updateScore();
	send->addUserToList(*user);
	int level = 0;

	Client *client = Client::getClientInstance();
	client->start();	
	NetMsg* newMsg = client->request(send, RANKING, level);
	
	if (newMsg == nullptr) {
		cout << "ÍøÂçÁ´½ÓÊ§°Ü" << endl;
		delete send;
		Client::getClientInstance()->close();
		getchar();
		return 0;
	}

	Client::getClientInstance()->close();
	cout << newMsg->getCommand() << endl;

	for (int i = 0; i < 10; i++) {
		User* user = &newMsg->getUsers()[i];
		cout << "username:" << user->getUserName() << " score:" << user->getRankScore(level) << endl;
	}

	delete send;
	delete newMsg;
	getchar();
	return 0;
}