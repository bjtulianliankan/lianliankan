/*
* Name: QinKuai
* Status: Finished
*/
#pragma once
#ifndef COMMON_H
#define COMMON_H
//引用Windows环境下的网络头文件
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")

//#define SERVER_IP "127.0.0.1"
#define SERVER_IP "192.168.1.100"

#define SERVER_PORT 8888

//在TCP/IP协议下BUFFER定在1024
#define BUFFER_SIZE 0x400 // 1024

#define CHARS_MAX_LENGTH 0x400 //1024 1kB

#define INT_LENGTH 4

#define USHORT_LENGTH 2

#define DEFAULT_COMMAND 0
//定义CS两端在登陆、注册和登出相关操作
//所对应的操作码
//注册操作
#define REGISTER 1
//注册成功
#define REGISTER_SUCCESS 2
//注册失败
#define REGISTER_FAIL 3

//登陆操作
#define LOGIN 4
//登陆成功
#define LOGIN_SUCCESS 5
//登陆失败
#define LOGIN_FAIL 6

//登出操作
#define LOGOUT 7
//登出成功
#define LOGOUT_SUCCESS 8
//登出失败
#define LOGOUT_FAIL 9

//定义CS两端在游戏结束时相关数据
//所对应的操作码
//信息更新
#define USERDATA_UPDATE 10
//信息上传成功
#define USEREDATA_UPDATE_SUCCESS 11
//信息上传失败
#define USERDATA_UPDATE_FAIL 12

//定义CS两端在排行榜申请时的相关数据
//所对应的操作码
//申请排行榜信息
#define RANKING 13
//申请排行榜数据成功
#define RANKING_SUCCESS 14
//申请排行榜数据失败
#define RANKING_FAIL 15


#endif // !COMMON_H

