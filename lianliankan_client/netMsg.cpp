/*
* Name: QinKuai
* Status: Finished
*/

#include "netMsg.h"

NetMsg::NetMsg() {
	this->list_size = 0;
	this->command = DEFAULT_COMMAND;
}

NetMsg::NetMsg(std::vector<User>& users, ushort command) {
	this->users = users;
	this->list_size = users.size();
	this->command = command;
}

NetMsg::~NetMsg() {
	while (this->list_size--) {
		this->users.pop_back();
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
		ptItems: ushort 2
	}
}
通过对象调用序列化函数后
将传入的字符串数组直接传入该对象的对应数据
字符串数组的大小不小于CHARS_MAX_LENGTH(1kB)
*/
int NetMsg::serialize(char*& chars, int mode) {
	int count = 0;
	//command ushort
	memcpy(chars + count, &(this->command), USHORT_LENGTH);
	count += USHORT_LENGTH;

	//list_size ushort
	memcpy(chars + count, &(this->list_size), USHORT_LENGTH);
	count += USHORT_LENGTH;

	//通过循环实现User列表的序列化
	for (std::vector<User>::iterator it = this->users.begin(); it != this->users.end(); it++) {
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
		ushort clearGameNumber = (*it).getClearGameNumber();
		memcpy(chars + count, &clearGameNumber, USHORT_LENGTH);
		count += USHORT_LENGTH;

		//每一局单独的分数
		//积分数组的长度定在了GAMEAMOUNT
		int scorePerGame = 0;
		for (int i = 1; i <= GAMEAMOUNT; i++) {
			scorePerGame = (*it).getGameScore(i);
			memcpy(chars + count, &scorePerGame, INT_LENGTH);
			count += INT_LENGTH;
		}

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

		//PtItems
		ushort ptItems = (*it).getPropmtItemAmount();
		memcpy(chars + count, &ptItems, USHORT_LENGTH);
		count += USHORT_LENGTH;
	}

	return count;
}
/*
排行榜的结构：
command: ushort
level: int
list_size: ushort
list<User>:{
	User:{
		username_length: ushort
		username: char*
		score: int;
	}
}
*/
int NetMsg::server_ranking_serialize(char*& chars, int level) {
	int count = 0;
	//command ushort
	memcpy(chars + count, &(this->command), USHORT_LENGTH);
	count += USHORT_LENGTH;

	//写入对应的请求的关卡
	memcpy(chars + count, &level, INT_LENGTH);
	count += INT_LENGTH;

	//list_size ushort
	memcpy(chars + count, &(this->list_size), USHORT_LENGTH);
	count += USHORT_LENGTH;

	//添加排行榜string-int对应串
	for (int i = 0; i < this->list_size; i++) {
		//username_length
		ushort usernameLength = this->users[i].getUserName().length();
		memcpy(chars + count, &usernameLength, USHORT_LENGTH);
		count += USHORT_LENGTH;

		//username
		memcpy(chars + count, this->users[i].getUserName().c_str(), usernameLength);
		count += usernameLength;

		//分数信息
		int score = 0;
		if (level == 0) {
			//score
			score = this->users[i].getScore();
		} else {
			//某一局的分数
			score = this->users[i].getGameScore(level);
		}
		memcpy(chars + count, &score, INT_LENGTH);
		count += INT_LENGTH;
	}

	return count;
}

//客户端用以序列化排行榜请求信息
//int 表示gameLevel
int NetMsg::client_ranking_serialize(char*& chars, int level) {
	int count = 0;
	//command ushort
	memcpy(chars + count, &(this->command), USHORT_LENGTH);
	count += USHORT_LENGTH;

	//写入对应的请求的关卡
	memcpy(chars + count, &level, INT_LENGTH);
	count += INT_LENGTH;

	//list_size ushort
	memcpy(chars + count, &(this->list_size), USHORT_LENGTH);
	count += USHORT_LENGTH;

	//此处只需要填入用户名即可
	for (int i = 0; i < this->list_size; i++) {
		//username_length
		ushort usernameLength = this->users[i].getUserName().length();
		memcpy(chars + count, &usernameLength, USHORT_LENGTH);
		count += USHORT_LENGTH;

		//username
		memcpy(chars + count, this->users[i].getUserName().c_str(), usernameLength);
		count += usernameLength;
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
		ptItems: ushort 2
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
		User* newUser = new User("", "");
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
		for (int i = 1; i <= GAMEAMOUNT; i++) {
			memcpy(&scorePerGame, chars + offset, INT_LENGTH);
			if (scorePerGame < 0) {
				return -1;
			}

			newUser->setGameScore(scorePerGame, i);
			offset += INT_LENGTH;
		}

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

		//解析PtItems
		int ptItems = 0;
		memcpy(&ptItems, chars + offset, USHORT_LENGTH);
		if (ptItems < 0) {
			return -1;
		}
		offset += USHORT_LENGTH;

		this->users.push_back(*newUser);
	}

	return offset;
}

/*
排行榜的结构：
command: ushort
level: int
list_size: ushort
list<User>:{
	User:{
		username_length: ushort
		username: char*
		score: int;
	}
}
*/
int NetMsg::client_ranking_deserialize(char*& chars, int& level) {
	int offset = 0;

	//解析command
	memcpy(&this->command, chars + offset, USHORT_LENGTH);
	offset += USHORT_LENGTH;

	//解析level
	memcpy(&level, chars + offset, INT_LENGTH);
	offset += INT_LENGTH;

	//解析list_size
	memcpy(&this->list_size, chars + offset, USHORT_LENGTH);
	offset += USHORT_LENGTH;

	if (this->list_size < 0) {
		return -1;
	}
	//通过获取到的list_size来遍历获取User列表
	for (int i = 0; i < this->list_size; i++) {
		User* newUser = new User("", "");
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

		//获取到字符串中的分数信息
		int score = 0;
		memcpy(&score, chars + offset, INT_LENGTH);
		if (score < 0) {
			return -1;
		}
		newUser->setRankScore(score, level);
		offset += INT_LENGTH;
		
		this->users.push_back(*newUser);
	}

	return offset;
}

//后台用以解析前台的排行榜请求
int NetMsg::server_ranking_deserialize(char*& chars, int& level) {
	int offset = 0;

	//解析command
	memcpy(&this->command, chars + offset, USHORT_LENGTH);
	offset += USHORT_LENGTH;

	//解析level
	memcpy(&level, chars + offset, INT_LENGTH);
	offset += INT_LENGTH;

	//解析list_size
	memcpy(&this->list_size, chars + offset, USHORT_LENGTH);
	offset += USHORT_LENGTH;

	if (this->list_size < 0) {
		return -1;
	}
	for (int i = 0; i < this->list_size; i++) {
		User* newUser = new User("", "");
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

		this->users.push_back(*newUser);
	}
	return offset;
}

ushort NetMsg::command_deserialize(char*& chars) {
	int offset = 0;

	//解析出字符串中的command
	ushort command = DEFAULT_COMMAND;
	memcpy(&command, chars + offset, USHORT_LENGTH);
	offset += USHORT_LENGTH;

	return command;
}

void NetMsg::addUserToList(User& user) {
	this->users.push_back(user);
	this->list_size++;
}


std::vector<User>& NetMsg::getUsers() {
	return this->users;
}

void NetMsg::setUsers(std::vector<User>& users) {
	this->users = users;
}

ushort NetMsg::getCommand() {
	return this->command;
}

void NetMsg::setCommand(ushort command) {
	this->command = command;
}
