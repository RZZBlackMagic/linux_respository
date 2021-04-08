#include<RBTree.h>

using namespace std;

RBtree::RBtree(){
	Nil = create_node();
	root = Nil;
	Nil->color = BLACK;
}

RBTNode<T>* RBTree::create_node(T x){
	RBTNode<T>* node = new RBTNode<T>();
	assert(node!=NULL);
	node->key = x;
	node->left = Nil;
	node->right = Nil;
	node->parent = Nil;
	node->color = RED;
	return node;
}
RBTree::~RBtree(){
	destory_tree(root);
	delete Nil;
	Nil = NULL;
}
int RBTree::insert_node(RBTNode<T>* node){
	return 0;
}
int RBTree::delete_node(RBTNode<T>* node){
	return 0;
}
int RBTree::print_tree(RBTNode<T>* root){
	return 0;
}
void RBTree::left_rotate(RBTNode<T>* node){
}
void RBTree::right_rotate(RBTNode<T>* node){
}
void RBTree::insert_fix(){
}
void RBTree::delete_fix(){
}
RBTNode<T>* RBTree::find_replace_node(RBTNode<T>* node){
	return NULL;
}
void RBTree::print_tree(){
	if(root!=Nil){
		cout<<root->key<<" "<<root->color<<endl;;
		print_tree(root->left);
		print_tree(root->right);
	}
}
