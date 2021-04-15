#pragma once
#include <assert.h>
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
private:
	Span* _head = nullptr;
};
//freelist:centralCache
class Freelist{
public:

private:
	void* ptr = nullptr;
	size_t _size;//建立链表中对象的个数
	size_t _max_size;//最大个数，超过这个个数就进行释放
};
