#include "RBTNode.h"
#include <cassert>
using namespace std;
static int FLAG = 0;  //0:zuo   1:you
const int INIT_CONST = -1;
template<typename T>
class RBTree{
	private:
		RBTNode<T>* root;
		RBTNode<T>* Nil;
		//创建节点
		RBTNode<T>* create_node(T x);
		//左旋转
		void left_rotate(RBTNode<T>* z);
		//右旋转
		void right_rotate(RBTNode<T>* z);
		//插入后调整
		void insert_fix(RBTNode<T>* node);
		//删除后调整
		void delete_fix();
		//寻找后继节点用来替代
		RBTNode<T>* find_replace_node(RBTNode<T>* node);
		//通过父节点获取叔叔节点
		RBTNode<T>* find_uncle_by_parent(RBTNode<T>* parent);
	public:
		//无参构造
		RBTree();
		RBTree(T x);
		~RBTree();
		RBTNode<T>* getRoot();
		//寻找插入位置
		RBTNode<T>* find_insert_place(T x);
		//插入
		int insert_node(T x);
		//删除
		int delete_node(RBTNode<T>* node);
		//销毁树
		int destory_tree(RBTNode<T>* root);
		//打印
		//void print_tree();
};
template<typename T>
RBTNode<T>* RBTree<T>::find_uncle_by_parent(RBTNode<T>* parent){
	if(parent == parent->parent->left){
		//父节点是祖父节点的左孩子
		return parent->parent->right;
	}else{
		return parent->parent->right;
	}
};
template<typename T>
RBTNode<T>* RBTree<T>::find_insert_place(T x){
	RBTNode<T>* node = getRoot();
	//cout<<"root:"<<node->key<<endl;
	if(node->key==INIT_CONST){
		//树为空
		return node;
	}
	RBTNode<T>* pre_node = NULL;
	while(node->key!=INIT_CONST){
		if(x>node->key){
			pre_node = node;
			node = node->right;
			FLAG = 1;
		}else if(x<node->key){
			pre_node = node;
			node = node->left;
			FLAG = 0;
		}else{
			cout<<"插入的数树中有了"<<endl;
		}
	}
	if(pre_node==NULL){
		cout<<"插入时，寻找位置出现错"<<endl;
	}
	return pre_node;
};
template<typename T>
RBTree<T>::RBTree(){
        this->Nil = create_node(-1);
        this->root = this->Nil;
        this->Nil->color = BLACK;
};
template<typename T>
RBTNode<T>* RBTree<T>::create_node(T x){
        RBTNode<T>* node = (RBTNode<T>*)malloc(sizeof(RBTNode<T>));
        assert(node!=NULL);
        node->key = x;
        node->left = Nil;
        node->right = Nil;
        node->parent = Nil;
        node->color = RED;
        return node;
};
template<typename T>
RBTree<T>::~RBTree(){
        destory_tree(root);
        delete Nil;
        Nil = NULL;
};
template<typename T>
int RBTree<T>::insert_node(T x){
	RBTNode<T>* p = find_insert_place(x);
	RBTNode<T>* new_node = create_node(x);
	if(p==NULL){
		cout<<"p is NULL"<<endl;
	//	cout<<x<<endl;
	}
	//cout<<p->key<<endl;
	if(p->key==INIT_CONST){
		//树为空，插入跟节点
		new_node->color = BLACK;
		this->root = new_node;
	}
	if(FLAG==0)
		p->left = new_node;
		new_node->parent = p;
	if(FLAG==1)
		p->right = new_node;
		new_node->parent = p;
	insert_fix(new_node);
        return 0;
};
template<typename T>
int RBTree<T>::delete_node(RBTNode<T>* node){
        return 0;
};
template<typename T>
void RBTree<T>::left_rotate(RBTNode<T>* cur){
	RBTNode<T>* p =  cur->parent;
	cur->parent = cur->parent->parent;
	if(p->parent->left==p->parent)
		p->parent->left = cur;
	else
		p->parent->right = cur;
	cur->right = p;
	p->parent = cur;
	p->left = Nil;
};
template<typename T>
void RBTree<T>::right_rotate(RBTNode<T>* cur){
	RBTNode<T>* p = cur->parent;
	cur->left = p;
	cur->parent = p->parent;
	if(p->parent->left==p->parent)
		p->parent->left = cur;
	else
		p->parent->right = cur;
	p->parent = cur;
	p->right = Nil;
};
template<typename T>
void RBTree<T>::insert_fix(RBTNode<T>* cur){
	RBTNode<T>* p = cur->parent;
	cout<<"p->key:"<<p->key<<endl;
        if(p->color==RED){
                RBTNode<T>* uncle = find_uncle_by_parent(p);
                RBTNode<T>* node = cur;
                if(uncle->color==RED){
			//将父节点和祖父节点变色，往上递归处理
                        while(node->parent->color==RED){
				node->parent->color = BLACK;
				find_uncle_by_parent(node->parent)->color = BLACK;
				node->parent->parent->color = RED;
				node = node->parent->parent;	
                        }	
			getRoot()->color = BLACK;
			return ;
                }
                if(uncle->key==-1||uncle->color==BLACK){
			if(cur->parent->parent->left==cur->parent){
				//当前节点的父节点是祖父节点的左孩子
				if(FLAG==1){
					//新增节点是父节点的右孩子
					//先将当前节点和父节点交换，变成同左情况处理
					RBTNode<T>* temp = p;
					temp->left = temp;
					temp->key = cur->key;
					p->right = Nil;
					FLAG = 0;
				}
				if(FLAG==0){
					//新增节点是父节点的左孩子
					//同左，直接右转变色即可
					cur->parent->color = BLACK;
					cur->parent->parent->color = RED;
					right_rotate(cur->parent);
				}
			}else{
				//当前节点的父节点是祖父节点的右孩子
                                if(FLAG==0){
                                        //新增节点是父节点的左孩子
                                        //先将当前节点和父节点交换，变成同友情况处理
                                        RBTNode<T>* temp = p;
                                        temp->right = temp;
                                        temp->key = cur->key;
                                        p->left = Nil;
                                        FLAG = 1;
                                }
                                if(FLAG==1){
                                        //新增节点是父节点的右孩子
                                        //同右，直接左转变色即可
                                        cur->parent->color = BLACK;
                                        cur->parent->parent->color = RED;
                                        left_rotate(cur->parent);
                                }

			}
                }
		getRoot()->color = BLACK;
        }
};
template<typename T>
void RBTree<T>::delete_fix(){
};
template<typename T>
RBTNode<T>* RBTree<T>::find_replace_node(RBTNode<T>* node){
        return NULL;
};
template<typename T>
void print_tree(RBTNode<T>* node){
	if(node->key==-1)
		return ;
        cout<<node->key<<" "<<node->color<<endl;
        print_tree(node->left);
        print_tree(node->right);
};                                                                                                                                                    
template<typename T>
RBTNode<T>* RBTree<T>::getRoot(){
	return this->root;
};
