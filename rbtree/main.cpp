#include<iostream>
#include "RBTree.h"

int main(){
	RBTree<int>* rbt = new RBTree<int>();
	int arr[] = {1,7,3,9,5,8,2,15,12,11,10};
	int length = sizeof(arr)/sizeof(int);
	for(int i=0;i<length;i++)
		rbt->insert_node(arr[i]);
	print_tree(rbt->getRoot());
	return 0;
}
