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
inline long accumlate(long start,long middle,long end) {
	return (end - middle)> 99997867?(end - middle)% 99997867: (end - middle);// % 99997867;
}
long arr[1000000] = { 0 };
long deal1(long N, long D, long start, long middle, long res) {
	if (N - start < 3) {
		return res;
	}
	if ((middle < N - 1) && arr[middle] - arr[start] <= D) {
		//cout << "A" << endl;
		long end = middle;
	 	while ((end) < (N-1)&&arr[end+1] - arr[start] <= D) {
			//cout << "B" << endl;
			end++;
		}
		cout << "start:" << start << "middle:" << middle << "end:" << end<<"   ";
		cout << accumlate(start, middle, end)<<endl;
		res = (res + accumlate(start, middle, end)) > 99997867 ? (res + accumlate(start, middle, end)) % 99997867 : (res + accumlate(start, middle, end));
		return deal1(N, D, start, middle + 1, res);
	}
	else {
		cout << "start:" << start << "middle:" << middle << endl;
		//cout << "C" << endl;
		return deal1(N, D, start + 1, start + 2, res);
	}
}
long deal2(long N,long D) {
	long start, middle, end, res = 0;
	for (start = 0; start < N - 2; start++) {
		for (middle = start+1; middle < N - 1; middle++) {
			if (arr[middle] - arr[start] > D) {
				break;
			}
			for (end = middle; end < N - 1; end++) {
				if (arr[end+1] - arr[start] > D) {
					break;
				}
			}
			res = (res+(end - middle))> 99997867?(res+end-middle)% 99997867:(res+end-middle);
		}
	}
	return res;
}
inline long accumlate1(long i) {
	long res = 0;
			res = ((i - 1) * (i - 2) / 2 );
	return res;
}
long deal3(long N,long D) {
	long start = 0, res = 0;
	long i = 0;
	bool flag;
	for (start = 0; start < N - 2; start++) {
		flag = false;
		for (i = start; i < N&&arr[i] - arr[start] <= D; i++) {
			flag = true;
		}
	//	cout << i - start +1<< endl;
		//cout << flag << endl;
		//if (flag) {
			if (i - start >= 2) {
				res = (res + accumlate1(i - start));
			}
		//}
	}
	return res;
}
void test2() {
	long N, D;
	cin >> N >> D;
	for (long i = 0; i < N;i++) {
		cin >> arr[i];
	}
//	long res = deal1(N,D,0,1,0);
	long res = deal3(N,D);
	cout << res << endl;
}