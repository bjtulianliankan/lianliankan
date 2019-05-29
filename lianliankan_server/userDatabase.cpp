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
		return false;
	}
	try {
		stmt =  conn->createStatement();
		 res =  stmt->executeQuery("select * from users;");
		while (res->next()) {
			//用户名和密码
			std::string username = res->getString("username").c_str();
			std::string passwd = res->getString("password").c_str();
			User *user = new User(username, passwd);

			//用户分数
			user->setScore(res->getInt("score"));
			user->setClearGameNumber(res->getInt("clearLevel"));
			const char* chars = res->getString("gameScore").c_str();
			user->charsToScoreArray(chars);

			//用户道具
			user->setCoins(res->getInt("coins"));
			user->setReconstructItemAmount(res->getInt("RecItems"));
			user->setTimeDelayItemAmount(res->getInt("TDItems"));

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
		return FAIL;
	}
	return SUCCESS;
}

//添加用户信息
bool UserDatabase::addToDatabase(User& user) {
	try {
		stmt = conn->createStatement();
		sql::SQLString str = "insert into users values('";
		//用户名
		str.append(user.getUserName().c_str());
		str.append("', '");
		//用户密码
		str.append(user.getPassword().c_str());
		str.append("', ");
		//总分数
		str.append(intToString(user.getScore()).c_str());
		str.append(", ");
		//金币数
		str.append(intToString(user.getCoins()).c_str());
		str.append(", ");
		//道具数量
		str.append(intToString(user.getReconstructItemAmount()).c_str());
		str.append(", ");
		//道具数量
		str.append(intToString(user.getTimeDelayItemAmount()).c_str());
		str.append(", ");
		//已通关的关卡数目
		str.append(intToString(user.getClearGameNumber()).c_str());
		str.append(", '");
		//每局分数组成的字符串
		char temp[100];
		memset(temp, 0, 100);
		char* tempChar = temp;
		user.scoreArrayTochars(tempChar);
		str.append(tempChar);
		str.append("');");

		//for check
		//std::cout << str << std::endl;
		bool result = stmt->execute(str);
		if (result) {
			delete stmt;
			return FAIL;
		}
		delete stmt;
	} catch (sql::SQLException &e) {
		LOG(e.what());
		LOG(e.getErrorCode());
		LOG(e.getSQLState());
		return FAIL;
	}
	return SUCCESS;
}

//删去用户信息
bool UserDatabase::deleteToDatabase(User& user) {
	try {
		stmt = conn->createStatement();
		sql::SQLString str = "delete from users where username='";
		str.append(user.getUserName().c_str());
		str.append("';");

		//for check
		//std::cout << str << std::endl;
		bool result = stmt->execute(str);

		if (result) {
			delete stmt;
			return FAIL;
		}
		delete stmt;
	}
	catch (sql::SQLException &e) {
		LOG(e.what());
		LOG(e.getErrorCode());
		LOG(e.getSQLState());
		return FAIL;
	}
	return SUCCESS;
}

//更新用户信息
bool UserDatabase::updateToDatabase(User& user) {
	try {
		stmt = conn->createStatement();
		sql::SQLString str = "update users set score=";
		str.append(intToString(user.getScore()).c_str());
		str.append(", coins=");
		str.append(intToString(user.getCoins()).c_str());
		str.append(", RecItems=");
		str.append(intToString(user.getReconstructItemAmount()).c_str());
		str.append(", TDItems=");
		str.append(intToString(user.getTimeDelayItemAmount()).c_str());
		str.append(", clearLevel=");
		str.append(intToString(user.getClearGameNumber()).c_str());
		str.append(", gameScore=");
		char temp[100];
		memset(temp, 0, 100);
		char* tempChar = temp;
		user.scoreArrayTochars(tempChar);
		str.append("'");
		str.append(tempChar);
		str.append("'");
		str.append(" where username='");
		str.append(user.getUserName().c_str());
		str.append("';");

		//for check
		//std::cout << str << std::endl;
		int result = stmt->executeUpdate(str);
		if (!result) {
			return FAIL;
		}
		delete stmt;
	}
	catch (sql::SQLException &e) {
		LOG(e.what());
		LOG(e.getErrorCode());
		LOG(e.getSQLState());
		return FAIL;
	}
	return SUCCESS;
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
		LOG(e.what());
		LOG(e.getErrorCode());
		LOG(e.getSQLState());
		return FAIL;
	}
	return SUCCESS;
}

//实现int和string的转换
//方便翻译为sql语句
std::string UserDatabase::intToString(const int number) {
	std::stringstream ss;
	ss << number;
	return ss.str();
}

