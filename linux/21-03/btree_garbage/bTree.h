#include<iostream>
#include<string.h>
using namespace std;
const int M = 4;//节点的最大数量
template<typename T>
class b_tree_node{
public:
	int degree;//度:所有节点的最大字节点数
	int num;//key的个数
	T* keys;
	b_tree_node** childs;
	b_tree_node* parent;
	bool leaf;//是否为叶节点  false:不是   true:是
	b_tree_node(){};
	b_tree_node(int degree,bool leaf1){
		this->degree = degree;
		keys = new T[degree-1];//最多有degree-1个key
		childs = new b_tree_node*[degree];//最多有degree个子节点
		memset(keys,-1,sizeof(keys));
		memset(childs,NULL,sizeof(childs));
		parent = NULL;
		this->leaf = leaf1;
		num = 0;
	};
	~b_tree_node(){
		//keys = NULL;
		//childs = NULL;
		num = -1;
		degree = -1;
		leaf = false;
	}
};

template<typename T>
class b_tree{
	b_tree_node<T>* root;
	b_tree_node<T>* Nil;
	//节点分裂,返回1：分裂成功，返回0：分裂失败
	static b_tree_node<T>* split_node(b_tree_node<T>**& root,b_tree_node<T>* node,int flag);
	//寻找插入位置
	static b_tree_node<T>* find_insert_node(b_tree_node<T>**& cur,b_tree_node<T>* curr,T x);
	//判断当前节点是否存满。满了返回1，未满就返回0
	static inline int check_node_state(b_tree_node<T>* cur,int i);
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
	b_tree_node<T>** getRoot();
};
//得到root
template<typename T>
b_tree_node<T>** b_tree<T>::getRoot(){
	//cout<<"this->root"<<&(this->root)<<endl;
	return &(this->root);
}
//b树构造函数
template<typename T>
b_tree<T>::b_tree(){
	root = NULL;
};
//判断当前节点的字节点是否存满
template<typename T>
inline int b_tree<T>::check_node_state(b_tree_node<T>* cur,int m){
	//M阶b树，最多有M-1个key
	int n = 0;
	for(int i=0;i<cur->num;i++){
		if(cur->keys[i] == -1){
			break;
		}
		n++;
	}
	if(n == m){
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
b_tree_node<T>* b_tree<T>::split_node(b_tree_node<T>**& root,b_tree_node<T>* node,int flag){
//	cout<<"cccc"<<root<<endl;
//	cout<<node->keys[node->num/2]<<endl;
	//找节点最中间的数据提到父节点中
	int middle_index = node->num/2;
//	cout<<node->keys[middle_index]<<endl;
	//处理左字节点
	b_tree_node<T>* left_node = new b_tree_node<T>(4,node->leaf);
	for(int i=0;i<middle_index;i++){
		left_node->keys[i] = node->keys[i];
		left_node->num++;
		left_node->childs[i] = node->childs[i];
	}
	left_node->childs[middle_index] = node->childs[middle_index];
//	cout<<"left_node:"<<left_node->num<<endl;	
	//处理右字节点
	b_tree_node<T>* right_node = new b_tree_node<T>(4,node->leaf);
	for(int i=middle_index+1;i<node->num;i++){
		right_node->keys[i-middle_index-1] = node->keys[i];
		right_node->num++;
		right_node->childs[i-middle_index-1] = node->childs[i];
	}
	right_node->childs[right_node->num] = node->childs[node->num];
//	cout<<"right_node:"<<node->num<<endl;
//	cout<<right_node->keys[0]<<endl;
//	cout<<right_node->childs[1]->keys[0]<<endl;
	//处理父节点
	b_tree_node<T>* new_parent = new b_tree_node<T>(4,false);
	b_tree_node<T>* parent = node->parent;
	if(parent==NULL){
		//node就是跟节点
	//	cout<<"aa"<<endl;
		*root = new_parent;
		new_parent->keys[0] = node->keys[middle_index];
		new_parent->childs[0] = left_node;
		new_parent->childs[1] = right_node;
//		if(node->keys[node->num-1]==110){
//		cout<<"???????"<<endl;
//		cout<<new_parent->keys[0]<<endl;
//		cout<<new_parent->childs[0]->keys[0]<<endl;
//		cout<<new_parent->childs[0]->keys[1]<<endl;
//		cout<<new_parent->childs[1]->keys[0]<<endl;
//		cout<<right_node->childs[1]->keys[0]<<endl;
//		}
		new_parent->num++;
	}else{
		bool flag1 = true;
		//处理keys
		for(int i=0;i<parent->num;i++){
			if(parent->keys[i] < node->keys[middle_index]){
//				cout<<"aaaaa"<<node->keys[middle_index]<<endl;
				new_parent->keys[i] = parent->keys[i];
				new_parent->num++;
				continue;
			}
			if(flag1){
				new_parent->keys[i] = node->keys[middle_index];
				new_parent->num++;
				flag1 = false;
				continue;
			}
			new_parent->keys[i] = parent->keys[i-1];
			new_parent->num++;
		}
		if(flag1){
//			cout<<"aaaaaaaaaaaaaaaaaa"<<node->num<<endl;
			//在最后进行添加
			new_parent->keys[parent->num] = node->keys[middle_index];
			new_parent->num++;
		}else{
			new_parent->keys[parent->num] = parent->keys[node->num-1];
			new_parent->num++;
		}
//		cout<<new_parent->keys[0]<<endl;
//              cout<<new_parent->keys[1]<<endl;
		//处理childs
		for(int i=0;i<parent->num+1;i++){
			if(new_parent->keys[i] < node->keys[middle_index]){
				new_parent->childs[i] = parent->childs[i];
				continue;
			}
			if(new_parent->keys[i] == node->keys[middle_index]){
				new_parent->childs[i] = left_node;
				new_parent->childs[i+1] = right_node;
				continue;
			}
			new_parent->childs[i+1] = parent->childs[i]; 
		}
		new_parent->parent = parent->parent;
		if(parent->parent!=NULL){
			for(int i=0;i<parent->parent->num;i++){
				if(parent->parent->childs[i] == parent){
//					cout<<"********"<<endl;
					parent->parent->childs[i] = new_parent;
					break;
				}
			}
		}else{
//			cout<<"aaa"<<endl;
//			cout<<*root<<endl;
			*root = new_parent;
//			cout<<*root<<endl;
//			cout<<(*root)->num<<endl;
//			cout<<(*root)->keys[0]<<endl;
//			cout<<(*root)->keys[1]<<endl;
//			cout<<node->keys[middle_index]<<endl;
		}
	}
	if(node->childs[0]!=NULL){
		for(int i=0;i<node->num+1;i++){
			if(i<left_node->num+1){
				left_node->childs[i] = node->childs[i];
				node->childs[i]->parent = left_node;
			}else{
				right_node->childs[i-left_node->num-1] = node->childs[i];
				node->childs[i]->parent = right_node;
			}
		}
	}
	new_parent->leaf = false;
	left_node->parent = new_parent;
	right_node->parent = new_parent;
//	cout<<"right parentm:"<<right_node->keys[0]<<endl;
	b_tree_node<T>* temp = new_parent;
	while(temp!=NULL){
		//递归
		if(check_node_state(temp,4)){
//			cout<<"要分裂的副节点"<<endl;
//			for(int i=0;i<temp->num;i++){
//				cout<<temp->keys[i]<<" ";
//			}
//			cout<<"::::"<<endl;
			split_node(root,temp,0);
		}
		temp = temp->parent;
	}
//	cout<<"x:"<<node->keys[0]<<endl;
//	 cout<<"right parentm:"<<right_node->keys[0]<<endl;
	if(flag==1){
//		cout<<"a"<<endl;
//		cout<<right_node->keys[0]<<endl;
		return right_node;
	}else if(flag==0){
		return left_node;
	}else{
		return NULL;
	}
};
//寻找插入位置
template<typename T>
b_tree_node<T>* b_tree<T>::find_insert_node(b_tree_node<T>**& root,b_tree_node<T>* cur,T x){
//	cout<<"&root:"<<root<<endl;
	//插入前，进行检查该节点的字节点是否满了
	//b_tree_node<T>* cur = root;
//	cout<<"x:"<<x<<cur->num<<endl;
//	cout<<"num"<<cur->num<<endl;
//	if(cur->leaf==false){
//		cout<<"false"<<endl;
//	}else
//		cout<<"true"<<endl;
//	cout<<"key:"<<cur->keys[0]<<endl;
	if(cur->leaf==true){
		//在cur进行插入操作
//		cout<<"x:"<<x<<endl;
//		cout<<cur->num<<endl;
//		cout<<cur->keys[0]<<endl;
		int check_res = check_node_state(cur,3);
//      	cout<<check_res<<endl;
	        if(check_res){
//              cout<<"aaaaaa"<<x<<endl;
                //满了，进行分裂
                	if(x>cur->keys[cur->num/2]){
//                      	cout<<"11111111111111111111"<<endl;
	//                      cout<<"x:"<<x;
				if(x==111){
			//		cout<<"x:"<<x<<endl;
			//		cout<<cur->keys[0]<<endl;
				}
         	               	cur = split_node(root,cur,1);//右边
//              	        cout<<cur->num<<endl;
               		 }else
                        	///cout<<"d"<<endl;
                        	  if(x==111){
                          //              cout<<"x:"<<x<<endl;
                            //            cout<<cur->keys[0]<<endl;
                                }

				 cur = split_node(root,cur,0);//左边
               		 if(cur==NULL){
                        	//分裂失败
                        	exit(0);
                	}
        	}
		if(x==115)
//			cout<<"cur-keys[0]:"<<cur->parent->keys[0]<<endl;
		return cur;
	}
	if(x<cur->keys[0]){
		// 小于最小的，往左走
		if(cur->childs[0]==NULL)
			return cur;
		else
			return find_insert_node(root,cur->childs[0],x);
	}else if(x>cur->keys[cur->num-1]){
		//大于最大的，往右走
		//cout<<"ddd"<<endl;
		if(cur->childs[cur->num]==NULL){
			return cur;
		}else
			return find_insert_node(root,cur->childs[cur->num],x);
	}else{
		//在这个节点上
//		cout<<"*******"<<endl;
		for(int i = 1;i<cur->num;i++){
			if(x>cur->keys[i])
				continue;
			if(x == cur->keys[i]){
				//已经有了
				cout<<"您要插入的数据已经有了"<<endl;
				exit(0);
			}
			if(x<cur->keys[i]){
	 			//在i-1到i之间，接着去下一层的节点，该节点在字节点的坐标为i
				if(cur->childs[i]==NULL)
					return cur;
				else
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
	b_tree_node<T>** root_temp = getRoot();
	b_tree_node<T>* node = NULL;
//	cout<<"&root_temp:"<<(root_temp)<<endl;
	if(*root_temp!=NULL){
		//树不为空
		b_tree_node<T>* insert_place_node = find_insert_node(root_temp,*root_temp,x);
//		if(x==115)
//			cout<<"bbbb:"<<insert_place_node->parent->keys[0]<<endl;
		//此时插入的节点一定在叶节点
		b_tree_node<T>* new_node = new b_tree_node<T>(4,insert_place_node->leaf);
//		cout<<"****************************"<<&new_node<<endl;
		//print_tree(this->root);i
//		cout<<"******************************"<<endl;
		bool flag1 = true;
                //处理keys
                for(int i=0;i<insert_place_node->num;i++){
//			cout<<"i:"<<i<<" key:"<<insert_place_node->keys[i]<<endl;
                        if(insert_place_node->keys[i] < x){
                                new_node->keys[i] = insert_place_node->keys[i];
                                new_node->num++;
                                continue;
                        }
                        if(flag1){
			//	cout<<"i:"<<i<<x<<endl;
                                new_node->keys[i] = x;
  				new_node->num++;
  				flag1 = false;
                                continue;
                        }
                        new_node->keys[i] = insert_place_node->keys[i-1];
                        new_node->num++;
                }
                if(flag1){
                        //在最后进行添加
                        new_node->keys[insert_place_node->num] = x;
                        new_node->num++;
                }else{
//			cout<<"aaaaaa:wq"<<insert_place_node->keys[insert_place_node->num-1]<<endl;
                        new_node->keys[insert_place_node->num] = insert_place_node->keys[insert_place_node->num-1];
  //  			cout<<"aaaaaa:wq"<<insert_place_node->num<<endl;
			new_node->num++;
//			cout<<new_node->num<<endl;
                }
                //处理childs
                for(int i=0;i<new_node->num+1;i++){
  			new_node->childs[i] = NULL;
                }
		new_node->parent = insert_place_node->parent;
		if(insert_place_node->parent==NULL){
//			cout<<"x:"<<x<<endl;
			this->root = new_node;
		}else{
			for(int i=0;i<new_node->parent->num+1;i++){
				if(insert_place_node->parent->childs[i]->keys[0]==insert_place_node->keys[0]){
//					cout<<"77"<<endl;
//					cout<<insert_place_node->parent->childs[i]->keys[1]<<endl;
//					cout<<new_node->keys[0]<<endl;
					new_node->parent->childs[i] = new_node;
//					cout<<new_node->parent->childs[i]->keys[0]<<endl;
//					cout<<"hhh:"<<insert_place_node->parent->keys[0]<<endl;
					break;
				}
			}
		}
//		cout<<"root:"<<getRoot()->keys[0]<<endl;
		if(x==7){
			//cout<<"root nun:"<<getRoot()->num<<endl;
		}
//		insert_place_node->degree++;
		//插入之后再去判断是否存满
//		if(check_node_state(new_node)){
			//满了，分裂
			//split_node(root,new_node,0);	
//		}
	}else{
	//树为空
//		cout<<"aaa"<<endl;
		b_tree_node<T>* node = new b_tree_node<T>(4,true);
		node->keys[0] = x;
		node->num++;
		*root_temp = node;
	}
//	print_tree(this->root);
	return 1;
};

//删除
template<typename T>
int b_tree<T>::delete_node(T x){
	return 0;
};

template<typename T>
void b_tree<T>::print_tree(b_tree_node<T>* node){
	if(node==NULL){
	//	cout<<"当前树为空"<<endl;
		return;
	}
	//int t = node->degree;
	//cout<<"node_degree:"<<t<<endl;
	for(int i=0;i<node->num+1;i++){
//		cout<<"a"<<endl;
                print_tree(node->childs[i]);
        }
//	cout<<"num:"<<node->num<<endl;
	if(node->leaf==1)
		cout<<"true"<<endl;
	else
		cout<<"false"<<endl;
	for(int i=0;i<node->num;i++){
	//	cout<<"qqq"<<node->num<<endl;
		cout<<node->keys[i]<<" ";
	}	
	cout<<endl;
}

