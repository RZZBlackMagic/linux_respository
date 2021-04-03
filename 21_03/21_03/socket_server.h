#pragma once
#include <stdio.h>
#include <WinSock2.h>
#include<iostream>
#include<string>
#pragma comment(lib, "ws2_32.lib")

using namespace std;
void server_send(SOCKET sock) {
	string str;
	cout << "����˷���";
	getline(cin, str);
	const char * str1 = str.c_str();
	//const char* str = "Hello World!";
	send(sock, str1, strlen(str1) + sizeof(char), NULL);
	//�ر��׽���
	closesocket(sock);
}
void server_rev(SOCKET sock) {
	//���շ��������ص�����
	char szBuffer[MAXBYTE] = { 0 };
	recv(sock, szBuffer, MAXBYTE, NULL);
	//������յ�������
	printf("\n client say: %s\n", szBuffer);
	//�ر��׽���
	closesocket(sock);
}
int server_listen(){
	//cout << "����˿�ʼ��" << endl;
	//��ʼ��DLL
	WSADATA wsaData;
	//��һ��Ӧ�ó������WSAStartup����ʱ������ϵͳ���������Socket�汾��������Ӧ��Socket�⣬Ȼ���
	//�ҵ���Socket�⵽��Ӧ�ó����С��Ժ�Ӧ�ó���Ϳ��Ե����������Socket���е�����Socket�����ˡ��ú���ִ�гɹ��󷵻�0
	WSAStartup(MAKEWORD(2, 2), &wsaData);//���ܳ���ʹ��2.2�汾��socket

	//�����׽���
	SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);//��������IP���ͣ����ݴ��䷽ʽ/�׽������ͣ����ݴ���Э�飩
	//���׽���
	sockaddr_in sockAddr;//һ�ֵ�ַ��ַ�ṹ
	memset(&sockAddr, 0, sizeof(sockAddr));  //ÿ���ֽڶ���0���
	sockAddr.sin_family = PF_INET;  //ʹ��IPv6��ַ
	sockAddr.sin_addr.s_addr = INADDR_ANY;// inet_addr("127.0.0.1");  //�����IP��ַ
	sockAddr.sin_port = htons(1234);  //�˿�
	::bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
	//�������״̬
	if (listen(servSock, 20) == SOCKET_ERROR) {
		cout << "��������" << endl;
	}
	// ���տͻ�������
	sockaddr clntAddr;//һ�ֵ�ַ�ṹ����sockaddr_in���Ӧ
	int nSize = sizeof(SOCKADDR);
	while (true) {
		//accpet() �����������Ƕ�ȡ��������Ӷ����еĵ�һ�����ʹӶ������Ƴ��������Դ���
		//����һ�����ں������ӵ��׽����������������� connfd ����ʾ���������µ������׽��֣�
		//�û�����/�̣߳�����Ϊ�����ߣ��Ϳ���ͨ����������׽��ֺͿͻ��˽������ݴ��䣬��ǰ����˵�ļ����׽��֣�sockfd������Ȼ�������߼�����
		SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
		//cout << "server socket:" << servSock << endl;
		//cout << "client socket:" << clntSock << endl;
		//��ͻ��˷�������
		if (clntSock == INVALID_SOCKET) {
			cout << "���������ʧ��" << endl;
		}
		else {
			//cout << "��������ӳɹ�" << endl;
			//break;
			thread rev_thread(server_rev, clntSock);
			thread send_thread(server_send, clntSock);
			rev_thread.join();
			send_thread.join();
		}

	}
	closesocket(servSock);
	//��ֹ DLL ��ʹ��
	WSACleanup();
	cout << "server over"<< endl;
	return 0;
}