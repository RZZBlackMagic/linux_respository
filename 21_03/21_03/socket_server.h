#pragma once
#include <stdio.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

int server_listen(){
	cout << "����˿�ʼ��" << endl;
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
	SOCKET clntSock;
	//while (true) {
		clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
		//��ͻ��˷�������
		if (clntSock == INVALID_SOCKET) {
			cout << "dengdailianjie" << endl;
		}
		else {
			cout << "���ӳɹ�" << endl;
			//break;
		}
		
	//}
	cout << "server socket:" << servSock << endl;
	cout << "client socket:" << clntSock << endl;
	const char* str = "Hello World!";
	send(clntSock, str, strlen(str) + sizeof(char), NULL);
	//�ر��׽���
	closesocket(clntSock);
	closesocket(servSock);
	//��ֹ DLL ��ʹ��
	WSACleanup();
	cout << "fuwuduan jieshule"<< endl;
	return 0;
}