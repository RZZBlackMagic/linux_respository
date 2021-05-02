#include "centralCache.h"
centralCache* centralCache::_Inst = nullptr;

//centralCache* central = centralCache::getInstance();
//给threadCache分配一定数量的对象
//获取一个span，切分为num个大小为byte的块，返回给threadCache，剩下继续放在centralCache
size_t centralCache::provideRangeObjToThreadCache(void*& start,void*& end,size_t n,size_t bytes){
	std::cout<<"centralcache::provideRangeObjToThreadCache->getOneSpan"<<std::endl;
	//根据bytes计算出要在spanlist数组中获取的index(先对齐)
	
	//size_t byte;
	Spanlist* list = nullptr;
	//根据获取到的list和byte去调用getOneSpan获取span
	Span* new_span = getOneSpan(list,bytes);
	//在span中分割出num个byte大小的块后
	void* obj = new_span->_objlist;
	start = obj;
	size_t p_num = 0;
	void* prev = nullptr;
	while(obj!=nullptr&&p_num<=n){
		prev = obj;
		obj = NEXT_OBJ(obj);
		p_num++;
	}
	end = prev;
	//将剩下的i继续放在centralCache，取出来的返回
	new_span->objlist = NEXT_OBJ(prev);
	new_span->use_count+=p_num;
	NEXT_OBJ(prev) = nullptr;
	return p_num;
};

//获取一个span，返回，不够就去pageCache要
Span* centralCache::getOneSpan(Spanlist* list,size_t _size){
	std::cout<<"centralcache::getOneSpan->AllocmemFormCentralCache"<<std::endl;
	//在spanlist中找到了，直接返回
	if(list->is_empty()){		
	//没有找到，就去pageCache中获取新的span
		size_t npage = classSize::numToPage(_size);//计算需要多少页
		Span* span = pageObj->AllocMemForCentralCache(npage);
		//设置span的objlist:就是通过NEXT_OBJ建立映射，构造成链表形状
		char* start = (char*)(span->_page_id*4*1024);
		char* end = start+span->_npage*4*1024;
		char* cur = start;
		char* next = start+_size;
		while(next<end){
			NEXT_OBJ(cur) = next;
		       	cur = next;
			next += _size;
			count++;
		}
		span->_objlist = start;
		span->_use_count = 0;
		span->_obj_size = _size;
		//将新的span挂在spanlist上
		list.push_front(span);	
		return span;
	}
	Span* span = list.begin();
	while(span!=list.end()){
		if(span->_objlist!=nullptr){
			return span;//找一个不空的span返回
		}
		span = span->_next;
	}	
	list.pop_front();
	return span;
};

//释放一定数量的对象到page
//以span为单位返回给pageCache
void centralCache::releaseListToSpan(void* start,size_t byte_size){

};
