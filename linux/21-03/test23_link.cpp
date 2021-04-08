#include<iostream>
#include<malloc.h>
using namespace std;

int a = 0;

struct Node{
	int node_id;
	int value;
	Node* next;
};

struct Link{
	Node* head;
};

Link* create_link(){
	Link* link = (Link*)malloc(sizeof(Link));
	link->head = NULL;
	Node* cur = NULL;
	Node* next = NULL;
	for(int i=0;i<10;i++){
		next = (Node*)malloc(sizeof(Node));
		next->node_id = a;
		next->value = a;
		next->next = NULL;
		if(a==0){
			link->head = next;
			cur = next;
		}else{
			cur->next = next;
			cur = cur->next;
		}
		a++;
	}
	return link;
}

int count_link(Link* link){
	int num = 0;
	Node* cur = link->head;
	while(cur!=NULL){
		num++;
		cur = cur->next;
	}
	cur = NULL;
	return num;
}

void turn_around(Link* link){
	int num = count_link(link);
	cout<<num<<endl;
	if(link==NULL){
		return ;
	}
	Node* cur = link->head->next;
	if(!cur){
		return ;
	}
	Node* node = NULL;
	Node* pre_node = NULL;
	Node* cur_new = link->head;
	for(int i=0;i<num;i++){
		pre_node = cur;
		node = pre_node->next;
		if(node){
			while(node->next){
				pre_node = node;
				node = node->next;
			}
		}else{
			node = cur;
		}
		cout<<"id:"<<node->node_id<<endl;
		cur_new->next = node;
		cur_new = cur_new->next;
		pre_node->next=NULL;
	}
	cur_new->next=NULL;
}

void reverse(Link* link){
	if(link->head==NULL){
		cout<<"NULL"<<endl;
		return ;
	}
	Node* node = link->head->next;
	if(node==NULL){
		cout<<"没必要"<<endl;
		return ;
	}
	Node* post_node = node->next;
	link->head->next = NULL;
	Node* temp = NULL;
	while(post_node!=NULL){
		temp = link->head->next;
		link->head->next = node;
		node->next = temp;
		node = post_node;
		post_node = post_node->next;
	}
	temp = link->head->next;
	link->head->next = node;
	node->next = temp;
}
void reverse1(Link* link){
	if(link->head==NULL){
		cout<<"NULL"<<endl;
		return;
	}
	Node* h = link->head;
	Node* node = link->head->next;
	if(node==NULL){
		cout<<"没必要"<<endl;
		return ;	
	}
	Node* post_node = node->next;
	node->next = NULL;
	while(post_node){
		Node* temp = post_node->next;
		post_node->next = node;
		node = post_node;
		post_node = temp;
	}
	link->head->next = node;
}
void print_link(Link* link){
	Node* node = link->head;
	while(node){
		cout<<node->node_id<<endl;
		node = node->next;
	}
	node = NULL;
}
int main(){
	Link* link = create_link();
	print_link(link);
	cout<<"反转"<<endl;
	reverse1(link);
	print_link(link);
	return 0;
}
