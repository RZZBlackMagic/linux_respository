#include<iostream>
#include<malloc.h>
using namespace std;
struct Entry{
	int key;
	int value;
	Node* next;
};

struct Link{
	Entry* head;
};
class MyHash{
        private:
                static MyHash* hash;
                static Entry* hashArr;
                static int p ;
		static int size;
                MyHash(){
//			size = 10;
  			initHash();
                        hashArr = (int*)malloc(size*sizeof(int));
                        memset(hashArr, -1, size * sizeof(int));
		//	for(int i=0;i<size;i++){
		//		Entry* entry = (Entry*)malloc(sizeof(Entry));
		//		entry.key = i;
		//		entry.value = -1;
		//		entry.next = NULL;
		//		hashArr[i] = entry;
		//	}
                }
                ~MyHash(){
                        free(hashArr);
                }
                int getSizeOfArr(){
                        size = malloc_usable_size(hashArr)/sizeof(*hashArr);
                	return size;
		}
                void initHash(){
			size = 10;
                        //getSizeOfArr();
			p = size;//小于size的最小素数
                        if(size<2){
                                p = 1;
                        }
                        while(p>2){
		//		cout<<"p:"<<p<<endl;
                                int temp = p--;
                                int i = p;
                                while(temp%i!=0){
                                        i--;
                                }
                                if(i==1){
                                        p = temp;
                                        break;
                                }
                        }
                }
                int dealConflict(int key){
                        int index = key%p;
			int temp = index;
                        while(hashArr[index]!=-1){
				if(index==size-1){
					index = 0;
					continue;
				}
				index++;
				if(index==temp){
					cout<<"数组满了"<<endl;
					return -1;
				}
			}
                        return index;
                }
		
		//统计数组内元素个数
		int count_num(){
			int num = 0;
			for(int i=0;i<size;i++){
				if(hashArr[i]!=-1)
					num++;
			}
			return num;
		}
		void expand_arr(){
			int old_size = size;
			size = (size)*2;
			hashArr = (int*)realloc(hashArr,sizeof(int)*size);
			memset(hashArr+old_size,-1,size*sizeof(int));
			int s = getSizeOfArr();
			cout<<"扩容后size："<<size<<endl;
		}
        public:
                static MyHash* getMyHash(){
                        if(hash==NULL)
                                return new MyHash();
                        else
                                return hash;
                }
                int add(int key){
			int num = count_num();
			float s = 0.7000*(float)size;
			cout<<"s:"<<s<<endl;
			if(num>=(0.7*size)){
				//扩容
				expand_arr();
			}
                        int index = key%p;
                        if(hashArr[index]!=-1){
                                index = dealConflict(key);
                        }
			if(index==-1){
				cout<<"添加失败"<<endl;
				return index;
			}
                        hashArr[index] = key;
                        return index;
                }
                int deleteElement(int key){
			int index = key%p;
			int temp = index;
			while(hashArr[index]!=key)
			{
				if(index==9){
					index = 0;
					continue;
				}
				index++;
				if(index = temp){
					cout<<"没有找到该元素"<<endl;
					return -1;
				}
			}
			hashArr[index] = -1;
                        return index;
                }
		void print_arr(){
			cout<<size<<endl;
			for(int i=0;i<size;i++){
				cout<<hashArr[i]<<endl;
			}
		}
};
MyHash* MyHash::hash = NULL;
int* MyHash::hashArr = NULL;
int MyHash::p = 0;
int MyHash::size = 0;
