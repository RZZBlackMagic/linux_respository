#include<iostream>

using namespace std;

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
	Node* node = NULL;
	Node* pre_node = NULL;
	for(int i=0;i<10;i++){
		node = (Node*)malloc(sizeof(Node));
		node->id = i;
		if(i==0){
			link->head = node;
			node->next = link->head;
			node->pre =  link->head;
			pre_node = node;
		}
		pre_node->next = node;
		node->pre = pre_node;
		node->next = link->head;
		link->head->pre = node;
		pre_node = pre_node->next;
	}
	return link;
}

void reverse(Link* link){
	Node* temp = NULL;
	if(link->head==link->head->next){
		cout<<"NULL"<<endl;
		return ;
	}
	Node* post_node = link->head->next;
	if(post_node==link->head){
		cout<<"没必要"<<endl;
		return ;
	}
	Node* cur = link->head;
	temp = cur->next;
	cur->next = cur->pre;
	cur->pre = temp;
	cur = cur->pre;
	while(cur!=link->head){
		temp = cur->next;
		cur->next = cur->pre;
		cur->pre = temp;
		cur = cur->pre;
	}

}
void pre_print_link(Link* link){
	if(link->head->next==link->head){
		cout<<"NULL"<<endl;
		return;
	}
	cout<<link->head->id<<endl;
	Node* node = link->head->next;
	if(node->next==link->head){
		cout<<"没必要"<<endl;
	}
	while(node!=link->head){
		cout<<node->id<<endl;
		node = node->next;
	}
}

void post_print_link(Link* link){
	if(link->head->next==link->head){
		cout<<"NULL"<<endl;
	}
	cout<<link->head->id<<endl;
	Node* node = link->head->next;
	if(node->next==link->head){
		cout<<"没必要"<<endl;
	}
	while(node->next!=link->head){
		node = node->next;
	}
	while(node!=link->head){
		cout<<node->id<<endl;
		node = node->pre;
	}
	
}
int main(){
	Link* link = create_link();
	pre_print_link(link);
	cout<<"houxiang"<<endl;
	post_print_link(link);
	reverse(link);
	cout<<"......."<<endl;
	pre_print_link(link);
	post_print_link(link);
	return 0;
}
