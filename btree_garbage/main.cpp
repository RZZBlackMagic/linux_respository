#include<iostream>
#include "bTree.h"
using namespace std;

int main(){
	b_tree<int>* tree = new b_tree<int>();
	int arr[] = {100,3,20,50,70,60,80,90,110,120,115};
//	tree->insert_node(5);
	for(int i=0;i<sizeof(arr)/sizeof(int);i++){
		tree->insert_node(arr[i]);
	}
//	cout<<"aaa"<<endl;
//	tree->insert_node(4);
//	tree->insert_node(3);
//	tree->insert_node(1);
//	tree->insert_node(2);
//	tree->insert_node(5);
	b_tree<int>::print_tree(*(tree->getRoot()));
	return 0;
}
