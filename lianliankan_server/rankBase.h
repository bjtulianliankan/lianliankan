/*
* Name: QinKuai
* Status: Finished
*/
#pragma once

#ifndef RANKBASE_H
#define RANKBASE_H

#define RANK_SPLIT '$'
#include "gameSettings.h"
#include <string>
#include <vector>
#include "userBase.h"
#include "databaseCommon.h"

//for test
#include <iostream>

//该类用以在服务器运行时直接将排行榜信息
//拉至线上以查找
class RankBase {
public:
	//保证该实例只被创建一次
	static RankBase* getRankBase();

	//将数据库中的数据初始化到线上的RankBase中
	//int表示对用的gameLevel
	//string表示数据库中的格式化字符串
	//该方法进行解析并存入数组
	int stringToScoreRank(int, std::string);

	//将排行榜信息转化为结构化字符串
	//便于存入数据库
	//int表示gameLevel
	std::string scoreRankToString(int);

	//针对未存在于数据库的用户
	//当有新用户注册完成后
	//检查可插入信息后，向排行榜中插入相关信息
	bool insertToRankBase(const int);

	//检查可以被插入排行榜中
	//就是检验当前已注册人数是否超过了10人
	//已确定新用户注册时是否需要插入排行榜
	bool canInsert();
	
	//针对已存在于数据库的用户
	//在服务器端存在用户积分相关信息发生变化后
	//此方法用以更新在RankBase上的排行榜信息
	bool updateToRankBase(const int);

	//确定是否需要在排行榜数组中更新信息
	//int用以指定gameLevel
	//只要对应gameLevel的分数高于当前排行榜的最后一位的分数
	//便确定更新
	bool needToUpdate(const int, int);

	//查找排行榜中的ID
	//查找到返回该ID所在的index
	//ID不存在返回-1
	//第二个int表示gameLevel
	int findID(int, int);

	//获取到指定gameLevel的排行榜数组
	std::vector<int>& getRankByLevel(int);

	static int getGameAmount();
private:
	RankBase();
	//单实例
	static RankBase* rankBaseInstance;
	//该实例中维护一个排行榜列表
	//排行榜是通过直接存储用户ID实现
	//通过gameLevel获取对应局游戏的排行榜
	std::vector<std::vector<int>> rank;
	//记录整个游戏已有的游戏局数
	static int gameAmount;
};

#endif // !RANKBASE_H

