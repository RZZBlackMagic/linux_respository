// 21_03.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
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

// ���г���: Ctrl + F5 ����� >����ʼִ��(������)���˵�
// ���Գ���: F5 ����� >����ʼ���ԡ��˵�

// ����ʹ�ü���: 
//   1. ʹ�ý��������Դ�������������/�����ļ�
//   2. ʹ���Ŷ���Դ�������������ӵ�Դ�������
//   3. ʹ��������ڲ鿴���������������Ϣ
//   4. ʹ�ô����б��ڲ鿴����
//   5. ת������Ŀ��>���������Դ����µĴ����ļ�����ת������Ŀ��>�����������Խ����д����ļ���ӵ���Ŀ
//   6. ��������Ҫ�ٴδ򿪴���Ŀ����ת�����ļ���>���򿪡�>����Ŀ����ѡ�� .sln �ļ�
