/*
* Name: QinKuai
* Status: Unfinished
*/
#include "server.h"

Server::Server() {}

Server::~Server() {}

/*
��ʼ����������������
*/
bool Server::init() {
	//����Winsock
	//��дWSA��Ϣ
	WSADATA wsadata;
	int error = WSAStartup(0x0202, &wsadata);

	//�鿴�Ƿ����쳣����
	if (error) {
		LOG("Winsock�����쳣")
		return false;
	}
	if (wsadata.wVersion != 0x0202) {
		//���Winsock
		WSACleanup();
		LOG("Winsock�汾�쳣")
		return false;
	}

	//��ʼ��TCP socket�Ľṹ
	this->addr.sin_family = PF_INET;
	this->addr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, SERVER_IP, &this->addr.sin_addr.s_addr);

	//����socket
	this->socket1 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//����socketʧ��
	if (socket1 == INVALID_SOCKET) {
		LOG("Socket����ʧ��")
		return false;
	}

	//��Socket
	if (bind(this->socket1, (LPSOCKADDR)&this->addr, sizeof(this->addr)) == SOCKET_ERROR) {
		LOG("Socket�޷���")
		return false;
	}

	int ret = listen(this->socket1, 1);

	if (ret == SOCKET_ERROR) {
		LOG("Socket����ʧ��")
		return false;
	}

	return true;
}

/*
��������������֮��
���ø÷���
��ʼ�������������Ϣ
����ʼ�����ͻ�������
*/
void Server::start() {
	if (!this->init()){
		LOG("��������ʼ��ʧ��")
		return;
	}

	LOG("��������ʼ�����")

	//��ʽ��ʼ�����ͻ�����Ϣ
	//������
	char* recv_buf = new char[BUFFER_SIZE];
	//�����ͻ��˻�����
	char* send_buf = new char[BUFFER_SIZE];

	while (true) {
		//socekt�Ͽ�ֱ���˳�����������
		if (this->socket1) {
			delete[] recv_buf;
			return;
		}
		//��������ʼ�����ǻ���������
		memset(recv_buf, 0, BUFFER_SIZE);
		memset(send_buf, 0, BUFFER_SIZE);

		SOCKADDR_IN client_addr;
		socklen_t client_length = sizeof(client_addr);
		SOCKET client = accept(this->socket1, (struct sockaddr*)&client_addr, &client_length);
		int len = recv(client, recv_buf, BUFFER_SIZE, 0);
		
		//���������л�����
		//ȷ���ͻ�����������
		NetMsg *client_request = new NetMsg();
		//���ͻ���������Ϣ
		//�����л����ص�������
		client_request->deserialize(recv_buf, DEFAULT_MODE);

		//�����ͻ���ʱ�����л�����ָ��
		NetMsg *send_back = nullptr;


		//��ʱ��client_request�Ѿ�ʱ�����õ�NetMsg������
		switch (client_request->getCommand()) {
		case REGISTER:
			//��������ʵ���û�ע��
			 send_back = userRegister(client_request);

			client_request->serialize(send_buf, SECRET_MODE);
			//��ͻ��˷���ע�����
			send(client, send_buf, BUFFER_SIZE, 0);
				
			delete client_request;
			delete send_back;
			continue;
		case LOGIN:
			//��������ʵ���û���½
			 send_back = userLogin(client_request);

			 client_request->serialize(send_buf, SECRET_MODE);
			 //��ͻ��˷�����½���
			 send(client, send_buf, BUFFER_SIZE, 0);
			 
			 delete client_request;
			 delete send_back;
			continue;
		case LOGOUT:
			//��������ʵ���û��ǳ�
			 send_back = userLogout(client_request);

			 client_request->serialize(send_buf, SECRET_MODE);
			 //��ͻ��˷����ǳ����
			 send(client, send_buf, BUFFER_SIZE, 0);

			 delete client_request;
			 delete send_back;
			continue;
		case USERDATA_UPDATE:
			//��������ʵ���û����ݸ���
			 send_back = userdataUpdate(client_request);

			 client_request->serialize(send_buf, SECRET_MODE);
			 //��ͻ��˷����û����ݸ������
			 send(client, send_buf, BUFFER_SIZE, 0);
			 
			 delete client_request;
			 delete send_back;
			continue;
		case RANKING:
			//��������ʵ����������
			send_back = userRanking();
			continue;
		}
	}
}

/*
������ͣ�÷���
*/
void Server::close() {
	if (this->socket1) {
		closesocket(this->socket1);
	}
	WSACleanup();
	LOG("�����������˳�")
}

/*
�����ע�᷽��
*/
NetMsg* Server::userRegister(NetMsg* msg) {
	User *user = &msg->getUsers().front();
	//�������˵ı���LOG��Ϣ
	std::string log = user->getUserName();

	NetMsg* send_back = new NetMsg();

	//��ѯUserBase���Ƿ�����ͬ�û������û�
	if (UserBase::getUserBase()->findUser(user->getUserName()) != nullptr) {
		send_back->setCommand(REGISTER_FAIL);
		log.append(" �û����Ѵ��ڣ�ע��ʧ��");
		LOG(log)
		return send_back;
	}

	//д�����ݿ���
	bool result = UserDatabase::addToDatabase(*user);
	if (!result) {
		send_back->setCommand(REGISTER_FAIL);
		log.append(" д�����ݿ�ʧ�ܣ�ע��ʧ��");
		LOG(log)
		return send_back;
	}

	//��ӵ�UserBase��
	UserBase::getUserBase()->addUser(*user);
	
	send_back->setCommand(REGISTER_SUCCESS);
	log.append(" ע��ɹ�");
	LOG(log)
	return send_back;
}

/*
����˵�½����
*/
NetMsg* Server::userLogin(NetMsg* msg) {
	User* user = &msg->getUsers().front();
	//�������˵ı���LOG��Ϣ
	std::string log = user->getUserName();

	NetMsg* send_back = new NetMsg();

	//��UserBase��ά����list�в�ѯ�û��Ƿ����
	User* user_send_back = UserBase::getUserBase()->findUser(user->getUserName());
	if (user_send_back == nullptr) {
		send_back->setCommand(LOGIN_FAIL);
		log.append(" �޸��û�����½ʧ��");
		LOG(log)
		return send_back;
	}

	//���ҳɹ���������˺�֮��ȷ���û���½
	user_send_back->setLoginStatus(true);

	//�������ҵ����û��෵���ͻ���
	send_back->setCommand(LOGIN_SUCCESS);
	send_back->addUserToList(*user_send_back);

	log.append(" ��½�ɹ�");
	LOG(log)

	return send_back;
}

/*
����˵ǳ�����
*/
NetMsg* Server::userLogout(NetMsg* msg) {
	User* user = &msg->getUsers().front();
	//�������˵ı���LOG��Ϣ
	std::string log = user->getUserName();

	NetMsg* send_back = new NetMsg();

	//���û��ĵ�½״̬����Ϊ�ǳ�
	UserBase::getUserBase()->findUser(user->getUserName())->setLoginStatus(false);

	//���õǳ��ɹ�
	send_back->setCommand(LOGOUT_SUCCESS);

	log.append(" �ǳ��ɹ�");
	LOG(log)
	return send_back;
}


/*
��������ݸ��·���
*/
NetMsg* Server::userdataUpdate(NetMsg* msg) {
	User* user = &msg->getUsers().front();
	//�������˵ı���LOG��Ϣ
	std::string log = user->getUserName();

	NetMsg* send_back = new NetMsg();

	//����ָ����û���ָ��
	User* user_in_server = UserBase::getUserBase()->findUser(user->getUserName(), user->getPassword());
	
	//�������ݿ��е��û�����
	bool result = UserDatabase::updateToDatabase(*user);

	if (!result) {
		log.append(" ���ݸ���ʧ��");
		LOG(log)
		send_back->setCommand(USERDATA_UPDATE_FAIL);
		return send_back;
	}

	//�����û���Ϣ
	//���������Ϣ
	user_in_server->setScore(user->getScore());
	user_in_server->setClearGameNumber(user->getClearGameNumber());
	for (int i = 1; i <= user_in_server->getClearGameNumber(); i++) {
		user_in_server->setGameScore(user->getGameScore(i), i);
	}
	//���������Ϣ
	user_in_server->setCoins(user->getCoins());
	user_in_server->setReconstructItemAmount(user->getReconstructItemAmount());
	user_in_server->setTimeDelayItemAmount(user->getTimeDelayItemAmount());

	//�������ݸ��³ɹ�
	send_back->setCommand(USEREDATA_UPDATE_SUCCESS);

	log.append(" ���ݸ��³ɹ�");
	LOG(log)
	return send_back;
}


/*
���������а�����
*/
NetMsg* Server::userRanking() {
	return nullptr;

}