#include<iostream>

using namespace td;
typedef enum{RED=0,BLACK}Color;
template<typename T>
struct RETNode{
	Color color;
	T key;
	RETNode* left;
	RETNode* right;
	RETNode* parent;
};


int main(){
	return 0;
}
