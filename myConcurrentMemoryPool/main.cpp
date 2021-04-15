#include<iostream>
#include "common.h"
void printList(Spanlist* list){
	Span* span = list->begin();
	while(span->_page_id!=0){
		std::cout<<"page_id:"<<span->_page_id<<std::endl;
		span = span->_next;
	}
}
void freeListTest(){
	Spanlist* list = new Spanlist();
	for(int i=1;i<10;i++){
		Span* span = new Span;
		span->_page_id = i;
		list->push_front(span);
	}
	list->pop_front();
	list->pop_back();
	printList(list);
}

int main(){
	//freeListTest();
	int i =1;
	std::cout<<(i>>12)<<std::endl;
	return 0;
}
