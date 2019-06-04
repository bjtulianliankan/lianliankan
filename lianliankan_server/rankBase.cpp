/*
* Name: QinKuai
* Status: Finished
*/

#include "rankBase.h"

//静态变量初始化
int RankBase::gameAmount = GAMEAMOUNT;
RankBase* RankBase::rankBaseInstance = nullptr;

//隐藏的构造器
RankBase::RankBase() {
	this->rank.resize(RankBase::gameAmount + 1);
}

//保证该实例只被创建一次
RankBase* RankBase::getRankBase() {
	if (RankBase::rankBaseInstance == nullptr) {
		RankBase::rankBaseInstance = new RankBase();
	}
	return RankBase::rankBaseInstance;
}

//将从数据库中获取到的排行榜字符串数据
//转化为排行榜数组信息
//将数据库中的数据初始化到线上的RankBase中
int RankBase::stringToScoreRank(int gameLevel, std::string str) {
	//对应gameLevel的数组初始化
	this->rank[gameLevel].resize(10);

	int length = str.length();
	int counter = 0;
	int tempID = 0;
	int index = 0;
	int offset = 0;
	while (true) {
		index = str.find(RANK_SPLIT, offset);
		if (index == -1) {
			return -1;
		}
		std::string temp = str.substr(offset, index - offset);
		tempID = atoi(temp.c_str());

		if (tempID <= 0) {
			return -1;
		}
		this->rank[gameLevel][counter++] = tempID;
		offset = index + 1;
	}
}

//在服务器端存在用户积分相关信息发生变化后
//此方法用以更新在RankBase上的排行榜信息
std::string RankBase::scoreRankToString(int gameLevel) {
	std::string str("");
	int count = 0;
	int tempID = 0;
	int length = (UserBase::getUsersLength() < 10) ? UserBase::getUsersLength() : 10;

	for (int i = 0; i < length; i++) {
		tempID = this->rank[gameLevel][i];
		str.append(intToString(tempID));
		str += RANK_SPLIT;
	}
	return str;
}

//针对未存在于数据库的用户
//当有新用户注册时
//检查可插入信息后，向排行榜中插入相关信息
bool RankBase::insertToRankBase(const int ID) {
	if (canInsert()) {
		for (int i = 0; i <= GAMEAMOUNT; i++) {
			this->rank[i][UserBase::getUsersLength() - 1] = ID;
		}
		return true;
	}
	return false;
}

//检查可以被插入排行榜中
//int表示gameLevel
bool RankBase::canInsert() {
	if (UserBase::getUsersLength() > 10) {
		return false;
	}
	return true;
}

//将获取到的格式化的字符转转化为排行榜信息
//并存入对应gameLevel的数组中
bool RankBase::updateToRankBase(const int ID) {
	//i遍历所有的gameLevel
	for (int i = 0; i <= GAMEAMOUNT; i++) {
		if (needToUpdate(ID, i)) {
			//表示排行榜中已有该用户
			//在已有的分数下更新
			int index = findID(ID, i);
			if (index != -1) {
				//擦除掉已有原用户ID
				std::vector<int>::iterator it = this->rank[i].begin();
				it += index;
				this->rank[i].erase(it);
				//插入新的原用户ID
				for (it = this->rank[i].begin(); it < this->rank[i].end(); it++) {
					int oldScore = UserBase::getUserBase()->findUser(*it)->getRankScore(i);
					int newScore = UserBase::getUserBase()->findUser(ID)->getRankScore(i);
					if (newScore > oldScore) {
						this->rank[i].insert(it, ID);
						break;
					}
					if (*(it + 1) == 0 || (it + 1) == this->rank[i].end()) {
						this->rank[i].insert(it + 1, ID);
						break;
					}
				}
			//表示排行榜中还没有该用户
			//直接插入该用户的分数信息
			} else {
				for (std::vector<int>::iterator it = this->rank[i].begin(); it < this->rank[i].end(); it++) {
					int oldScore = UserBase::getUserBase()->findUser(*it)->getRankScore(i);
					int newScore = UserBase::getUserBase()->findUser(ID)->getRankScore(i);
					if (newScore > oldScore) {
						this->rank[i].insert(it, ID);
						break;
					}
				}
				//剔除掉最后一个多出来的元素
				//保证数组始终长度为10
				this->rank[i].pop_back();
			}
		}
	}
	return true;
}

//确定是否需要在排行榜数组中更新信息
//只要对应gameLevel的分数高于当前排行榜的最后一位的分数
//便确定更新
bool RankBase::needToUpdate(const int ID, int gameLevel) {
	int last = (UserBase::getUsersLength() < 10) ? UserBase::getUsersLength() - 1 : 9;
	//当排行榜的最后一位为传入的ID时
	//直接允许更新
	//可以保证这次的分数一定比之前的高
	if (this->rank[gameLevel][last] == ID) {
		return true;
	}
	int lastScore = UserBase::getUserBase()->findUser(this->rank[gameLevel][last])->getRankScore(gameLevel);
	int newScore = UserBase::getUserBase()->findUser(ID)->getRankScore(gameLevel);
	if (lastScore >= newScore) {
		return false;
	}
	return true;
}

//查找排行榜中的ID
//ID不存在返回-1
int RankBase::findID(int ID, int gameLevel) {
	for (int i = 0; i < 10; i++) {
		if (ID == this->rank[gameLevel][i]) {
			return i;
		}
	}
	return -1;
}

//获取到指定gameLevel的排行榜数组
std::vector<int>& RankBase::getRankByLevel(int level) {
	return this->rank[level];
}

//for test
void RankBase::showAll() {
	int length = (UserBase::getUsersLength() < 10) ? UserBase::getUsersLength() : 10;
	for (int i = 0; i <= RankBase::getGameAmount(); i++) {
		std::cout << "gameLevel " << i << ": ";
		for (int j = 0; j < length; j++) {
			if (UserBase::getUserBase()->findUser(rank[i][j]) != nullptr) {
				std::cout << UserBase::getUserBase()->findUser(rank[i][j])->getRankScore(i) << " ";
			}
			//std::cout << this->rank[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

int RankBase::getGameAmount() {
	return RankBase::gameAmount;
}