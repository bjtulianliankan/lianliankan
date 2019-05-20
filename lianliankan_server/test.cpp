/*
* Name: QinKuai
* Just for Test
*/

#include <string>
#include <string.h>
#include <iostream>
#include "user.h"
#include "netMsg.h"
#define MAX_LENGTH 4096
using namespace std;

void println(int number) {
	cout << number << endl;
}

void println(string str) {
	cout << str << endl;
}

int main(void) {
	//char* test = new char[100];
	//memset(test, '\0', 100);

	////1
	//int number = 10;
	//memcpy(test, &number, INT_LENGTH);



	char* chars = new char[MAX_LENGTH];
	memset(chars, '\0', MAX_LENGTH);
	NetMsg* msg1 = new NetMsg();
	User* user1 = new User("QinKuai", "123456");
	User* user2 = new User("QINKUAI", "1234567");

	//std::list<User>* users = new std::list<User>();
	//msg1->setUsers(*users);
	msg1->addUserToList(*user1);
	msg1->addUserToList(*user2);

	msg1->setCommand(LOGIN);
	int counter = msg1->serialize(chars);

	NetMsg *msg2 = new NetMsg();
	msg2->deserialize(chars);
	cout << msg2->getCommand() << endl;
	for (std::list<User>::iterator it = msg2->getUsers().begin(); it != msg2->getUsers().end() ; it++) {
		cout << it->getUserName() << endl;
		cout << it->getPassword() << endl;
		println(it->getScore());
		println(it->getCoins());
		println(it->getTimeDelayItemAmount());
		println(it->getTimePauseItemAmount());
	}
	



	delete[] chars;
	delete msg1;
	delete msg2;
	//delete users;
	getchar();
	return 0;
}