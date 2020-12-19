#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
/*
����������Ž�����������֣�һ��Ϊ�գ�һ�������ʼʱ��ȫ��Ԫ�أ�����̰���㷨��ѡ����С�ļ���������У�ÿ�μ���ǰ
�ж����������е�Ԫ��֮���Ƿ���ȣ�������������true���������Ԫ�ص�����֮�ʹ��ڼ���Ԫ�ص�����֮�ͣ�����false������
*/
using namespace std;

void splitMarble() {
	vector<int> bVec;
	vector<int> sVec;
	//��ʼ������101200
	int n;
	for (int i = 1; i <= 6; i++) {
		cin >> n;
		for (int j = 1; j <= n; j++) {
			bVec.push_back(i);
		}
	}
	//��Ԫ�شӴ�С����(��Ϊ����Ҫ���������)
	 //�������� �����մӴ�С��˳���������
	sort(bVec.rbegin(), bVec.rend());
	for (vector<int>::iterator it = bVec.begin(); it != bVec.end(); it++) {
		cout << *it << " ";
	}
	int bSum = accumulate(bVec.begin(), bVec.end(), 0);
	int sSum = 0;
	int now;
	while (sSum <= bSum) {
		bSum = accumulate(bVec.begin(), bVec.end(), 0);
		sSum = accumulate(sVec.begin(), sVec.end(), 0);
		cout << "bsum��" << bSum << endl;
		cout << "sSum:" << sSum << endl;
		if (sSum == bSum) {
			cout << "true" << endl;
			return;
		}
		now = bVec.back();
		bVec.pop_back();
		sVec.push_back(now);
	}
	cout << "false" << endl;
	return;
}