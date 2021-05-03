#include "centralCache.h"
centralCache* centralCache::_Inst = nullptr;
void centralCache::printCentralCache(){
	std::cout<<"#####################"<<std::endl;
	std::cout<<"centralCache的span情况:"<<std::endl;
	for(size_t i=0;i<NLISTS;i++){
		if(_spanlist[i].is_empty())
			continue;
		Span* cur = _spanlist[i].begin();
		while(cur->_next!=_spanlist[i].begin()){
				void* obj = cur->_objlist;
		//		while(obj!=nullptr){
				std::cout<<"该span在数组中的下标："<<i<<std::endl;
				std::cout<<"该span内的obj起始地址："<<obj<<std::endl;
				std::cout<<"该span中有"<<cur->_npage<<"页"<<std::endl;
				std::cout<<"对象大小："<<cur->_obj_size<<std::endl;
				std::cout<<"对象块的实用计数："<<cur->_use_count<<std::endl;
				std::cout<<std::endl;
		//		obj = NEXT_OBJ(obj);
		//	}
			cur = cur->_next;
		}
		std::cout<<""<<std::endl;
	}
	std::cout<<"#####################"<<std::endl;
	std::cout<<std::endl;
}
//centralCache* central = centralCache::getInstance();
//给threadCache分配一定数量的对象
//获取一个span，切分为num个大小为byte的块，返回给threadCache，剩下继续放在centralCache
size_t centralCache::provideRangeObjToThreadCache(void*& start,void*& end,size_t n,size_t bytes){
	assert(bytes<MAXBYTES);
//	std::cout<<"centralcache::provideRangeObjToThreadCache->getOneSpan"<<std::endl;
	//根据bytes计算出要在spanlist数组中获取的index(先对齐)
	//用了threadCache同样的对齐方式，说明_spanlist和threadCache.cpp中的_freelist是结构一样的
	mtx.lock();
	size_t index = classSize::index(bytes);	
	//size_t byte;
	Spanlist* list = &_spanlist[index];
	//根据获取到的list和byte去调用getOneSpan获取span
	Span* new_span = getOneSpan(list,bytes);
	/*
	//根据此次申请的内存快大小_size构造自由链表。
	char* new_start = (char*)new_span->_objlist;//依据是pageCache里面的申请pageId初始化。
	char* new_end = new_start+new_span->_npage*4*1024;
        char* new_cur = new_start;
        char* new_next = new_start+bytes;
        while(new_next<new_end){
                NEXT_OBJ(new_cur) = new_next;
                new_cur = new_next;
                new_next += bytes;
//                count++;
        }
	new_span->_obj_size = bytes;
*/
	std::cout<<"central未向thread分割前："<<std::endl;
	printCentralCache();
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
	new_span->_objlist = NEXT_OBJ(prev);
	new_span->_use_count+=p_num;
	NEXT_OBJ(prev) = nullptr;
	//list->push_front(new_span);
	//计算剩余大小
/*	bytes = new_span->_npage*4*1024-n*bytes;
	size_t new_index = classSize::index(bytes);//计算剩余的内存快在数组的下标
	_spanlist[new_index].push_front(new_span);
	*/
	std::cout<<"central向thread分割后："<<std::endl;
	printCentralCache();
	mtx.unlock();
	return p_num;
};

//获取一个span，返回，不够就去pageCache要
Span* centralCache::getOneSpan(Spanlist* list,size_t _size){
//	std::cout<<"centralcache::getOneSpan->AllocmemFormCentralCache"<<std::endl;
	//在spanlist中找到了，直接返回
	if(list->is_empty()){		
	//没有找到，就去pageCache中获取新的span
		size_t npage = classSize::numToPage(_size);//计算需要多少页(不只是将_size转为page，这里计算的是申请很多个_size大小的内存快需要多少页，很多是多少在numMoveSize定义)
		std::cout<<npage<<std::endl;
		Span* span = pageCache::getPageCacheObj()->AllocMemForCentralCache(npage);
		//设置span的objlist:就是通过NEXT_OBJ建立映射，构造成链表形状
		char* start = (char*)(span->_page_id*4*1024);//依据是pageCache里面的申请pageId初始化。
		char* end = start+span->_npage*4*1024;
		char* cur = start;
		char* next = start+_size;
		while(next<end){
			NEXT_OBJ(cur) = next;
		       	cur = next;
			next += _size;
//			count++;
		}
		span->_objlist = start;
		span->_use_count = 0;
		span->_obj_size = _size;
		//将新的span挂在spanlist上
		list->push_front(span);	
		return span;
	}
	Span* span = list->begin();
	while(span!=list->end()){
		if(span->_objlist!=nullptr){
			return span;//找一个不空的span返回
		}
		span = span->_next;
	}	
//	list->pop_front();
	return span;
};

//释放一定数量的对象到page
//以span为单位返回给pageCache
void centralCache::releaseListToSpan(void* start,size_t byte_size){

};
