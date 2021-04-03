#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

using namespace std;
#pragma comment(lib,"ws2_32.lib")
//客户端发送函数
void client_send(SOCKET sock) {
	string str;
	cout << "客户端发：";
	getline(cin, str);
	const char * str1 = str.c_str();
	//const char* str = "Hello World!";
	send(sock, str1, strlen(str1) + sizeof(char), NULL);
	//关闭套接字
	closesocket(sock);
}
//客户端接受函数
void client_rev(SOCKET sock) {
	//接收服务器传回的数据
	char szBuffer[MAXBYTE] = { 0 };
	recv(sock, szBuffer, MAXBYTE, NULL);
	//输出接收到的数据
	printf("server say: %s\n", szBuffer);
	//关闭套接字
	closesocket(sock);
}
int client_apply() {
	//cout << "client start" << endl;
	//初始化DLL
	WSADATA wsaData;
	//当一个应用程序调用WSAStartup函数时，操作系统根据请求的Socket版本来搜索相应的Socket库，然后绑定
	//找到的Socket库到该应用程序中。以后应用程序就可以调用所请求的Socket库中的其它Socket函数了。该函数执行成功后返回0
	WSAStartup(MAKEWORD(2, 2), &wsaData);//不能程序使用2.2版本的socket

	while (true) {
		//创建套接字
		SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);//参数：（IP类型，数据传输方式/套接字类型，数据传输协议）
		//绑定套接字
		sockaddr_in sockAddr;//一种地址地址结构
		memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
		sockAddr.sin_family = PF_INET;  //使用IPv6地址
		sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //服务器的IP地址
		sockAddr.sin_port = htons(1234);  //端口
		//cout << "c_client_socket:" << sock << endl;
		int res = connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
		if (res == SOCKET_ERROR) {
			printf("客户端连接失败");
			break;
		}
		else {
			//printf("客户端连接成功");
			thread send_thread(client_send,sock);
			thread rev_thread(client_rev,sock);
			send_thread.join();
			rev_thread.join();
		}
	}
	
	//终止使用 DLL
	WSACleanup();
	cout << "client over" << endl;
	//system("pause");
	return 0;
}