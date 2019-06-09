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
	User* user = new User("ClientTest", "123456");
	user->setClearGameNumber(3);
	user->setCoins(1400);
	user->setReconstructItemAmount(2);
	user->setTimeDelayItemAmount(4);
	user->setGameScore(2400, 1);
	user->setGameScore(5020, 2);
	user->setGameScore(2500, 3);
	user->updateScore();
	send->addUserToList(*user);
	int level = 2;

	Client *client = Client::getClientInstance();
	client->start();	
	NetMsg* newMsg = client->request(send, RANKING, level);
	
	cout << newMsg->getCommand() << endl;
	if (newMsg == nullptr || newMsg->getCommand() == RANKING_FAIL) {
		cout << "ÍøÂçÁ´½ÓÊ§°Ü" << endl;
		delete send;
		Client::getClientInstance()->close();
		getchar();
		return 0;
	}
	
	
	Client::getClientInstance()->close();
	cout << "ÍøÂçÁ´½Ó³É¹¦" << endl;

	for (int i = 0; i < 10; i++) {
		User* user = &newMsg->getUsers()[i];
		cout << "username:" << user->getUserName() << " score:" << user->getRankScore(level) << endl;
	}

	delete send;
	delete newMsg;
	getchar();
	return 0;
}