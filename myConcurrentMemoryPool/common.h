#pragma once
#include <assert.h>
#include <iostream>
//using namespace std;
//（pageCache中）129是pagelist数组的大小，pagelist[1]里面放的就是1页大小的span的链表，pagelist[2]里面放的就是2页大小的span链表.....(0不放)
const int NPAGES = 129;
//（centralCache中）240是spanlist数组的大小
const int NLISTS = 240;
//const int PAGELIST_SIZE = 16;
typedef  size_t PageID;
struct Span {
	PageID _page_id = 0;//一个span中有好多页，记录指向该span的指针所在的页的页号
	size_t _npage = 0;//该span中有几页（1页=4k）
	//span为双向循环链表
	Span* _prev = nullptr;
	Span* _next = nullptr;
	//对象自由链表
	void* _objlist = nullptr;
	size_t _obj_size = 0;//对象内存块大小
	size_t _use_count = 0;//对象内存块实用计数
};
//双向循环链表(pageCache)
class Spanlist {
public:
	Spanlist() 
	{
		_head = new Span;
		_head->_next = _head;
		_head->_prev = _head;
	};
	//在cur后面插入new_node
	void insert(Span* cur, Span* new_node)
	{
		assert(cur||new_node);
		Span* temp = cur->_next;
		cur->_next = new_node;
		new_node->_prev = cur;
		new_node->_next = temp;
		temp->_prev = new_node;
	};
	//删除cur
	void earse(Span* cur)
	{
		assert(cur || cur->_next != _head);
		cur->_prev->_next = cur->_next;
		cur->_next->_prev = cur->_prev;

	};
	Span* pop_front()
	{
		Span* Begin = begin();
		earse(Begin);
	};
	Span* pop_back(){
		Span* End = end();
		earse(End);
	}
	void push_back(Span* new_node)
	{
		Span* cur = _head->_prev;
		insert(cur, new_node);
	};
	void push_front(Span* new_node) 
	{
		insert(_head, new_node);
	};
	Span* begin()
	{
		return _head->_next;
	};
	Span* end()
	{
		return _head->_prev;
	};
	bool empty() {
		_head->_next = _head;
		_head->_prev = _head;
	};
	bool is_empty(){
		return (_head->_next==_head&&_head->_prev==_head)?true:false;
	}
private:
	Span* _head = nullptr;
};

inline void*& NEXT_OBJ(void* ptr)
{//ptr=(void**)ptr,返回*ptr
//	void* temp = (void*)ptr;
//	return temp;
	return *((void**)ptr);
};

//freelist:centralCache
class Freelist{
public:
	size_t size(){
		return _size;
	}
	size_t max_size(){
		return 10;
	}
	void* clear(){
		void* start = _ptr;
		_ptr = nullptr;
		_size = 0;
		return start;
	}
	bool is_empty(){
		return _ptr==nullptr;
	}
	//头插，每个obj里面放的都是后面一个obj的地址（NEXT_OBJ(obj)=ptr）,ptr是头指针，指向地一个obj
	void push(void* obj){
//		int a = 0;
//		_ptr = (void*)&a;
//		_size = 0;
//		std::cout<<"origin Obj:"<<obj<<std::endl;
//		std::cout<<"origin ptr:"<<_ptr<<std::endl;
//		_ptr = NEXT_OBJ(obj);
//		_ptr = obj;
//		std::cout<<NEXT_OBJ(obj)<<std::endl;
		//std::cout<<*(int*)_ptr<<std::endl;
		NEXT_OBJ(obj) = _ptr;//此时ptr是未插入时的第一个obj。插入后obj成第一个，让obj的内容为ptr
		_ptr = obj;//插入后，当前obj成第一个
		++_size;		
	}	
	void push_range(void* start,void* end,size_t num){
		NEXT_OBJ(end) = _ptr;
		_ptr = start;
		_size+=num;
	}
	void* pop(){
		void* obj = (_ptr);
		_ptr = NEXT_OBJ(obj);
		_size--;
		return obj;
	}
private:
	void* _ptr=nullptr ;
	size_t _size;//建立链表中对象的个数
	//size_t _max_size;//最大个数，超过这个个数就进行释放
};

class classSize{
public:
	static inline size_t _round_up(size_t size,size_t align){
		
		if(size%align){
//			std::cout<<(size/align+1)*8<<std::endl;
			return (size/align+1)*align;
		}else{
			return (size/align)*align;                                                          
		}
		
//		return ((size + align - 1) & ~(align - 1));
	}
	static inline size_t round_up(size_t size){
		if(size<128){
			return _round_up(size,8);
		}else if(size<1024){
			return _round_up(size,16);
		}else if(size<8*1024){
			return _round_up(size,128);
		}else if(size<64*1024){
			return _round_up(size,512);
		}
		std::cout<<"内存对其出错，内存块太大了"<<std::endl;
		return -1;
	}
	static inline size_t _index(size_t size,size_t align){
		//return ((size + (1<<align) - 1) >> align) - 1;
		return _round_up(size,align)/align-1;
	}
	static inline size_t index(size_t size){
		//每个区间有多少链
		size_t group_array[4]={16,56,56,112};
		if(size<128){
			return _index(size,8);
		}else if(size<1024){
			return _index(size-128,16)+group_array[0];
		}else if(size<8*1024){
			return _index(size-1024,128)+group_array[0]+group_array[1];
		}else if(size<64*1024){
			return _index(size-8*1024,512)+group_array[0]+group_array[1]+group_array[2];
		}
		std::cout<<"计算freelist下标出错"<<std::endl;
		return -1;
	}
};
