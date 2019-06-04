/*
* Name: QinKuai
* Status: Unfinised
*/

#include "user.h"

User::User() {};

User::User(std::string username, std::string password) {
	this->userName = username;
	this->password = password;
	
	this->score = 0;
	this->clearGameNumber = 0;
	this->gameScore.resize(GAMEAMOUNT);
	for (int i = 0; i < GAMEAMOUNT; i++) {
		this->gameScore[i] = 0;
	}
	
	this->coins = 0;
	this->timeDelayItemAmount = 0;
	this->reconstructItemAmount = 0;
	this->loginStatus = false;
}

int User::getID() {
	return this->ID;
}

void User::setID(int id) {
	this->ID = id;
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

void User::updateScore() {
	int score = 0;
	for (int i = 1; i <= GAMEAMOUNT; i++) {
		score += this->getGameScore(i);
	}
	this->setScore(score);
}

ushort User::getClearGameNumber() {
	return this->clearGameNumber;
}

void User::setClearGameNumber(ushort number) {
	this->clearGameNumber = number;
}

int User::getGameScore(int gameLevel) {
	if (gameLevel <= 0 || gameLevel > GAMEAMOUNT) {
		return -1;
	}
	return this->gameScore[gameLevel - 1];
}

void User::setGameScore(int score, int gameLevel) {
	if (gameLevel <= 0 || gameLevel > GAMEAMOUNT) {
		return;
	}
	this->gameScore[gameLevel - 1] = score;
}

//获取在排行榜中索引对应的分数
//0表示总分
//之后的整数表示单据分数
int User::getRankScore(int gameLevel) {
	if (gameLevel < 0 || gameLevel > GAMEAMOUNT) {
		return -1;
	}
	return (gameLevel == 0) ? this->score : this->getGameScore(gameLevel);
}

//设置在排行榜中索引对应的分数
//0表示总分
//之后的整数表示单据分数
void User::setRankScore(int score, int level) {
	if (level == 0) {
		this->setScore(score);
	} else {
		this->setGameScore(score, level);
	}
}

//将每局得分组成的数组转化为字符串
//以便于存入数据库
int User::scoreArrayTochars(char*& chars) {
	int count = 0;
	int gameScore = 0;
	for (int i = 1; i <= GAMEAMOUNT; i++) {
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

//将每局得分的字符串转化为得分数组
//以便得出数据库中的数据
int User::charsToScoreArray(const std::string& str) {
	int scorePerGame = 0;
	int index = 0;
	int offset = 0;
	for (int i = 1; i <= GAMEAMOUNT; i++) {
		index = str.find(SPLIT_FLAG, offset);
		std::string temp = str.substr(offset, index - offset);
		scorePerGame = atoi(temp.c_str());

		if (scorePerGame < 0) {
			return -1;
		}
		this->setGameScore(scorePerGame, i);
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