/*
* Name: QinKuai
* Status: Unfinised
*/
#pragma once
#ifndef USER_H
#define USER_H

#ifndef INT_LENGTH
#define INT_LENGTH 4
#endif // !INT_LENGTH

#define SPLIT_FLAG '$'

#include <string>
#include <string.h>
#include "tool.h"
#include <vector>

typedef unsigned short ushort;

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
	ushort getClearGameNumber();
	void setClearGameNumber(ushort);
	int getGameScore(int);
	void setGameScore(int, int);
	//将每局得分组成的数组转化为字符串
	//以便于存入数据库
	int scoreArrayTochars(char*&);
	//将每局得分的字符串转化为等分数组
	//以便得出数据库中的数据
	int charsToScoreArray(const char*&);

	int getCoins();
	void setCoins(int);
	ushort getReconstructItemAmount();
	void setReconstructItemAmount(ushort);
	ushort getTimeDelayItemAmount();
	void setTimeDelayItemAmount(ushort);

	bool getLoginStatus();
	void setLoginStatus(bool);
private:
	//用户名
	//15个字符以内
	std::string userName;
	//用户密码
	std::string password;

	//积分相关
	//总得分
	int score;
	//当前已经通关的局等级
	//1表示通过了关卡1，以此类推
	ushort clearGameNumber;
	//每局游戏的积分
	std::vector<int> gameScore;

	//道具相关
	//金币数
	int coins;
	//延时道具持有数
	ushort timeDelayItemAmount;
	//重构道具持有数
	ushort reconstructItemAmount;

	//记录用户的登陆状态
	bool loginStatus;

	//在此基础上可以加上
	//分局积分
	//int score_game1;
	//用户当前已到达的局数，前提是整个游戏拥有多局游戏
	//int finished;
};

#endif // !USER_H

