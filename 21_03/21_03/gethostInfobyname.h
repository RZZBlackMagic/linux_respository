// 21_03.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
using namespace std;
int get()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	struct hostent *host = gethostbyname("cn.bing.com");
	if (!host) {
		puts("Get IP address error!");
		system("pause");
		exit(0);
	}
	for (int i = 0; host->h_aliases[i]; i++) {
		cout << "aliases" << i + 1 << ":" << host->h_aliases[i] << endl;
	}
	if (host->h_addrtype = AF_INET) {
		cout << "addrtype:" << "AF_INET" << endl;
	}
	else {
		cout << "addrtype:" << "AF_INET" << endl;
	}
	for (int i = 0; host->h_addr_list[i]; i++) {
		cout << "IP address" << i + 1 << ";" << inet_ntoa(*(struct in_addr*)host->h_addr_list[i]) << endl;
	}
	std::cout << "Hello World!\n";
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
