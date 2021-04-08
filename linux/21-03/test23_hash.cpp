#include<iostream>
#include<cstring>
#include "MyLinkHash.h"
using namespace std;

int main(){
	MyLinkHash* hash = MyLinkHash::getHash();

	int index24 = hash->add(24,1);
	int index25 = hash->add(25,2);
	int index17 = hash->add(17,3);
	int index16 = hash->add(16,4);
	int index13 = hash->add(13,5);
	int index1 = hash->add(1,6);
	int index172 = hash->add(172,7);
	int index7 = hash->add(7,8);
	int index8 = hash->add(8,9);
	int index19 = hash->add(19,0);
	int index3 = hash->add(3,11);
	cout<<"index24:"<<index24<<endl;
	cout<<"index25:"<<index25<<endl;
	cout<<"index17:"<<index17<<endl;
	cout<<"index16:"<<index16<<endl;
	cout<<"index13:"<<index13<<endl;
	cout<<"index1:"<<index1<<endl;
	cout<<"index172:"<<index172<<endl;
	cout<<"index7:"<<index7<<endl;
	cout<<"index8:"<<index8<<endl;
	cout<<"index19:"<<index19<<endl;
	cout<<"index3:"<<index3<<endl;
	hash->print_hash();
//	int i = hash->find_by_key(1);
//	cout<<i<<endl;
//	hash->print_hash();
//	hash->delete_entry(17,3);
//	hash->delete_entry(172,7);
//	hash->print_hash();
	return 0;
}
