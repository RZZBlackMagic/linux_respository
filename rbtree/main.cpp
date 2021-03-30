#include<iostream>
#include "RBTree.h"
using namespace std;
int main(){
	RBTree<int>* rbt = new RBTree<int>();
	int arr[] = {20,12,7,9,10,8,5,6,4,13,11};
	int length = sizeof(arr)/sizeof(int);
	for(int i=0;i<length;i++){
		rbt->insert_node(arr[i]);
//		cout<<"__________________________"<<endl;
//		cout<<"插入"<<arr[i]<<"后："<<endl;
//		print_tree(rbt->getRoot());
	}
	print_tree(rbt->getRoot());
	rbt->delete_node(8);
	print_tree(rbt->getRoot());
//	rbt->delete_node(4);
//	print_tree(rbt->getRoot());
//	rbt->delete_node(6);
//	print_tree(rbt->getRoot());
	//rbt->delete_node(20);
	//print_tree(rbt->getRoot());
	//rbt->delete_node(6);
	//print_tree(rbt->getRoot());
	return 0;
}
