#pragma once
#include <stdio.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

int server_listen(){
	cout << "服务端开始了" << endl;
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
	SOCKET clntSock;
	//while (true) {
		clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
		//向客户端发送数据
		if (clntSock == INVALID_SOCKET) {
			cout << "dengdailianjie" << endl;
		}
		else {
			cout << "连接成功" << endl;
			//break;
		}
		
	//}
	cout << "server socket:" << servSock << endl;
	cout << "client socket:" << clntSock << endl;
	const char* str = "Hello World!";
	send(clntSock, str, strlen(str) + sizeof(char), NULL);
	//关闭套接字
	closesocket(clntSock);
	closesocket(servSock);
	//终止 DLL 的使用
	WSACleanup();
	cout << "fuwuduan jieshule"<< endl;
	return 0;
}