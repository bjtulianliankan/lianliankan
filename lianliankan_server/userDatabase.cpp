/*
* Name: QinKuai
* Status: Finished
*/

#include "userDatabase.h"

//静态变量初始化
sql::Driver* UserDatabase::driver = nullptr;
sql::Connection* UserDatabase::conn = nullptr;
sql::Statement* UserDatabase::stmt = nullptr;
sql::ResultSet* UserDatabase::res = nullptr;


UserDatabase::UserDatabase() {}

//将数据库中的数据拉到UserBase中
//实现用户数据管理于list中
bool UserDatabase::databaseToUsers() {
	if (!init()) {
		LOG("数据库初始化失败")
		return DB_FAIL;
	}
	try {
		stmt =  conn->createStatement();
		res =  stmt->executeQuery("select * from users;");
		while (res->next()) {
			//用户名和密码
			std::string username = res->getString("username");
			std::string passwd = res->getString("password");
			User *user = new User(username, passwd);

			//用户ID
			user->setID(res->getInt("ID"));

			//用户分数
			user->setScore(res->getInt("score"));
			user->setClearGameNumber(res->getInt("clearLevel"));
			user->charsToScoreArray(res->getString("gameScore"));

			//用户道具
			user->setCoins(res->getInt("coins"));
			user->setReconstructItemAmount(res->getInt("RecItems"));
			user->setTimeDelayItemAmount(res->getInt("TDItems"));
			user->setPropmtItemAmount(res->getInt("PTItems"));

			UserBase::getUserBase()->addUser(*user);

			//for test
			//UserBase::getUserBase()->showAll();
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

//添加用户信息
bool UserDatabase::addToDatabase(User& user) {
	try {
		stmt = conn->createStatement();
		sql::SQLString str = "insert into users values('";
		//用户名
		str.append(user.getUserName());
		str.append("', '");
		//用户密码
		str.append(user.getPassword());
		str.append("', ");
		//总分数
		str.append(intToString(user.getScore()));
		str.append(", ");
		//金币数
		str.append(intToString(user.getCoins()));
		str.append(", ");
		//道具数量
		str.append(intToString(user.getReconstructItemAmount()));
		str.append(", ");
		//道具数量
		str.append(intToString(user.getTimeDelayItemAmount()));
		str.append(", ");
		//已通关的关卡数目
		str.append(intToString(user.getClearGameNumber()));
		str.append(", '");
		//每局分数组成的字符串
		char temp[100];
		memset(temp, 0, 100);
		char* tempChar = temp;
		user.scoreArrayTochars(tempChar);
		str.append(tempChar);
		str.append("',");
		//用户在服务器中注册的ID
		str.append(intToString(user.getID()));
		str.append(", ");
		//道具数量
		str.append(intToString(user.getPropmtItemAmount()));
		str.append(");");

		//for check
		//std::cout << str << std::endl;
		bool result = stmt->execute(str);
		if (result) {
			delete stmt;
			return DB_FAIL;
		}
		delete stmt;
	} catch (sql::SQLException &e) {
		LOG(e.what());
		LOG(e.getErrorCode());
		LOG(e.getSQLState());
		return DB_FAIL;
	}
	return DB_SUCCESS;
}

//删去用户信息
bool UserDatabase::deleteToDatabase(User& user) {
	try {
		stmt = conn->createStatement();
		sql::SQLString str = "delete from users where username='";
		str.append(user.getUserName());
		str.append("';");

		//for check
		//std::cout << str << std::endl;
		bool result = stmt->execute(str);

		if (result) {
			delete stmt;
			return DB_FAIL;
		}
		delete stmt;
	}
	catch (sql::SQLException &e) {
		LOG(e.what());
		LOG(e.getErrorCode());
		LOG(e.getSQLState());
		return DB_FAIL;
	}
	return DB_SUCCESS;
}

//更新用户信息
bool UserDatabase::updateToDatabase(User& user) {
	try {
		stmt = conn->createStatement();
		sql::SQLString str = "update users set score=";
		str.append(intToString(user.getScore()));
		str.append(", coins=");
		str.append(intToString(user.getCoins()));
		str.append(", RecItems=");
		str.append(intToString(user.getReconstructItemAmount()));
		str.append(", TDItems=");
		str.append(intToString(user.getTimeDelayItemAmount()));
		str.append(", PTItems=");
		str.append(intToString(user.getPropmtItemAmount()));
		str.append(", clearLevel=");
		str.append(intToString(user.getClearGameNumber()));
		str.append(", gameScore=");
		char temp[100];
		memset(temp, 0, 100);
		char* tempChar = temp;
		user.scoreArrayTochars(tempChar);
		str.append("'");
		str.append(tempChar);
		str.append("'");
		str.append(" where username='");
		str.append(user.getUserName());
		str.append("';");

		//for check
		//std::cout << str << std::endl;
		int result = stmt->executeUpdate(str);
		if (!result) {
			return DB_FAIL;
		}
		delete stmt;
	}
	catch (sql::SQLException &e) {
		LOG(e.what());
		LOG(e.getErrorCode());
		LOG(e.getSQLState());
		return DB_FAIL;
	}
	return DB_SUCCESS;
}

//此方法来关闭数据库连接
void UserDatabase::close() {
	delete res;
	delete stmt;
	conn->close();
}

//创建用户数据库对象时初始化与数据库的连接
bool UserDatabase::init() {
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
