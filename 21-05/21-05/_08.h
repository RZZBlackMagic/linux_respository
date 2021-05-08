#pragma once
#include <iostream>
#include<vector>
#include<string>
using namespace std;
string deal(string str,int start) {
	int length = str.length();
	int len = length-start;
	if (len < 3) {
		return str;
	}
	if (len >= 3) {
		if (str.at(start) == str.at(start+1) && str.at(start+1) == str.at(start+2)) {
			return deal(str.substr(0, start+2)+str.substr(start+3,start+len),start);
		}
	}
	if (len >= 4) {
		if (str.at(start) == str.at(start+1) && str.at(start+2) == str.at(start+3)) {
			return deal(str.substr(0, start+2) + str.substr(start+3, start+len),start);
		}
	}
	if (len >= 6) {
		if (str.at(start) == str.at(start+1) && str.at(start+2) == str.at(start+3) && str.at(start+4) == str.at(start+5)) {
			return deal(str.substr(0, start+2) + str.substr(start+3, start+len),start);
		}
	}
	return deal(str,start+1);
}
void test1() {
	vector<string> res;
	int N;
	string str;
	cin >> N ;
	getchar();
	while (N--) {
		std::getline(std::cin,str);
		res.push_back(deal(str,0));
	}
	for (auto it = res.begin(); it != res.end(); it++) {
		cout << *it << endl;
	}
}
long accumlate(long start,long middle,long end) {
	return ((middle - start)*(end - middle));// % 99997867;
}
long arr[1000000] = { 0 };
long deal1(long N, long D, long start, long middle, long res) {
	if (N - start < 3) {
		return 0;
	}
	if ((middle < N - 1) && arr[middle] - arr[start] <= D) {
		cout << "A" << endl;
		long end = middle +1;
		while ((end) < (N-1)&&arr[end] - arr[middle] <= D) {
			cout << "B" << endl;
			end++;
		}
		return deal1(N, D, start, middle + 1, res + accumlate(start, middle, end));
	}
	else {
		cout << "C" << endl;
		return deal1(N, D, start + 1, start + 2, res);
	}
}
void test2() {
	long N, D;
	cin >> N >> D;
	for (long i = 0; i < N;i++) {
		cin >> arr[i];
	}
	cout << deal1(N, D, 0, 1, 0) << endl;
}