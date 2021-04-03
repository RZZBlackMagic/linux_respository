// 21_03.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#include <iostream>
#include <stdio.h>
#include <mutex>
#include <stdlib.h>
#include<malloc.h>
#include<set>
 #include <thread>
#include <WinSock2.h>
using namespace std;
mutex m;

class MyHash {
private:
	static MyHash* hash ;
	static int* hashArr ;
	int p;
	MyHash() {
		hashArr = (int*)malloc(10 * sizeof(int));
		memset(hashArr, -1, 10 * sizeof(int));
		initHash();
	}
	~MyHash() {
		free(hashArr);
	}
	int getSizeOfArr() {
		int size = sizeof(hashArr) / sizeof(int);
		return size;
	}
	void initHash() {
		int size = getSizeOfArr();
		int p = size;//小于size的最小素数
		if (size < 2) {
			p = 1;
		}
		while (p > 2) {
			int temp = p--;
			int i = p;
			while (temp%i != 0) {
				i--;
			}
			if (i == 1) {
				p = temp;
				break;
			}
		}
	}
	int dealConflict(int key) {
		int index = key % p;
		while (hashArr[++index] != -1);
		return index;
	}
public:
	static MyHash* getMyHash() {
		if (hash == NULL)
			return new MyHash();
		else
			return hash;
	}
	int add(int key) {
		int index = key % p;
		if (hashArr[index] == -1) {
			index = dealConflict(key);
		}
		hashArr[index] = key;
		return index;
	}
	int deleteElement(int key) {
		return 0;
	}
	int update(int newKey, int key) {
		return 0;
	}
};
MyHash* MyHash::hash = NULL;
int* MyHash::hashArr = NULL;

int main() {
	//Link* link = create_link();
	//count_link(link);
	//pre_print_link(link);
	//cout << "houxiang:" << endl;
	//post_print_link(link);
	
	set<int> s;
	MyHash* hash = MyHash::getMyHash();
	int index = hash->add(24);
	cout << index << endl;
	return 0;

}

//int main()
//{
	//server_listen();
	//client_apply();
	//thread thread_server(server_listen);
	//thread thread_client(client_apply);
	//thread_server.join();//需要t.join()，join有两个意思，一是等待子线程完毕，主线程继续向下运行；二是
	//主线程可以回收子线程，如果子线程没有被回收，则会abort，除非用detach，表示主线程不用管子线程了
	//thread_client.join();
	//cout << "回到主线程" << endl;
	//return 0;
//}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
