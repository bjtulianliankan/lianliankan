/*
* Name: QinKuai
*/

#ifndef COMMON_H
#define COMMON_H

#define SERVER_IP "127.0.0.1"

#define SERVER_PORT 8888

#define BUUFER_SIZE 0x1000 // 4096

#define CHARS_MAX_LENGTH 0x1000 //4096 4kB

#define INT_LENGTH 4

#define DEFAULT_COMMAND -1;
//定义CS两端在登陆、注册和登出相关操作
//所对应的操作码
//注册操作
#define REGISTER 0
//注册成功
#define REGISTER_SUCCESS 1
//注册失败
#define REGISTER_FAIL 2

//登陆操作
#define LOGIN 3
//登陆成功
#define LOGIN_SUCCESS 4
//登陆失败
#define LOGIN_FAIL 5

//登出操作
#define LOGOUT 6
//登出成功
#define LOGOUT_SUCCESS 7
//登出失败
#define LOGOUT_FAIL 8

//定义CS两端在游戏结束时相关数据
//所对应的操作码
//信息更新
#define GAME_UPDATE 9
//信息上传成功
#define GAME_UPDATE_SUCCESS 10
//信息上传失败
#define GAME_UPDATE_FAIL 11

//定义CS两端在排行榜申请时的相关数据
//所对应的操作码
//申请排行榜信息
#define RANKING 12
//申请排行榜数据成功
#define RANKING_SUCCESS 13
//申请排行榜数据失败
#define RANKING_FAIL 14

#endif // !COMMON_H

