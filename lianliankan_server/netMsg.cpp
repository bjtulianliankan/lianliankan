/*
* Name: QinKuai
* Status: Finished
*/

#include "netMsg.h"

NetMsg::NetMsg() {
	this->list_size = 0;
	this->command = DEFAULT_COMMAND;
}

NetMsg::NetMsg(std::list<User>& users, ushort command) {
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
每个用户的信息不建议超过100B 
command: ushort 2

list_size :ushort 2

list<User> users:{
	User:{
		username_length: ushort 2
		username: char* 
		password_length: ushort 2
		password: char*

		score: int 4
		clearGameNumber: ushort 2
		list<int> gameScore:{
			gameLevelScore: int 4
		}

		coins: int 4
		TDItems: ushort 2
		reconstructItems: ushort 2
	}
}
通过对象调用序列化函数后
将传入的字符串数组直接传入该对象的对应数据
字符串数组的大小不小于CHARS_MAX_LENGTH(1kB)
*/
int NetMsg::serialize(char*& chars, int mode) {
	ushort count = 0;
	//command ushort
	memcpy(chars + count, &(this->command), USHORT_LENGTH);
	count += USHORT_LENGTH;

	//list_size ushort
	memcpy(chars + count, &(this->list_size), USHORT_LENGTH);
	count += USHORT_LENGTH;

	//通过循环实现User列表的序列化
	for (std::list<User>::iterator it = this->users.begin(); it != this->users.end(); it++) {
		//username_length
		ushort usernameLength = (*it).getUserName().length();
		memcpy(chars + count, &usernameLength, USHORT_LENGTH);
		count += USHORT_LENGTH;

		//username
		memcpy(chars + count, (*it).getUserName().c_str(), usernameLength);
		count += usernameLength;

		//只在默认模式下写入用户密码
		if (mode == DEFAULT_MODE) {
			//password_length
			ushort passwdLength = (*it).getPassword().length();
			memcpy(chars + count, &passwdLength, USHORT_LENGTH);
			count += USHORT_LENGTH;

			//password
			memcpy(chars + count, (*it).getPassword().c_str(), passwdLength);
			count += passwdLength;
		}

		//score
		int score = (*it).getScore();
		memcpy(chars + count, &score, INT_LENGTH);
		count += INT_LENGTH;

		//clearGameNumber
		//同时也是代表了积分数组的长度
		ushort clearGameNumber = (*it).getClearGameNumber();
		memcpy(chars + count, &clearGameNumber, USHORT_LENGTH);
		count += USHORT_LENGTH;

		//每一局单独的分数
		int scorePerGame = 0;
		for (int i = 1; i <= clearGameNumber; i++) {
			scorePerGame = (*it).getGameScore(i);
			memcpy(chars + count, &scorePerGame, INT_LENGTH);
			count += INT_LENGTH;
		}

		if (mode != RANKING_MODE) {
			//coins
			int coins = (*it).getCoins();
			memcpy(chars + count, &coins, INT_LENGTH);
			count += INT_LENGTH;

			//TDItems
			ushort timeDelayItems = (*it).getTimeDelayItemAmount();
			memcpy(chars + count, &timeDelayItems, USHORT_LENGTH);
			count += USHORT_LENGTH;

			//RecItems
			ushort recItems = (*it).getReconstructItemAmount();
			memcpy(chars + count, &recItems, USHORT_LENGTH);
			count += USHORT_LENGTH;
		}
	}

	return count;
}

/*
*序列化结构：
command: ushort 2

list_size :ushort 2

list<User> users:{
	User:{
		username_length: ushort 2
		username: char*
		password_length: ushort 2
		password: char*

		score: int 4
		clearGameNumber: ushort 2
		list<int> gameScore:{
			gameLevelScore: int 4
		}

		coins: int 4
		TDItems: ushort 2
		reconstructItems: ushort 2
	}
}
通过对象调用反序列化函数后
该对象直接完成数据传输
传输过来的数据不超过1kB
*/
int NetMsg::deserialize(const char* chars, int mode) {
	//解析command
	int offset = 0;
	memcpy(&this->command, chars + offset, USHORT_LENGTH);
	offset += USHORT_LENGTH;

	//解析list_size
	memcpy(&this->list_size, chars + offset, USHORT_LENGTH);
	offset += USHORT_LENGTH;

	if (this->list_size < 0) {
		return -1;
	}
	//通过获取到的list_size来遍历获取User列表
	for (int i = 0; i < this->list_size; i++) {
		User* newUser = new User();
		//确定字符串数组的长度
		ushort username_length = 0;
		memcpy(&username_length, chars + offset, USHORT_LENGTH);
		offset += USHORT_LENGTH;

		//构建username的字符数组
		//并将解析到的数据直接付给该对象中的User对象
		if (username_length <= 0) {
			return -1;
		}
		char* username_chars = new char[username_length + 1];
		//字符数组初始化
		memset(username_chars, '\0', username_length + 1);
		memcpy(username_chars, chars + offset, username_length);
		std::string username(username_chars);
		newUser->setUserName(username);
		delete[] username_chars;
		offset += username_length;

		//在默认模式下解析用户密码
		if (mode == DEFAULT_MODE) {
			int passwd_length = 0;
			//获取密码的字符串长度
			memcpy(&passwd_length, chars + offset, USHORT_LENGTH);
			offset += USHORT_LENGTH;

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
		}

		//解析score
		int score = 0;
		memcpy(&score, chars + offset, INT_LENGTH);
		if (score < 0) {
			return -1;
		}
		newUser->setScore(score);
		offset += INT_LENGTH;

		//解析clearGameNumber
		ushort level = 0;
		memcpy(&level, chars + offset, USHORT_LENGTH);
		if (level < 0) {
			return -1;
		}
		newUser->setClearGameNumber(level);
		offset += USHORT_LENGTH;

		//解析每一局的局分数
		int scorePerGame = 0;
		for (int i = 1; i <= level; i++) {
			memcpy(&scorePerGame, chars + offset, INT_LENGTH);
			if (scorePerGame < 0) {
				return -1;
			}
			newUser->setGameScore(scorePerGame, i);
			offset += INT_LENGTH;
		}

		if (mode != RANKING_MODE) {
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
			memcpy(&timeDelayItems, chars + offset, USHORT_LENGTH);
			if (timeDelayItems < 0) {
				return -1;
			}
			newUser->setTimeDelayItemAmount(timeDelayItems);
			offset += USHORT_LENGTH;

			//解析RecItems
			int recItems = 0;
			memcpy(&recItems, chars + offset, USHORT_LENGTH);
			if (recItems < 0) {
				return -1;
			}
			newUser->setReconstructItemAmount(recItems);
			offset += USHORT_LENGTH;
		}

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

ushort NetMsg::getCommand() {
	return this->command;
}

void NetMsg::setCommand(ushort command) {
	this->command = command;
}
