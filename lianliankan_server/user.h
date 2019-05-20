/*
* Name: QinKuai
* Status: Unfinised
*/

#ifndef USER_H
#define USER_H
#include <string>
#include "common.h"

class User {
public:
	User();
	User(std::string, std::string);

	std::string getUserName();
	void setUserName(std::string);
	std::string getPassword();
	void setPassword(std::string);
	int getScore();
	void setScore(int);
	int getCoins();
	void setCoins(int);
	int getTimePauseItemAmount();
	void setTimePauseItemAmount(int);
	int getTimeDelayItemAmount();
	void setTimeDelayItemAmount(int);
private:
	//用户名
	std::string userName;
	//用户密码
	std::string password;
	//总得分
	int score;
	//道具相关
	//金币数
	int coins;
	//暂停道具持有数
	int timePauseItemAmount;
	//延时道具持有数
	int timeDelayItemAmount;

	//在此基础上可以加上
	//分局积分
	//int score_game1;
	//用户当前已到达的局数，前提是整个游戏拥有多局游戏
	//int finished;
};

#endif // !USER_H

