#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

using namespace std;
#pragma comment(lib,"ws2_32.lib")
//�ͻ��˷��ͺ���
void client_send(SOCKET sock) {
	string str;
	cout << "�ͻ��˷���";
	getline(cin, str);
	const char * str1 = str.c_str();
	//const char* str = "Hello World!";
	send(sock, str1, strlen(str1) + sizeof(char), NULL);
	//�ر��׽���
	closesocket(sock);
}
//�ͻ��˽��ܺ���
void client_rev(SOCKET sock) {
	//���շ��������ص�����
	char szBuffer[MAXBYTE] = { 0 };
	recv(sock, szBuffer, MAXBYTE, NULL);
	//������յ�������
	printf("server say: %s\n", szBuffer);
	//�ر��׽���
	closesocket(sock);
}
int client_apply() {
	//cout << "client start" << endl;
	//��ʼ��DLL
	WSADATA wsaData;
	//��һ��Ӧ�ó������WSAStartup����ʱ������ϵͳ���������Socket�汾��������Ӧ��Socket�⣬Ȼ���
	//�ҵ���Socket�⵽��Ӧ�ó����С��Ժ�Ӧ�ó���Ϳ��Ե����������Socket���е�����Socket�����ˡ��ú���ִ�гɹ��󷵻�0
	WSAStartup(MAKEWORD(2, 2), &wsaData);//���ܳ���ʹ��2.2�汾��socket

	while (true) {
		//�����׽���
		SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);//��������IP���ͣ����ݴ��䷽ʽ/�׽������ͣ����ݴ���Э�飩
		//���׽���
		sockaddr_in sockAddr;//һ�ֵ�ַ��ַ�ṹ
		memset(&sockAddr, 0, sizeof(sockAddr));  //ÿ���ֽڶ���0���
		sockAddr.sin_family = PF_INET;  //ʹ��IPv6��ַ
		sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //��������IP��ַ
		sockAddr.sin_port = htons(1234);  //�˿�
		//cout << "c_client_socket:" << sock << endl;
		int res = connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
		if (res == SOCKET_ERROR) {
			printf("�ͻ�������ʧ��");
			break;
		}
		else {
			//printf("�ͻ������ӳɹ�");
			thread send_thread(client_send,sock);
			thread rev_thread(client_rev,sock);
			send_thread.join();
			rev_thread.join();
		}
	}
	
	//��ֹʹ�� DLL
	WSACleanup();
	cout << "client over" << endl;
	//system("pause");
	return 0;
}