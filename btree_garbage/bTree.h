#include<iostream>
using namespace std;
//const int M = 4;//节点的最大数量
template<typename T>
class b_tree_node{
public:
	int degree;//度：此节点的子节点个数
	int num;//key的个数，奇数
	T* keys;
	b_tree_node** childs;
	b_tree_node* parent;
	bool leaf;//是否为叶节点  false:不是   true:是
	b_tree_node(){};
	b_tree_node(int degree,int leaf){
		this->degree = degree;
		keys = new T[degree-1];//最多有degree-1个key
		childs = new b_tree_node*[degree];//最多有degree个子节点
		leaf = leaf;
		num = sizeof(keys)/sizeof(T);
	};
	~b_tree_node(){
		keys = NULL;
		childs = NULL;
		num = -1;
		degree = -1;
		leaf = false;
	}
};

template<typename T>
class b_tree{
	static int M ;
	b_tree_node<T>* root;
	b_tree_node<T>* Nil;
	//节点分裂,返回1：分裂成功，返回0：分裂失败
	static b_tree_node<T>* split_node(b_tree_node<T>*& root,b_tree_node<T>* node,int flag);
	//寻找插入位置
	static b_tree_node<T>* find_insert_node(b_tree_node<T>*& cur,b_tree_node<T>* curr,T x);
	//判断当前节点是否存满。满了返回1，未满就返回0
	static inline int check_node_state(b_tree_node<T>* cur);
public:
	b_tree();
	//通过key来查找
	b_tree_node<T>* find_node_by_key(T x);
	//添加
	int insert_node(T x);
	//删除
	int delete_node(T x);
	//打印
	static void print_tree(b_tree_node<T>* node);
	//得到root
	b_tree_node<T>* getRoot();
};
//得到root
template<typename T>
b_tree_node<T>* b_tree<T>::getRoot(){
	return root;
}
//b树构造函数
template<typename T>
b_tree<T>::b_tree(){
	root = NULL;
};
//判断当前节点的字节点是否存满
template<typename T>
inline int b_tree<T>::check_node_state(b_tree_node<T>* cur){
	//M阶b树，最多有M-1个key
	if(cur->degree==M){
//		cout<<"fen"<<endl;
		return 1;
	}
	return 0;
};
//节点分裂函数
//falg=1：cur指向分裂后的右边
//flag=0：cur指向分裂后的左边
//返回值：指向分裂后的插入位置
template<typename T>
b_tree_node<T>* b_tree<T>::split_node(b_tree_node<T>*& root,b_tree_node<T>* node,int flag){
	if(node==NULL)
		return NULL;
	//取此节点的中间节点（n/2向上取整）作为新节点，其他关键字形成两个字节点作为新节点的左右孩子,新节点加到父节点里面，递归处理副节点知道root
	int middle_key = node->num/2;//向上取整(向上取整要加1,但是下表从0开始再减1)
	b_tree_node<T>* middle_node = (b_tree_node<T>*)malloc(sizeof(b_tree_node<T>));
	b_tree_node<T>* left_node = (b_tree_node<T>*)malloc(sizeof(b_tree_node<T>));
	b_tree_node<T>* right_node = (b_tree_node<T>*)malloc(sizeof(b_tree_node<T>));
	//复制左右节点
	left_node->num = middle_key;
	left_node->degree = node->degree/2;
	left_node->leaf = node->leaf;
	left_node->parent = middle_node;
	left_node->keys = (T*)malloc(sizeof(T)*left_node->num);
	left_node->childs = (b_tree_node<T>**)malloc(sizeof(b_tree_node<T>*)*left_node->degree);
	for(int i=0;i<middle_key;i++){
		left_node->keys[i] = node->keys[i];
		left_node->childs[i] = node->childs[i];
	}
	left_node->childs[middle_key] = node->childs[middle_key];

	right_node->num = middle_key;
	right_node->degree = node->degree/2;
	right_node->leaf = node->leaf;
	right_node->parent = middle_node;
	right_node->keys = (T*)malloc(sizeof(T)*right_node->num);
	right_node->childs = (b_tree_node<T>**)malloc(sizeof(b_tree_node<T>*)*right_node->degree);
	for(int i=middle_key+1,j=0;i<node->num;i++,j++){
		right_node->keys[j++] = node->keys[i];
		right_node->childs[j++] = node->childs[i];
	}
	right_node->childs[right_node->degree-1] = node->childs[node->num];
	//从上到下检查，所以此节点的父节点都检查过了，一定是未满，直接插入父节点
	b_tree_node<T>* parent = node->parent;
	if(parent==NULL){
		//node是跟节点
//		cout<<"d"<<endl;
		middle_node->num = 1;
		middle_node->degree = 2;
		middle_node->leaf = false;
		middle_node->parent = NULL;
		middle_node->keys = (T*)malloc(sizeof(T));
		middle_node->childs = (b_tree_node<T>**)malloc(sizeof(b_tree_node<T>*)*2);
		middle_node->keys[0] = node->keys[middle_key];
		middle_node->childs[0] = left_node;
		middle_node->childs[1] = right_node;
		root = middle_node;
		free(node);
		if(flag == 0){
			return left_node;	
		}else{
			return right_node;
		}
	}
	//寻找加入位置
	int insert_index = 0;
	for(insert_index=0;insert_index<parent->num;insert_index++){
		if(node->keys[middle_key]>parent->keys[insert_index])
			continue;
		break;
	}
	//cout<<"insert_index:"<<insert_index<<endl;
	//在parent的insert_index插入
	b_tree_node<T>* new_parent = (b_tree_node<T>*)malloc(sizeof(b_tree_node<T>));
	new_parent->num = parent->num+1;
	new_parent->degree = parent->degree+1;
	new_parent->leaf = parent->leaf;
	new_parent->parent = parent->parent;
	new_parent->keys = (T*)malloc(sizeof(T)*new_parent->num);
	new_parent->childs = (b_tree_node<T>**)malloc(sizeof(b_tree_node<T>*)*new_parent->degree);
	bool f = false;
	for(int i=0;i<parent->num;i++){
		if(insert_index==i){
			new_parent->keys[i] = node->keys[middle_key];
			new_parent->childs[i] = left_node;
			new_parent->childs[i+1] = right_node;
			continue;
		}
		if(insert_index>i){
			new_parent->keys[i] = parent->keys[i-1];
			if(f==false){
				f = true;
				continue;
			}
			new_parent->childs[i] = parent->childs[i-1];
		}
		if(insert_index<i){
			new_parent->childs[i] = parent->childs[i];
			new_parent->keys[i] = parent->keys[i];
		}
	}
	//key比child少一个，将最后一个child复制进来
	if(insert_index==parent->num){
		new_parent->keys[insert_index] = node->keys[middle_key];
		new_parent->childs[insert_index] = left_node;  
		new_parent->childs[insert_index+1] = right_node;
	}else{
		new_parent->keys[new_parent->num-1] = parent->keys[parent->num-1];
		new_parent->childs[parent->num] = parent->childs[parent->degree-2];
		new_parent->childs[parent->num+1] = parent->childs[parent->degree-1];
	}
	//最后修改new_parent的父指针的该子指针
	if(parent->parent!=NULL){
		if(parent->parent->parent == NULL){
			//parent的父节点是root
			for(int i=0;i<parent->parent->degree;i++){
				if(parent->parent->childs[i] == parent){
					parent->parent->childs[i] = new_parent;
				
				}
			}
		}
	}else{
//		cout<<"aa"<<endl;
		root = new_parent;
	}
	left_node->parent = new_parent;
	right_node->parent = new_parent;
	free(parent);
	//插入完成后，在看父节点是否满，向上递归处理
	while(new_parent->degree == M){
               split_node(root,new_parent,0);
               if(new_parent->parent!=NULL){
                       new_parent = new_parent->parent;
               }else{
                       //到跟节点了
                       break;
               }
        }
	free(node);
	if(flag==1)
		return right_node;
	else
		return left_node;
};
//寻找插入位置
template<typename T>
b_tree_node<T>* b_tree<T>::find_insert_node(b_tree_node<T>*& root,b_tree_node<T>* cur,T x){
	//插入前，进行检查该节点的字节点是否满了
	//b_tree_node<T>* cur = root;
        int check_res = check_node_state(cur);
        if(check_res){
                //满了，进行分裂
		if(x>cur->keys[cur->num/2]){
			cur = split_node(root,cur,1);//右边
		}else
			///cout<<"d"<<endl;
			cur = split_node(root,cur,0);//左边
                if(cur==NULL){
                        //分裂失败
                        exit(0);
                }
        }
	if(cur->leaf){
		//在cur进行插入操作
		return cur;
	}
	if(x<cur->keys[0]){
		// 小于最小的，往左走
		return find_insert_node(root,cur->childs[0],x);
	}else if(x>cur->keys[cur->degree-2]){
		//大于最大的，往右走
		//cout<<"ddd"<<endl;
		return find_insert_node(root,cur->childs[cur->degree-1],x);
	}else{
		//在这个节点上
		for(int i = 1;i<cur->degree-1;i++){
			if(x>cur->keys[i])
				continue;
			if(x == cur->keys[i]){
				//已经有了
				cout<<"您要插入的数据已经有了"<<endl;
				exit(0);
			}
			if(x<cur->keys[i]){
				//在i-1到i之间，接着去下一层的节点，该节点在字节点的坐标为i
				return find_insert_node(root,cur->childs[i],x);
			}
		}	
	}
};
//通过key来查找
template<typename T>
b_tree_node<T>* b_tree<T>::find_node_by_key(T x){
	return NULL;
};

//添加
template<typename T>
int b_tree<T>::insert_node(T x){
	//寻找插入位置
	b_tree_node<T>* node = NULL;
	if(getRoot()!=NULL){
		//树不为空
		//cout<<"。"<<endl;
		node = find_insert_node(root,root,x);
		b_tree_node<T>* new_node = (b_tree_node<T>*)malloc(sizeof(b_tree_node<T>));
		new_node->num = node->num+1;
		new_node->degree = node->degree+1;
		new_node->leaf = node->leaf;
		new_node->parent = node->parent;
		new_node->keys = (T*)malloc(sizeof(T)*new_node->num);
		new_node->childs = (b_tree_node<T>**)malloc(sizeof(b_tree_node<T>*)*(new_node->degree));
		int flag = true;
		int insert_index = 0;
		for(int i=0;i<node->num;i++){
			if(node->keys[i]>x){
				insert_index = i;
				break;
			}
		}
		for(int i=0;i<node->num;i++){
			if(node->keys[i]!=0&&x>node->keys[i]){
			//	cout<<"i:"<<i<<" "<<node->keys[i]<<endl;
				new_node->keys[i] = node->keys[i];
			}else{
				if(flag){
					new_node->keys[i] = x;
					flag = false;
					continue;
				}
				new_node->keys[i] = node->keys[i-1];
			}
			new_node->childs[i] = NULL;
		}
		if(insert_index==node->num){
			new_node->keys[node->num] = x;
		}else{
			new_node->keys[node->num] = node->keys[node->num-1];
		}
		new_node->childs[new_node->degree-2] = NULL;
		new_node->childs[new_node->degree-1] = NULL;
		if(node->parent!=NULL){
			for(int i=0;i<node->parent->degree;i++){
				if(node==node->parent->childs[i]){
					node->parent->childs[i] = new_node;
					//cout<<"dd"<<endl;
					break;
				}
			}
		}else{
			root = new_node;
		}
		free(node);
		node=NULL;
	}else{
	//树为空
		node = (b_tree_node<T>*)malloc(sizeof(b_tree_node<T>));
		node->num = 1;
		node->degree = 2;
		node->leaf = true;
		node->parent = NULL;
		node->keys = (T*)malloc(sizeof(T)*node->num);
		(node->childs) = (b_tree_node<T>**)malloc(sizeof(b_tree_node<T>*)*node->degree);
		node->keys[0] = x;
		node->childs[0] = NULL;
		node->childs[1] = NULL;
		root = node;
	}
	return 1;
};

//删除
template<typename T>
int b_tree<T>::delete_node(T x){
	return 0;
};
template<typename T>
void b_tree<T>::print_tree(b_tree_node<T>* node){
	if(node==NULL)
		return;
	int t = node->degree;
	//cout<<"node_degree:"<<t<<endl;
	for(int i=0;i<node->degree;i++){
                print_tree(node->childs[i]);
        }
	cout<<"degree:"<<node->degree<<endl;
	for(int i=0;i<node->num;i++){
		cout<<node->keys[i]<<" ";
	}	
	cout<<endl;
}
template<typename T>
int b_tree<T>::M = 4;

