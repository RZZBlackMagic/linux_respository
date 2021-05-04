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
//	std::cout<<"central未向thread分割前："<<std::endl;
//	printCentralCache();
	//在span中分割出num个byte大小的块后
	void* obj = new_span->_objlist;
	start = obj;
	size_t p_num = 0;	
	void* prev = nullptr;
	while(obj!=nullptr&&p_num<n){
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
//	std::cout<<"central向thread分割后："<<std::endl;
//	printCentralCache();
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
		//要申请很多个内存快的size总和转换为npage，也就是一个span的大小。将这一个span逻辑上划为自由链表。
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
		NEXT_OBJ(cur) = nullptr;
		span->_objlist = start;
		span->_use_count = 0;
		std::cout<<"getOneSpan"<<_size<<std::endl;
		span->_obj_size = _size;
		//将新的span挂在spanlist上，这里的list存储是自由链表计算下标的，并不是span大小。
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
void centralCache::returnObjToCentral(void* start){
//	std::cout<<">?<"<<std::endl;
//	printCentralCache();
//	pageCache::getPageCacheObj()->printMap();
	//有可能是申请了两次，这两次申请的链是连在一起的只有一个nullptr，第一次在后面，第二次在前面（因为是头插）
	//如果 释放第二次，没问题，直接走到nullptr结束。如果释放第一次的，就将两次一起释放了，处理两个span
	while(start){
		void* next = NEXT_OBJ(start);
		Span* span = pageCache::getPageCacheObj()->MapToObj(start);
		//根据映射找到对应的span，这个span是正在用的span，在central中，不在page中。将其链接上去（这就是还给central了）
		NEXT_OBJ(start) = span->_objlist;
		span->_objlist = start;//还回来的本质就是将span的objlist链接回来，分配的本质就是将span的objlist向后移动。span还是在spanlist上面挂的。返回去的只是一个span的objlist上面的部分void*块。
		span->_use_count--;
		//该span没有使用者了。
		if(span->_use_count==0){
			//说明span没有人在使用，返回到page
			//在central将其删掉
			size_t _index = classSize::index(span->_obj_size);
			_spanlist[_index].earse(span);
			span->_obj_size = 0;
			span->_use_count = 0;
			span->_objlist = nullptr;
			span->_next = nullptr;
			span->_prev = nullptr;
			pageCache::getPageCacheObj()->TakeSpanToPage(span);	
		}
		start = next;
	}
//	printCentralCache();
//	pageCache::getPageCacheObj()->printMap();
	//考虑一下：如果上面循环走完，use_count还不为0咋办？
	//提示：根据分配原则，每次申请的内存块(n个)内存大小向上调整后就是分配的内存大小。分配的n个内存块逻辑上构造成自由链表
	//末尾为nullptr，本次释放也就是要释放这n个内存块。
};
