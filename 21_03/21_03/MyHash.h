#pragma once
class MyHash {
private:
	static MyHash* hash;
	static int* hashArr;
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
