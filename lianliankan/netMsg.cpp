/*
* Name: QinKuai
* Status: Finished
*/

#include "netMsg.h"

NetMsg::NetMsg() {
	this->list_size = 0;
	this->command = DEFAULT_COMMAND;
}

NetMsg::NetMsg(std::list<User>& users, int command) {
	this->users = users;
	this->list_size = users.size();
	this->command = command;
}

NetMsg::~NetMsg() {
	while (this->list_size--) {
		this->users.pop_front();
	}
}

/*
*序列化结构：
command:int
list<User> users:{
	User:{
		username_length:int
		username:char*
		password_length:int
		password:char*
		score:int
		coins:int
		TDItems:int
		TPItems:int
	}
}
通过对象调用序列化函数后
将传入的字符串数组直接传入该对象的对应数据
字符串数组的大小不小于CHARS_MAX_LENGTH(4kB)
*/
int NetMsg::serialize(char*& chars) {
	int count = 0;
	//command
	memcpy(chars + count, &(this->command), INT_LENGTH);
	count += INT_LENGTH;

	//list_size
	memcpy(chars + count, &(this->list_size), INT_LENGTH);
	count += INT_LENGTH;

	//通过循环实现User列表的序列化
	for (std::list<User>::iterator it = this->users.begin(); it != this->users.end(); it++) {
		//username_length
		int usernameLength = (*it).getUserName().length();
		memcpy(chars + count, &usernameLength, INT_LENGTH);
		count += INT_LENGTH;

		//username
		memcpy(chars + count, (*it).getUserName().c_str(), usernameLength);
		count += usernameLength;

		//password_length
		int passwdLength = (*it).getPassword().length();
		memcpy(chars + count, &passwdLength, INT_LENGTH);
		count += INT_LENGTH;

		//password
		memcpy(chars + count, (*it).getPassword().c_str(), passwdLength);
		count += passwdLength;

		//score
		int score = (*it).getScore();
		memcpy(chars + count, &score, INT_LENGTH);
		count += INT_LENGTH;

		//coins
		int coins = (*it).getCoins();
		memcpy(chars + count, &coins, INT_LENGTH);
		count += INT_LENGTH;

		//TDItems
		int timeDelayItems = (*it).getTimeDelayItemAmount();
		memcpy(chars + count, &timeDelayItems, INT_LENGTH);
		count += INT_LENGTH;

		//TPItems
		int timePauseItems = (*it).getTimePauseItemAmount();
		memcpy(chars + count, &timePauseItems, INT_LENGTH);
		count += INT_LENGTH;
	}

	return count;
}

/*
*序列化结构：
command:int
list<User> users:{
	User:{
		username_length:int
		username:char*
		password_length:int
		password:char*
		score:int
		coins:int
		TDItems:int
		TPItems:int
	}
}
通过对象调用反序列化函数后
该对象直接完成数据传输
*/
int NetMsg::deserialize(const char* chars) {
	//解析command
	int offset = 0;
	memcpy(&this->command, chars + offset, INT_LENGTH);
	offset += INT_LENGTH;

	//解析list_size
	memcpy(&this->list_size, chars + offset, INT_LENGTH);
	offset += INT_LENGTH;

	if (this->list_size < 0) {
		return -1;
	}
	//通过获取到的list_size来遍历获取User列表
	for (int i = 0; i < this->list_size; i++) {
		User* newUser = new User();
		int username_length = 0;
		memcpy(&username_length, chars + offset, INT_LENGTH);
		offset += INT_LENGTH;

		//构建username的字符数组
		//并将解析到的数据直接付给该对象中的User对象
		if (username_length <= 0) {
			return -1;
		}
		char* username_chars = new char[username_length + 1];
		memset(username_chars, '\0', username_length + 1);
		memcpy(username_chars, chars + offset, username_length);
		std::string username(username_chars);
		newUser->setUserName(username);
		delete[] username_chars;
		offset += username_length;

		int passwd_length = 0;
		memcpy(&passwd_length, chars + offset, INT_LENGTH);
		offset += INT_LENGTH;

		//构建password的字符串数组
		//并将解析到的数据直接付给该对象中的User对象
		if (passwd_length <= 0) {
			return -1;
		}

		char* passwd_chars = new char[passwd_length + 1];
		memset(passwd_chars, '\0', passwd_length + 1);
		memcpy(passwd_chars, chars + offset, passwd_length);
		std::string passwd(passwd_chars);
		newUser->setPassword(passwd);
		delete[] passwd_chars;
		offset += passwd_length;

		//解析score
		int score = 0;
		memcpy(&score, chars + offset, INT_LENGTH);
		if (score < 0) {
			return -1;
		}
		newUser->setScore(score);
		offset += INT_LENGTH;

		//解析coins
		int coins = 0;
		memcpy(&coins, chars + offset, INT_LENGTH);
		if (coins < 0) {
			return -1;
		}
		newUser->setCoins(coins);
		offset += INT_LENGTH;

		//解析TDItems
		int timeDelayItems = 0;
		memcpy(&timeDelayItems, chars + offset, INT_LENGTH);
		if (timeDelayItems < 0) {
			return -1;
		}
		newUser->setTimeDelayItemAmount(timeDelayItems);
		offset += INT_LENGTH;

		//解析TPItems
		int timePauseItems = 0;
		memcpy(&timePauseItems, chars + offset, INT_LENGTH);
		if (timePauseItems < 0) {
			return -1;
		}
		newUser->setTimePauseItemAmount(timePauseItems);
		offset += INT_LENGTH;

		this->users.push_back(*newUser);
	}

	return 0;
}

void NetMsg::addUserToList(User& user) {
	this->users.push_back(user);
	this->list_size++;
}


std::list<User>& NetMsg::getUsers() {
	return this->users;
}

void NetMsg::setUsers(std::list<User>& users) {
	this->users = users;
}

int NetMsg::getCommand() {
	return this->command;
}

void NetMsg::setCommand(int command) {
	this->command = command;
}
