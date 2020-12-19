#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
/*
用两个数组放结果的两个部分，一个为空，一个放入初始时的全部元素，采用贪心算法，选择最小的加入空数组中，每次加入前
判断两个数组中的元素之和是否相等，相等则结束返回true，如果假如元素的数组之和大于减掉元素的数组之和，返回false并结束
*/
using namespace std;

void splitMarble() {
	vector<int> bVec;
	vector<int> sVec;
	//初始化容器101200
	int n;
	for (int i = 1; i <= 6; i++) {
		cin >> n;
		for (int j = 1; j <= n; j++) {
			bVec.push_back(i);
		}
	}
	//将元素从大到小排序(因为后面要从容器后出)
	 //逆向排序 即按照从大到小的顺序进行排序
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
		cout << "bsum：" << bSum << endl;
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