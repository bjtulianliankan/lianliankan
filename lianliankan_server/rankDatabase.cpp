/*
* Name: QinKuai
* Status: Finished
*/

#include "rankDatabase.h"

//静态变量的初始化
sql::Driver* RankDatabase::driver = nullptr;
sql::Connection* RankDatabase::conn = nullptr;
sql::Statement* RankDatabase::stmt = nullptr;
sql::ResultSet* RankDatabase::res = nullptr;


RankDatabase::RankDatabase() {}

//将数据库中的数据加载到RankBase中
bool RankDatabase::databaseToRanking() {
	if (!init()) {
		LOG("数据初始化失败")
		return DB_FAIL;
	}

	try {
		stmt = conn->createStatement();
		res = stmt->executeQuery("select * from ranking;");

		while (res->next()) {
			//获取gameLevel
			int gameLevel = res->getInt("gameLevel");
			//获取排行榜的string
			std::string rank = res->getString("rankByID");

			//将获取到的排行榜信息写入RankBase
			//依次存入RankBase的积分排行榜中
			RankBase::getRankBase()->stringToScoreRank(gameLevel, rank);
		}

		delete res;
		delete stmt;
	} catch (sql::SQLException &e) {
		LOG(e.what());
		LOG(e.getErrorCode());
		LOG(e.getSQLState());
		return DB_FAIL;
	}
	return DB_SUCCESS;
}

//更新数据库中的排行榜数据
//int表示对应的gameLevel
//std::string表示新的排行信息
//使用用户ID进行排行
bool RankDatabase::updateToDatabase(int gameLevel, std::string rankMsg) {
	try {
		stmt = conn->createStatement();
		sql::SQLString str = "update ranking set rankByID='";
		str.append(rankMsg);
		str.append("' where gameLevel=");
		str.append(intToString(gameLevel).c_str());
		str.append(";");

		//执行SQL
		stmt->executeUpdate(str);

		delete stmt;
	}
	catch (sql::SQLException &e) {
		LOG(e.what());
		LOG(e.getErrorCode());
		LOG(e.getSQLState());
		delete stmt;
		return DB_FAIL;
	}
	return DB_SUCCESS;
}

//关闭数据库连接
void RankDatabase::close() {
	delete res;
	delete stmt;
	conn->close();
}

//创建与排行榜数据库的连接
bool RankDatabase::init() {
	try {
		//建立数据库连接
		driver = get_driver_instance();
		conn = driver->connect("127.0.0.1:3306", "root", "ab123456");

		//连接MySQL数据库lianliankan
		conn->setSchema("lianliankan");
	} catch (sql::SQLException &e) {
		LOG(e.what())
		LOG(e.getErrorCode())
		LOG(e.getSQLState())
		return DB_FAIL;
	}
	return DB_SUCCESS;
}