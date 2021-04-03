#pragma once
#include <stdio.h>
#include <WinSock2.h>
#include<iostream>
#include<string>
#pragma comment(lib, "ws2_32.lib")

using namespace std;
void server_send(SOCKET sock) {
	string str;
	cout << "服务端发：";
	getline(cin, str);
	const char * str1 = str.c_str();
	//const char* str = "Hello World!";
	send(sock, str1, strlen(str1) + sizeof(char), NULL);
	//关闭套接字
	closesocket(sock);
}
void server_rev(SOCKET sock) {
	//接收服务器传回的数据
	char szBuffer[MAXBYTE] = { 0 };
	recv(sock, szBuffer, MAXBYTE, NULL);
	//输出接收到的数据
	printf("\n client say: %s\n", szBuffer);
	//关闭套接字
	closesocket(sock);
}
int server_listen(){
	//cout << "服务端开始了" << endl;
	//初始化DLL
	WSADATA wsaData;
	//当一个应用程序调用WSAStartup函数时，操作系统根据请求的Socket版本来搜索相应的Socket库，然后绑定
	//找到的Socket库到该应用程序中。以后应用程序就可以调用所请求的Socket库中的其它Socket函数了。该函数执行成功后返回0
	WSAStartup(MAKEWORD(2, 2), &wsaData);//不能程序使用2.2版本的socket

	//创建套接字
	SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);//参数：（IP类型，数据传输方式/套接字类型，数据传输协议）
	//绑定套接字
	sockaddr_in sockAddr;//一种地址地址结构
	memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
	sockAddr.sin_family = PF_INET;  //使用IPv6地址
	sockAddr.sin_addr.s_addr = INADDR_ANY;// inet_addr("127.0.0.1");  //具体的IP地址
	sockAddr.sin_port = htons(1234);  //端口
	::bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
	//进入监听状态
	if (listen(servSock, 20) == SOCKET_ERROR) {
		cout << "监听出错" << endl;
	}
	// 接收客户端请求
	sockaddr clntAddr;//一种地址结构，和sockaddr_in相对应
	int nSize = sizeof(SOCKADDR);
	while (true) {
		//accpet() 函数的作用是读取已完成连接队列中的第一项（读完就从队列中移除），并对此项
		//生成一个用于后续连接的套接字描述符（姑且用 connfd 来表示），有了新的连接套接字，
		//用户进程/线程（称其为工作者）就可以通过这个连接套接字和客户端进行数据传输，而前文所说的监听套接字（sockfd）则仍然被监听者监听。
		SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
		//cout << "server socket:" << servSock << endl;
		//cout << "client socket:" << clntSock << endl;
		//向客户端发送数据
		if (clntSock == INVALID_SOCKET) {
			cout << "服务端连接失败" << endl;
		}
		else {
			//cout << "服务段连接成功" << endl;
			//break;
			thread rev_thread(server_rev, clntSock);
			thread send_thread(server_send, clntSock);
			rev_thread.join();
			send_thread.join();
		}

	}
	closesocket(servSock);
	//终止 DLL 的使用
	WSACleanup();
	cout << "server over"<< endl;
	return 0;
}