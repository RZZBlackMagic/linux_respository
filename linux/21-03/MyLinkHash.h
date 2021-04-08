#include<iostream>
#include<mutex>
using namespace std;

struct Entry{
	int key;
	int value;
	Entry* next;
};

class MyLinkHash{
	private:
		static MyLinkHash* hash;
		static Entry* hashEntry;
		static int size;
		static int p;
		static int num;
		MyLinkHash(){
			initHash();
		}
		~MyLinkHash(){
			free(hashEntry);
			hashEntry = NULL;
		}
		void initHash(){
			size = 8;
			p = 8;
			hashEntry = (Entry*)malloc(sizeof(Entry)*size);
			for(int i=0;i<size;i++){
				Entry* entry = (Entry*)malloc(sizeof(Entry));
				entry->key = -1;
				entry->value = -1;
				entry->next = NULL;
				hashEntry[i] = *entry;
			}
		}
		int count_link_node(){
		}

		int dealConflict(int index,int key,int value){
			Entry* postEntry = &hashEntry[index];
			while(postEntry->next!=NULL){
				postEntry = postEntry->next;
			}
			Entry* entry = (Entry*)malloc(sizeof(Entry));
			if(entry==NULL)
				return -1;
			entry->key = key;
			entry->value = value;
			entry->next = NULL;
			postEntry->next = entry;
			return index;
		}
		int resize(){
			int old_size = size;
			size = size*2;
			p = p*2;
			cout<<size<<" "<<p<<endl;
			Entry* newHashEntry = (Entry*)malloc(size*sizeof(Entry));
			if(newHashEntry==NULL)
				return -1;
			for(int i=0;i<size;i++){
				newHashEntry[i].key = -1;
				newHashEntry[i].value = -1;
				newHashEntry[i].next = NULL;
			}
			for(int i=0;i<old_size;i++){
				Entry* old_entry = &hashEntry[i];
				if(old_entry->key==-1)
					continue;
				while(old_entry!=NULL){
					Entry* new_entry = (Entry*)malloc(sizeof(Entry));
					if(new_entry==NULL)
						return -1;
					new_entry->key = old_entry->key;
					new_entry->value = old_entry->value;
					new_entry->next = NULL;
					int index = old_entry->key%p;
					if(newHashEntry[index].key==-1){
						newHashEntry[index].key = new_entry->key;
					       	newHashEntry[index].value = new_entry->value;
						newHashEntry[index].next = NULL;
					}else{
						Entry* temp = &newHashEntry[index];
						while(temp->next!=NULL){
							temp = temp->next;
						}
						temp->next = new_entry;
					}
					old_entry = old_entry->next;
				}
			}
			free(hashEntry);
			hashEntry = newHashEntry;
			return 0;
		}
	public:
		static MyLinkHash* getHash(){
			if(hash==NULL){
				return new MyLinkHash();
			}else{
				return hash;
			}
		}
		Entry& create_entry(int key,int value){
			
		}
		int add(int key,int value){
			if(num>0.8*size){
				//扩容
				cout<<"扩容前："<<endl;
				print_hash();
				cout<<"开始扩容"<<endl;
				cout<<"......................"<<endl;
				if(resize()==-1){
					cout<<"扩容失败"<<endl;
					return -1;
				}
			}
			int index = key%p;
			if(hashEntry[index].key!=-1){
				//发生冲突
				index = dealConflict(index,key,value);
			}else{
				hashEntry[index].key = key;
				hashEntry[index].value = value;
				hashEntry[index].next = NULL;
			}
			if(index!=-1){
				num++;
			}
			return index;
		}
		int find_by_key(int key){
			int index = key%p;
			Entry* entry = &hashEntry[index];
			while(entry!=NULL){
				if(key==entry->key){
					return entry->value;
				}
				entry = entry->next;
			}
			return -1;
		}
		int update_entry(int key,int value){
			int index = key%p;
			Entry* entry = &hashEntry[index];
			while(entry!=NULL){
				if(key == entry->key){
					entry->value = value;
					return index;
				}
				entry = entry->next;
			}
			return -1;
		}
		int delete_entry(int key,int value){
			int index = key%p;
			if(hashEntry[index].key==-1){
				cout<<"没有该元素"<<endl;
			}
			Entry* pre_entry = &hashEntry[index];
			Entry* entry = pre_entry->next;
			if(pre_entry->key==key&&pre_entry->value==value){
				if(entry==NULL){
					pre_entry->key = -1;
					pre_entry->value = -1;
				}else{
					pre_entry->key = entry->key;
					pre_entry->value = entry->value;
					pre_entry->next = entry->next;
					free(entry);
					entry = NULL;
				}
				return index;
			}
			while(entry!=NULL){
				if(key==entry->key&&value==entry->value){
					pre_entry->next = entry->next;
					free(entry);
					entry = NULL;
					break;
				}
				entry = entry->next;
			}
			
			if(index!=-1)
				num--;
			return index;
		}
		void print_hash(){
			Entry* next_entry = NULL;
			for(int i=0;i<size;i++){
				cout<<"下标"<<i<<":"<<endl;
				if(hashEntry[i].key!=-1){
					next_entry = &hashEntry[i];
					int num = 1;
					while(next_entry!=NULL){
			                        cout<<"    节点:"<<num++<<endl;
		                                cout<<"    key:"<<next_entry->key<<endl;
                		                cout<<"    value:"<<next_entry->value<<endl;
						cout<<endl;
						next_entry = next_entry->next;	
					}
				}else{
					cout<<"该下标还没有东西"<<endl;
				}
			}
		}

};
MyLinkHash* MyLinkHash::hash = NULL;
Entry* MyLinkHash::hashEntry = NULL;
int MyLinkHash::size = 0;
int MyLinkHash::p = 0;
int MyLinkHash::num = 0;
