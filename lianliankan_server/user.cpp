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
	this->coins = 0;
	this->timeDelayItemAmount = 0;
	this->timePauseItemAmount = 0;
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

int User::getCoins() {
	return this->coins;
}

void User::setCoins(int coins) {
	this->coins = coins;
}

int User::getTimePauseItemAmount() {
	return this->timePauseItemAmount;
}

void User::setTimePauseItemAmount(int amount) {
	this->timePauseItemAmount = amount;
}

int User::getTimeDelayItemAmount() {
	return this->timeDelayItemAmount;
}

void User::setTimeDelayItemAmount(int amount) {
	this->timeDelayItemAmount = amount;
}