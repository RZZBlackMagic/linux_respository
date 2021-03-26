#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

using namespace std;
#pragma comment(lib,"ws2_32.lib")

int client_apply() {
	cout << "client start" << endl;
	//��ʼ��DLL
	WSADATA wsaData;
	//��һ��Ӧ�ó������WSAStartup����ʱ������ϵͳ���������Socket�汾��������Ӧ��Socket�⣬Ȼ���
	//�ҵ���Socket�⵽��Ӧ�ó����С��Ժ�Ӧ�ó���Ϳ��Ե����������Socket���е�����Socket�����ˡ��ú���ִ�гɹ��󷵻�0
	WSAStartup(MAKEWORD(2, 2), &wsaData);//���ܳ���ʹ��2.2�汾��socket

	//�����׽���
	SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);//��������IP���ͣ����ݴ��䷽ʽ/�׽������ͣ����ݴ���Э�飩
	//���׽���
	sockaddr_in sockAddr;//һ�ֵ�ַ��ַ�ṹ
	memset(&sockAddr, 0, sizeof(sockAddr));  //ÿ���ֽڶ���0���
	sockAddr.sin_family = PF_INET;  //ʹ��IPv6��ַ
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //��������IP��ַ
	sockAddr.sin_port = htons(1234);  //�˿�
	cout << "c_client_socket:" << sock << endl;
	int res = connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
	if (res == SOCKET_ERROR) {
		printf("����ʧ��");
		return 0;
	}
	else {
		printf("���ӳɹ�");
	}
	//���շ��������ص�����
	char szBuffer[MAXBYTE] = { 0 };
	recv(sock, szBuffer, MAXBYTE, NULL);
	//������յ�������
	printf("Message form server: %s\n", szBuffer);
	//�ر��׽���
	closesocket(sock);
	//��ֹʹ�� DLL
	WSACleanup();
	cout << "client over" << endl;
	//system("pause");
	return 0;
}