/*
* Name: QinKuai
* Status: Unfinised
*/

#include "user.h"

User::User() {
	User("", "");
}

User::User(std::string username, std::string password) {
	this->userName = username;
	this->password = password;
	
	this->score = 0;
	this->clearGameNumber = 0;
	
	this->coins = 0;
	this->timeDelayItemAmount = 0;
	this->reconstructItemAmount = 0;
	this->loginStatus = false;
}

std::string User::getUserName() {
	return this->userName;
}

void User::setUserName(std::string username) {
	this->userName = username;
}

std::string User::getPassword() {
	return this->password;
}

void User::setPassword(std::string password) {
	this->password = password;
}

int User::getScore() {
	return this->score;
}

void User::setScore(int score) {
	this->score = score;
}

ushort User::getClearGameNumber() {
	return this->clearGameNumber;
}

void User::setClearGameNumber(ushort number) {
	this->clearGameNumber = number;
}

int User::getGameScore(int gameLevel) {
	if (gameLevel <= 0 || gameLevel > this->clearGameNumber) {
		return -1;
	}
	return this->gameScore[gameLevel - 1];
}

void User::setGameScore(int score, int gameLevel) {
	if (gameLevel <= 0 || gameLevel > this->clearGameNumber) {
		return;
	}
	this->gameScore.resize(gameLevel);
	this->gameScore[gameLevel - 1] = score;
}

//将每局得分组成的数组转化为字符串
//以便于存入数据库
int User::scoreArrayTochars(char*& chars) {

	int count = 0;
	int gameScore = 0;
	for (int i = 1; i <= this->clearGameNumber; i++) {
		gameScore = this->getGameScore(i);
		//务必保证分数不超过8位正数
		_itoa_s(gameScore, chars + count, 8, 10);
		count += numberLength(gameScore);
		//添加分隔符
		memset(chars + count, SPLIT_FLAG, 1);
		count++;
	}

	return count;
}

//将每局得分的字符串转化为等分数组
//以便得出数据库中的数据
int User::charsToScoreArray(const char*& chars) {
	std::string str(chars);
	int level = this->getClearGameNumber();
	int gamePerGame = 0;
	int index = 0;
	int offset = 0;
	for (int i = 1; i <= level; i++) {
		index = str.find('#', offset);
		std::string temp = str.substr(offset, index - offset);
		gamePerGame = atoi(temp.c_str());

		if (gamePerGame < 0) {
			return -1;
		}
		this->setGameScore(gamePerGame, i);
		offset = index + 1;
	}
	return offset;
}

int User::getCoins() {
	return this->coins;
}

void User::setCoins(int coins) {
	this->coins = coins;
}

ushort User::getTimeDelayItemAmount() {
	return this->timeDelayItemAmount;
}

void User::setTimeDelayItemAmount(ushort amount) {
	this->timeDelayItemAmount = amount;
}

ushort User::getReconstructItemAmount() {
	return this->reconstructItemAmount;
}

void User::setReconstructItemAmount(ushort amount) {
	this->reconstructItemAmount = amount;
}

bool User::getLoginStatus() {
	return this->loginStatus;
}

void User::setLoginStatus(bool status) {
	this->loginStatus = status;
}