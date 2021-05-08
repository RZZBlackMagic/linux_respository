#pragma once
#include<iostream>
#include<vector>

using namespace std;

int deal(int n) {
	int res = 0;
	while (n > 2) {
		n -= 2;
		res++;
	}
	if (n == 2) {
		res++;
	}
	return res;
}

void test() {
	int n = 1;
	vector<int> res;
	while (n) {
		cin >> n;
		if(n!=0)
			res.push_back(deal(n));
	}
	auto it = res.begin();
	while (it != res.end()) {
		cout << *it << endl;
		it++;
	}
}

void test1() {
	int N, n;
	while (cin >> N) {
		int a[1001] = { 0 };
		while (N--) {
			cin >> n;
			a[n] = 1;
		}
		for (int i = 0; i < 1001; i++)
			if (a[i])
				cout << i << endl;
	}
}
int weight = 1;
long long Ox2Ten(string str) {
	long long res = 0;
	for (int i = str.length()-1; i > 0; i--) {
		char x = str.at(i);
		cout << x << endl;
		if (x != 'x') {
			if (i < str.length() - 1)
				weight *= 16;
			if (x == '0') {
				 (res += 0 * weight);
			}
			else if (x == '1') {
				 (res += 1 * weight);
			}
			else if(x=='2'){
				 (res += 2 * weight);
			}
			else if (x == '3') {
			     (res += 3 * weight);
			}
			else if (x == '4') {
				 (res += 4 * weight);
			}
			else if (x == '5') {
			     (res += 5 * weight);
			}
			else if (x == '6') {
				(res += 6 * weight);
			}
			else if (x == '7') {
				 (res += 7 * weight);
			}
			else if (x == '8') {
				 res += 8 * weight;
			}
			else if (x == '9') {
				 res += 9 * weight;
			}
			else if (x == 'A') {
				res += 10 * weight;
			}
			else if (x == 'B') {
				res += 11 * weight;
			}
			else if (x == 'C') {
				res += 12 * weight;
			}
			else if (x == 'D') {
				res += 13 * weight;
			}
			else if (x == 'E') {
				res += 14 * weight;
			}
			else if (x == 'F') {
				res += 15 * weight;
			}else {
				cout << "false" << endl;
			}			
		}
		else {
			break;
		}
	}
	return res;
}

void test2() {
	string str ;
	vector<long long> vec;
	while ( 1) {
		long long res = Ox2Ten(str);
		vec.push_back(res);
	}
	for (auto it = vec.begin(); it != vec.end(); it++) {
		cout << *it << endl;
	}
	//string str = "0x11";
	//long long res = Ox2Ten(str);
	//cout << res << endl;
}
void test3() {

}