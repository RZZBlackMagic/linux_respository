#include<iostream>

using namespace std;

int a = 0;
struct Node{
	int id;
	Node* next;
};

struct Link{
	Node* head;
};

Link* create_link(){
	Link* link = (Link*)malloc(sizeof(Link));
	link->head = NULL;
	Node* cur = NULL;
	for(int i=0;i<10;i++){
		Node* node = (Node*)malloc(sizeof(Node));
		node->id = a;
		node->next = NULL;
		a++;
		if(a==1){
			link->head = node;
			cur = link->head;
			continue;
		}
		cur->next = node;
		cur = cur->next;
	}
	cur->next = link->head;
	return link;
}

void print_link(Link* link){
	if(link->head==NULL){
		cout<<link->head->id<<endl;
		return;
	}
	if(link->head->next==link->head){
		cout<<link->head->id<<endl;
		return ;
	}
	cout<<link->head->id<<endl;
	Node* node = link->head->next;
	while(node!=link->head){
		cout<<node->id<<endl;
		node = node->next;
	}
}

void reverse(Link* link){
	if(link->head->next==link->head){
		return ;
	}
	Node* new_cur = link->head;
	Node* cur = link->head->next;
	Node* pre_node = NULL;
	if(cur->next==link->head){
		return;
	}
	Node* node = NULL;
	while(cur->next!=link->head){
		pre_node = cur;
		node = pre_node->next;
		while(node->next!=link->head){
			pre_node = node;
			node = node->next;
		}
		new_cur->next = node;
		pre_node->next = link->head;
		new_cur = new_cur->next;
	}
	new_cur->next = cur;
}
void pre_reverse(Link* link){
	if(link->head==link->head->next){
		cout<<"NULL"<<endl;
		return ;
	}
	Node* node = link->head->next;
	if(node->next==link->head){
		cout<<"没必要"<<endl;
		return ;
	}
	Node* post_node = node->next;
	Node* temp = NULL;
	link->head->next = link->head;
	while(node!=link->head){
		temp = link->head->next;
		link->head->next = node;
		node->next = temp;
		node = post_node;
		post_node = post_node->next;
	}
}
int main(){
	Link* link = create_link();
	print_link(link);
	pre_reverse(link);
	print_link(link);
	return 0;
}
