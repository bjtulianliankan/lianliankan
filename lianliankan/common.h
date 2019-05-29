/*
* Name: QinKuai
*/
#pragma once
#ifndef COMMON_H
#define COMMON_H
//����Windows�����µ�����ͷ�ļ�
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")

#define SERVER_IP "127.0.0.1"

#define SERVER_PORT 8888

#define BUFFER_SIZE 0x1000 // 4096

#define CHARS_MAX_LENGTH 0x1000 //4096 4kB

#define INT_LENGTH 4

#define DEFAULT_COMMAND -1;
//����CS�����ڵ�½��ע��͵ǳ���ز���
//����Ӧ�Ĳ�����
//ע�����
#define REGISTER 0
//ע��ɹ�
#define REGISTER_SUCCESS 1
//ע��ʧ��
#define REGISTER_FAIL 2

//��½����
#define LOGIN 3
//��½�ɹ�
#define LOGIN_SUCCESS 4
//��½ʧ��
#define LOGIN_FAIL 5

//�ǳ�����
#define LOGOUT 6
//�ǳ��ɹ�
#define LOGOUT_SUCCESS 7
//�ǳ�ʧ��
#define LOGOUT_FAIL 8

//����CS��������Ϸ����ʱ�������
//����Ӧ�Ĳ�����
//��Ϣ����
#define USERDATA_UPDATE 9
//��Ϣ�ϴ��ɹ�
#define USEREDATA_UPDATE_SUCCESS 10
//��Ϣ�ϴ�ʧ��
#define USERDATA_UPDATE_FAIL 11

//����CS���������а�����ʱ���������
//����Ӧ�Ĳ�����
//�������а���Ϣ
#define RANKING 12
//�������а����ݳɹ�
#define RANKING_SUCCESS 13
//�������а�����ʧ��
#define RANKING_FAIL 14

//#define LOG(logMsg) std::cout << "[LOG]" << "[" << __DATE__ << " " << __TIME__ << "][" << __FILE__ << "][" << __LINE__ << "]" << logMsg << std::endl;

#endif // !COMMON_H

