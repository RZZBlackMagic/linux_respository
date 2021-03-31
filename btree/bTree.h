#include<iostream>
using namepace std;
const int M = 4;//节点的最大数量
template<typename T>
class b_tree_node{
	int degree;//度：此节点的子节点个数
	int num;//key的个数，奇数
	T* keys;
	b_tree_node** childs;
	b_tree_node* parent;
	bool leaf;//是否为叶节点  false:不是   true:是
public:
	b_tree_node(){};
	b_tree_node(int degree,int leaf){
		this->degree = degree;
		keys = new T[degree-1];//最多有degree-1个key
		childs = new b_tree_node*[degree];//最多有degree个子节点
		leaf = leaf;
		num = sizeof(arr)/sizeof(T);
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
	b_tree_node<T>* root;
	b_tree_node<T>* Nil;
	b_tree(){};
	b_tree(int degree);
	//节点分裂,返回1：分裂成功，返回0：分裂失败
	int split_node(b_tree_node<T>* node);
	//寻找插入位置
	static b_tree_node<T>* find_insert_place(b_tree_node<T>* cur,T x);
	//判断当前节点是否存满。满了返回1，未满就返回0
	inline int check_node_state(b_tree_node<T>* cur);
public:
	//通过key来查找
	b_tree_node<T>* find_node_by_key(T x);
	//添加
	int insert_node(T x);
	//删除
	int delete_node(T x);
};

//b树构造函数
template<typename T>
b_tree<T>::b_tree(int degree){
	root = NULL;
};
//判断当前节点的字节点是否存满
inline int b_tree<T>::check_node_state(b_tree_node<T>* cur){
	//M阶b树，最多有M-1个key
	if(cur->num==M-1)
		return 1;
	return 0;
};
//节点分裂函数
template<typename T>
int b_tree<T>::split_node(b_tree_node<T>* node){
	//取此节点的中间节点（n/2向上取整）作为新节点，其他关键字形成两个字节点作为新节点的左右孩子,新节点加到父节点里面，递归处理副节点知道root
	int middle_arr = node->num/2;//向上取整(向上取整要加1,但是下表从0开始再减1)
	b_tree_node<T>* middle_node = (b_tree_node<T>*)malloc(sizeof(b_tree_node<T>));
	b_tree_node<T>* left_node = (b_tree_node<T>*)malloc(sizeof(b_tree_node<T>));
	b_tree_node<T>* right_node = (b_tree_node<T>*)malloc(sizeof(b_tree_node<T>));
	//复制左右节点
	left_node->num = middle_arr;
	left_node->degree = node->degree/2;
	left_node->leaf = cur->leaf;
	left_node->parent = middle_node;
	for(int i=0;i<middle_arr;i++){
		left_node->keys[i] = node->keys[i];
		left_node->childs[i] = node->childs[i];
	}
	left_node->childs[middle_arr] = node->childs[middle_arr];

	right_node->num = middle_arr;
	right_node->degree = node->degree/2;
	right_node->leaf = node->leaf;
	right_node->right = middle_parent;
	for(int i=middle_arr+1;i<node->num;i++){
		right_node->keys[i-middle_arr-1] = node->keys[i];
		right_node->childs[i-middle_arr-1] = node->childs[i];
	}
	right_node->childs[middle_arr] = node->childs[node->num];
	//在看父节点加一个是否存满，满了就去递归(满了注意处理left_node,right_node的parent)
	return 0;
};
//寻找插入位置
b_tree_node<T>* find_insert_place(b_tree_node<T>* cur,T x){
	//插入前，进行检查该节点的字节点是否满了
        int check_res = check_node_state(cur);
        if(check_res==1){
                //满了，进行分裂
                if(!split_node(cur->childs[check_res])){
                        //分裂失败
                        exit(0);
                }
        }
	b_tree_node<T>* res_node = NULL;
	if(cur->leaf){
		for(int i=0;i<cur->degree;i++){
			if(x>cur->arr[i]){
				continue;
			}
			break;
		}
		//插入下标i-1，进行插入操作
		
	}
	if(x<cur->arr[0]){
		// 小于最小的，往左走
		find_insert_node(cur->childs[0],x);
	}else if(x>cur->arr[cur->degree-2]){
		//大于最大的，往右走
		find_insert_node(cur->childs[cur->degree-1],x);
	}else{
		//在这个节点上
		for(int i = 1;i<cur->degree-1;i++){
			if(x>cur->arr[i])
				continue;
			if(x == cur->arr[i])
				//已经有了
				cout<<"您要插入的数据已经有了"<<endl;
			if(x<cur->arr[i]){
				//在i-1到i之间，接着去下一层的节点，该节点在字节点的坐标为i
				find_insert_node(cur->childs[i],x);
			}
		}	
	}
	return res_node;
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
	b_tree_node<T>* node = find_insert_node();
	return 0;
};

//删除
template<typename T>
int b_tree<T>::delete_node(T x){
	return 0;
};
