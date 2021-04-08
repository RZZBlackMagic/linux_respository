#include<iostream>

using namespace std;

int a = 0;

struct Node{
	int id;
	Node* pre;
	Node* next;
};

struct Link{
	Node* head;
};

Link* create_link(){
	Link* link = (Link*)malloc(sizeof(Link));
	link->head = NULL;
	Node* pre_node = NULL;
	Node* node = NULL;
	for(int i=0;i<10;i++){
		node = (Node*)malloc(sizeof(Node));
		node->id = i;
		node->next = NULL;
		//a++;
		if(i==0){
			node->pre = NULL;
			link->head = node;
			pre_node = link->head;
			continue;
		}
		pre_node->next = node;
		node->pre = pre_node;
		pre_node = pre_node->next;
	}
	return link;
}

void reverse(Link* link){
	if(!link->head){
		cout<<"Null"<<endl;
		return ;
	}
	Node* new_cur = link->head;
	if(link->head->next==NULL){
		cout<<"没必要"<<endl;
	}
	Node* cur = link->head->next;
	Node* pre_node = NULL;
	Node* node = NULL;
	while(cur->next!=NULL){
		pre_node = cur;
		node = cur->next;
		while(node->next){
			pre_node = node;
			node = node->next;
		}
		new_cur->next = node;
		node->pre = new_cur;
		pre_node->next = NULL;
		new_cur = new_cur->next;
	}
	new_cur->next = cur;
	cur->pre = new_cur;
	cur->next = NULL;
	}
void count_link(Link* link){
	int num = 0;
	if(link->head==NULL){
		cout<<"NULL"<<endl;
	}
	Node* node  = link->head;
	while(node){
		num++;
	}
	cout<<num<<endl;
}
void pre_print_link(Link* link){
	if(link->head==NULL){
		cout<<"NULL"<<endl;
		return ;
	}
	Node* node = NULL;
	node = link->head;
	while(node!=NULL){
		int i = 0;
	        i = node->id;	
		cout<< i <<endl;
		node=node->next;
	}
}

void post_print_link(Link* link){
	if(!link->head){
		cout<<"NULL"<<endl;
	}
	Node* node = link->head;
	while(node->next){
		node = node->next;
	}
	while(node){
		cout<<node->id<<endl;
		node = node->pre;
	}
}
int main(){
	Link* link = create_link();
	//count_link(link);
	pre_print_link(link);
	cout<<"houxiang:"<<endl;
	//post_print_link(link);
	reverse(link);
	pre_print_link(link);
	return 0;
}
