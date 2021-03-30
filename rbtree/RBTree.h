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
		void delete_fix(RBTNode<T>* node);
		//寻找前驱节点用来替代
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
		int delete_node(T x);
		//销毁树
		int destory_tree(RBTNode<T>* root);
		//通过key寻找节点		
		RBTNode<T>* find_node_by_key(T x);
		//打印
		//void print_tree();
};
template<typename T>
RBTNode<T>* RBTree<T>::find_uncle_by_parent(RBTNode<T>* parent){
	if(parent == parent->parent->left){
		//父节点是祖父节点的左孩子
		return parent->parent->right;
	}else if(parent == parent->parent->right){
		return parent->parent->left;
	}else{
		cout<<"找叔叔出错了"<<endl;
		exit(0);
	}
};
template<typename T>
RBTNode<T>* RBTree<T>::find_insert_place(T x){
	RBTNode<T>* node = getRoot();
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
	}
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
//黑红树的删除对应到2,3,4树中，真实删除的节点在2,3,4树中一定是叶子节点，所以有三种情况：2节点，3节点，4节点
//2节点：里面有一个节点，一定是黑色
//3节点：里面有两个节点，下面红，上面黑，如果删除黑，删除后用红代替再变黑，如果删除红，直接删
//4节点：里面有三个节点，下面两个红，上面黑，如果删除红，直接删，黑色他有两个孩子，不可能删除黑
//情况1：3节点和4节点
//情况2：2节点，黑色的，删除的话要找兄弟（这里的兄弟是2,3,4树中的兄弟哦，不是红黑书中的兄弟：）借一个，兄弟有的借，分两种情况，兄弟是3节点或者4节点
//	2.1：兄弟是3节点：兄弟只有一个孩子（这里的兄弟是2,3,4树中的兄弟，不是红黑树中的兄第，先在红黑树中找到其兄弟），父亲下去替代删除节点，兄弟上来替代父节点（但是要保证顺序，有可能在父亲下去之前旋转来保证顺序,尊转过程中注意变色)
//	2.2：兄弟节点是4节点，以当前节点的父节点旋转一次即可
//情况3：2节点，黑色的，删除的话要找兄弟借一个，但是兄弟没有子节点，没得借
//	3.1：如果兄弟为黑，父亲为红，将兄弟变红，父亲变黑即可
//	3.2：如果兄弟为黑，父亲为黑，将兄弟变红，但是可以找到一个不平衡的节点，该节点的一个子树刚才删了一个黑色，让此节点的另一个子树自损一个黑色，这个节点的左右子树就平衡了，但是往上他的父节点又不平衡了重复上面操作，递归直到跟节点结束
//
int RBTree<T>::delete_node(T x){
	RBTNode<T>* cur = find_node_by_key(x);
	if(cur->left!=Nil&&cur->right!=Nil){
                //左右字节点都存在，撒谎拿出后调整
                RBTNode<T>* real_delete_node = find_replace_node(cur);
                cur->key = real_delete_node->key;
                cur = real_delete_node;
	}
	RBTNode<T>* son_node = (cur->left!=Nil) ? cur->left:cur->right;//真正删除的节点的孩子节点（也就是真正删除了的节点的替代节点）
	if(son_node==Nil){
		//无字节点，直接删除（对应删除2,3,4树的3节点的红色和4节点的红色，不用调整）
		if(update_flag(cur)==-1){
			root = Nil;
		}
		if(cur->color==BLACK)
			delete_fix(cur);
		//只用修改cur父节点往下的指针，son_node为Nil，其往上的指针不能改
		if(FLAG==0){
        	        cur->parent->left = son_node;
	        }else{
	                cur->parent->right = son_node;
	        }
	        cur->parent = Nil;
	}else{
		//左右字节点只有一个存在(son_node)，用唯一的字节点替换就可以了
		if(update_flag(cur)==-1){
			root = son_node;
		}
		son_node->parent = cur->parent;//son_node不为Nil，所以要修改其复指针
		if(FLAG==0){
        	        cur->parent->left = son_node;
	        }else{
        	        cur->parent->right = son_node;
	        }
		cur->parent = Nil;
		if(cur->color==BLACK){
			//破坏了平衡去调整
			delete_fix(son_node);
		}
	}
        free(cur);
	return 0;
};
template<typename T>
void RBTree<T>::left_rotate(RBTNode<T>* cur){
	RBTNode<T>* p =  cur->parent;
		//以b为支点旋转
		//    a
		//     \
		//      b
		//       \
		//        c
		cur->parent = cur->parent->parent;

		if(p->parent==Nil){
			//当前节点的父节点是跟节点
			root = cur;
		}else{
			if(p->parent->left==p)
				p->parent->left = cur;
			else if(p->parent->right == p)
				p->parent->right = cur;
			else 
				cout<<"左旋出现问题,树有问题"<<endl;
		}
		RBTNode<T>* temp = cur->left;
		cur->left = p;
        	p->parent = cur;
       	        p->right = temp;
		temp->parent = p;
		//以b为支点旋转
		//      a
		//       \
		//        b
		//       / \
		//      c   d

};
template<typename T>
void RBTree<T>::right_rotate(RBTNode<T>* cur){
	RBTNode<T>* p = cur->parent;
	if(p->parent == Nil){
		root = cur;
	}else{
		if(p->parent->left == p){
			p->parent->left = cur;
		}else if(p->parent->right == p){
			p->parent->right = cur;
		}else{
			cout<<"右旋出错"<<endl;
		}
	}
	cur->parent = p->parent;
	RBTNode<T>* temp = cur->right;
	cur->right = p;
	p->parent = cur;
	p->left = temp;
	temp->parent = p;
};
template<typename T>
void RBTree<T>::insert_fix(RBTNode<T>* cur){
	RBTNode<T>* p = cur->parent;
        while(p->color==RED&&p->parent!=Nil){
		//更新FLAG
		update_flag(cur);
                RBTNode<T>* uncle = find_uncle_by_parent(p);
                if(uncle->color==RED){
			//将父节点和祖父节点变色，往上递归处理
				cur->parent->color = BLACK;
				find_uncle_by_parent(cur->parent)->color = BLACK;
				cur->parent->parent->color = RED;
				cur = cur->parent->parent;
				p = cur->parent;	
	                       	continue;
                }
                if(uncle->key==-1||uncle->color==BLACK){
			cout<<"a"<<endl;
			if(cur->parent->parent->left==cur->parent){
				//当前节点的父节点是祖父节点的左孩子
				if(FLAG==1){
					//新增节点是父节点的右孩子
					//先将当前节点和父节点交换，变成同左情况处理
					cur->parent = p->parent;
					p->parent->left = cur;
					cur->left = p;
					p->parent = cur;
					p->right = Nil;
					cur = p;
					FLAG = 0;
				}
				if(FLAG==0){
					//新增节点是父节点的左孩子
					//同左，直接右转变色即可
					cur->parent->color = BLACK;
					cur->parent->parent->color = RED;
					right_rotate(cur->parent);
					break;
				}
			}else{
				//当前节点的父节点是祖父节点的右孩子
				if(FLAG==0){
                                        //新增节点是父节点的左孩子
                                        //先将当前节点和父节点交换，变成同友情况处理
                                        p->parent->right = cur;
					cur->parent = p->parent;
					cur->right = p;
					p->parent = cur;
                                        p->left = Nil;
					cur = p;
                                        FLAG = 1;
                                }
				if(FLAG==1){
                                        //新增节点是父节点的右孩子
                                        //同右，直接左转变色即可
					cur->parent->color = BLACK;
                                        cur->parent->parent->color = RED;
                                        left_rotate(cur->parent);
					break;
					//旋转完成后，cur->parent变为旋转后的子树的跟节点，为黑色，所以cur->parent->parent不管是红的黑的都可以，所以不用在往上面递归了
                                }

			}
                }
        }
	getRoot()->color = BLACK;
};
//黑红树的删除对应到2,3,4树中，真实删除的节点在2,3,4树中一定是叶子节点，所以有三种情况：2节点，3节点，4节点
//2节点：里面有一个节点，一定是黑色
//3节点：里面有两个节点，下面红，上面黑，如果删除黑，删除后用红代替再变黑，如果删除红，直接删
//4节点：里面有三个节点，下面两个红，上面黑，如果删除红，直接删，黑色他有两个孩子，不可能删除黑
//情况1：3节点和4节点
//情况2：2节点，黑色的，删除的话要找兄弟（这里的兄弟是2,3,4树中的兄弟哦，不是红黑书中的兄弟：）借一个，兄弟有的借，分两种情况，兄弟是3节点或者4节点
//      2.1：兄弟是3节点：兄弟只有一个孩子（这里的兄弟是2,3,4树中的兄弟，不是红黑树中的兄第，先在红黑树中找到其兄弟），父亲下去替代删除节点，兄弟上来替代父节点（但是要保证顺序，有可能在父亲下去之前旋转来>保证顺序,尊转过程中注意变色) 如果被删节点的父节点为黑(不存在)，如果被删节点的父节点为红，旋转之后，父节点变黑，字节点变红
//      2.2：兄弟节点是4节点，以当前节点的父节点旋转一次即可（两种方案，1像2.1那样旋转两次，2也可以旋转一次）
//情况3：2节点，黑色的，删除的话要找兄弟借一个，但是兄弟没有子节点，没得借
//      3.1：如果兄弟为黑，父亲为红，将兄弟变红，父亲变黑即可
//      3.2：如果兄弟为黑，父亲为黑，将兄弟变红，但是可以找到一个不平衡的节点，该节点的一个子树刚才删了一个黑色，让此节点的另一个子树自损一个黑色，这个节点的左右子树就平衡了，但是往上他的父节点又不平衡了>重复上面操作，递归直到跟节点结束
template<typename T>
void RBTree<T>::delete_fix(RBTNode<T>* cur){
	if(cur!=Nil&&cur->color==BLACK){
		//这种情况对应删除无字节点的情况（情况2,3），如果是黑，破坏了平衡，处理（cur指向的是删除节点）
		if(cur->parent->left==cur){
			//当前节点是父节点的左孩子
		//情况2
		//先去找真实的兄弟节点，如果红黑树中，被删节点的兄弟节点是红色的，此时在2,3,4树中，就不是被删节点的兄弟节点，需要去找（旋转一次即可），如果红黑书中被删节点的兄弟节点是黑色的，那么在2,3,4树中他也是被删节点的兄弟节点,不用动(不可能没有兄弟的，这样就不黑色平衡了)
			RBTNode<T>* bro = cur->parent->right;
                        if(bro->color==RED){
                                //只有这种情况不是真兄弟
                                cur->parent->color = RED;
                                cur->parent->right->color = BLACK;
                                right_rotate(cur->parent->right);//旋转完成后，当前节点的父节点一定是红，兄弟节点一定是黑
                                bro = cur->parent->right;
                        }
			if(bro->left==Nil&&bro->right==Nil){
				//如果兄弟节点是2节点，
				//情况3
				if(cur->parent->color == RED){
                                        //情况3.1,父亲为红
					cur->parent->color = BLACK;
					cur->parent->right->color = RED;
                                }else{
                                        //情况3,2,父亲为黑
					RBTNode<T>* node = cur->parent;
					while(node!=Nil){
						node->right->color = RED;
						node = node->parent;
					}
                                }

			}else if(bro->left!=Nil&&bro->right!=Nil){
				//如果兄弟节点是4节点
				//情况2.2
				bro->color =cur->parent->color;
				cur->parent->color = BLACK;
				bro->right->color = BLACK;
				left_rotate(bro);
				cur = Nil;

			}else{
				//如果兄弟节点是3节点，没有字节点
				//情况2.1
				if(bro->left!=Nil){
					//先右旋
					bro->left->color = BLACK;
					bro->color = RED;
					right_rotate(bro->left);
					bro = cur->parent->right;
				}
				bro->color =cur->parent->color;
                                cur->parent->color = BLACK;
                                bro->right->color = BLACK;
                                left_rotate(bro);
                                cur = Nil;
			}
		}else{ 
			//当前节点是父节点的右孩子

			RBTNode<T>* bro = cur->parent->left;
                        if(bro->color==RED){
                                //只有这种情况不是真兄弟
                                cur->parent->color = RED;
                                cur->parent->left->color = BLACK;
                                left_rotate(cur->parent->left);//旋转完成后，当前节点的父节点一定是红，兄弟节点一定是黑
                                bro = cur->parent->left;
                        }
                        if(bro->left==Nil&&bro->right==Nil){
                                //如果兄弟节点是2节点，
                                //情况3
                                if(cur->parent->color == RED){
                                        //情况3.1,父亲为红
                                        cur->parent->color = BLACK;
                                        cur->parent->left->color = RED;
                                }else{
                                        //情况3,2,父亲为黑
                                        RBTNode<T>* node = cur->parent;
                                        while(node!=Nil){
                                                node->left->color = RED;
                                                node = node->parent;
                                        }
                                }

                        }else if(bro->left!=Nil&&bro->right!=Nil){
                                //如果兄弟节点是4节点
                                //情况2.2
                                bro->color =cur->parent->color;
                                cur->parent->color = BLACK;
                                bro->left->color = BLACK;
                                right_rotate(bro);
                                cur = Nil;

                        }else{
                                //如果兄弟节点是3节点，没有字节点
                                //情况2.1
                                if(bro->right!=Nil){
                                        //先左旋
                                        bro->right->color = BLACK;
                                        bro->color = RED;
                                        left_rotate(bro->right);
                                        bro = cur->parent->left;
                                }
                                bro->color =cur->parent->color;
                                cur->parent->color = BLACK;
                                bro->left->color = BLACK;
                                right_rotate(bro);
                                cur = Nil;
                        }
		}
	}else{
		//这种情况对应删除有一个字节点的（情况1），替代节点为红，删除节点为黑，替代过后，将替代节点变黑，保持黑色平衡（cur指向的是替代节点）
		cur->color = BLACK;
	}
};
template<typename T>
RBTNode<T>* RBTree<T>::find_node_by_key(T x){
	RBTNode<T>* root = getRoot();
	while(root!=Nil){
		if(root->key>x)
			root = root->left;
		else if(root->key<x)
			root = root->right;
		else 
			return root;
	}
	if(root==Nil){
		cout<<"find_node_by_key:树中没有该元素"<<endl;
		exit(0);
	}
        return root;
};
template<typename T>
RBTNode<T>* RBTree<T>::find_replace_node(RBTNode<T>* node){
	if(node->left==Nil){
		cout<<"检查该元素的前驱问题"<<endl;
		exit(0);
	}
	node = node->left;
	while(node->right!=Nil){
		node = node->right;
	}
	return node;
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
template<typename T>
inline int update_flag(RBTNode<T>* node){
	if(node->key==-1){
		cout<<"当前节点为Nil，更新flag失败"<<endl;
		return -2;
	}
	if(node->parent->key==-1){
		cout<<"当前节点为跟节点，更新Nil失败"<<endl;
		return -1;
	}
	if(node->parent->left == node){
		FLAG = 0;
	}else if(node->parent->right == node){
		FLAG = 1;
	}else{
		cout<<"更新flag出现问题，树有问题"<<endl;
	}
	return 0;
}
