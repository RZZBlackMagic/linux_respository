#pragma once
#include<iostream>
using namespace std;
int m;
int** arr = new int *[m];
void jumpChess(int** arr, int x, int y) {
	//�ݹ��˳�����
	//cout << arr << endl;
	if (arr[x][y] + x >= m && arr[x][y] + y >= m) {
		return;
	}
	if (x == m-1 || y == m-1) {
		//�ܴﵽ
		cout << "true" << endl;
		return;
	}
	if (x >= m || y >= m) {
		return;
	}
	//cout << "arr[x][y]="<<arr[x][y]<<";x="<<x <<"y="<<y<< endl;
	/*cout << "&arr[0][0]" << arr << endl;
	if (arr[x][y] > m-x||arr[x][y] > m-y) {
		
	}*/
	//��������
	//int * a = *arr + arr[0][0];
	jumpChess(arr, x, y + arr[x][y]);
	//��������
	/*int m = arr[0][0];
	cout << m << endl;
	int *b = *arr + arr[0][0] * y;
	cout << arr << endl;
	cout << &b << endl;
	cout << *b << endl;*/
	jumpChess(arr, x + arr[x][y], y);
}
