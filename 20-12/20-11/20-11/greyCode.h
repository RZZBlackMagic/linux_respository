#pragma once
#include <iostream>
#include  <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include<string.h> 

using namespace std;
int createGreyCode(char** arr,int n) {

	if (n == 1) {
		arr[0][0] = '0';
		arr[1][0] = '1';
		return 2;
	}
	int temp = createGreyCode(arr, n - 1);
	for (int i = 0; i < temp; i++) {
		for (int k = n-1; k > 0; k--) {
			arr[i][k] = arr[i][k-1];
		}
	}
	for (int i = temp-1,k = temp; i >= 0; i--) {
		for (int j = n-1; j > 0; j--) {
			arr[k][j] = arr[i][j];
			//k++;
		}
		k++;
	}
	for (int i = 0; i < temp; i++) {
		arr[i][0] = '0';
		arr[i + temp][0] = '1';
	}
	return 2 * temp;
}
void test1114() {
	//申请一个8*3的二维数组
	char** arr ;
	cout << "请输入位数：" << endl;
	int n ;
	cin >> n;
	int temp = 1;
	for (int i = 0; i < n; i++) {
		temp = temp * 2;
	}
	//arr = (char**)malloc((2 ^ n) * sizeof(n * sizeof(char)));
	arr = new char*[temp];
	for (int i = 0; i < temp; i++) {
		*(arr + i) = new char[n];
	}
	for (int i = 0; i < temp; i++) {
		for (int j = 0; j < n; j++) {
			arr[i][j] = '0';
		}
	}
	//cout << sizeof(arr) << endl;
	int num = createGreyCode(arr,n);
	cout << n<< "位数的格雷码有" << num << "个" << endl;
	for (int i = 0; i < temp; i++) {
		for (int j = 0; j < n; j++) {
			cout << arr[i][j] << " ";
		}
		cout << endl;
	}
}