/*
* Name: QinKuai
* Status: Finished
*/

#include "userBase.h"

//��̬������ʼ��
UserBase* UserBase::userBaseInstance = nullptr;

UserBase::UserBase() {}

//��֤��ȡ����ʵ��
UserBase* UserBase::getUserBase() {
	if (UserBase::userBaseInstance == nullptr) {
		UserBase::userBaseInstance = new UserBase();
	}
	return UserBase::userBaseInstance;
}

//ͨ���û�������������û�
void UserBase::addUser(std::string username, std::string passwd) {
	User user(username, passwd);
	this->users.push_back(user);
}

//ͨ����������û�
void UserBase::addUser(User& user) {
	this->users.push_back(user);
}

//ͨ���û�����ѯ�û�
User* UserBase::findUser(std::string username) {
	for (std::list<User>::iterator it = this->users.begin(); it != this->users.end(); it++) {
		if (!strcmp(it->getUserName().c_str(), username.c_str())) {
			return &(*it);
		}
	}
	return nullptr;
}

//ͨ���û����������ѯ�û�
User* UserBase::findUser(std::string username, std::string passwd) {
	for (std::list<User>::iterator it = this->users.begin(); it != this->users.end(); it++) {
		if (!strcmp(it->getUserName().c_str(), username.c_str())) {
			if (!strcmp(it->getPassword().c_str(), passwd.c_str())) {
				return &(*it);
			}
		}
	}
	return nullptr;
}

//for test
void UserBase::showAll() {
	for (std::list<User>::iterator it = this->users.begin(); it != this->users.end(); it++) {
		std::cout << (*it).getUserName() << " " << (*it).getPassword() << " " << (*it).getScore() << " " << (*it).getCoins() << " " << (*it).getTimeDelayItemAmount() << " " << it->getClearGameNumber();
	}
}