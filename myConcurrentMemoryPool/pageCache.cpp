#include "pageCache.h"
#include <errno.h>
pageCache* pageCache::page = nullptr;
void pageCache::printPageList(){
	std::cout<<"***********************"<<std::endl;
	std::cout<<"pageCache中的span："<<std::endl;
	for(size_t i=0;i<NPAGES;i++){
		if(_pagelist[i].is_empty())
			continue;
		Span* cur = _pagelist[i].begin();
		while(cur->_next!=_pagelist[i].begin()){
			std::cout<<"当前span的下标为："<<i<<std::endl;
			std::cout<<"span的size："<<i<<std::endl;
			std::cout<<"span的起始地址："<<cur->_objlist<<std::endl;
			std::cout<<std::endl;
			cur = cur->_next;
		}
		std::cout<<" "<<std::endl;
	}	
	std::cout<<"************************"<<std::endl;
	std::cout<<std::endl;
}
//申请npage的span，span是page的整数倍
//npage:多少个page
Span* pageCache::AllocMemForCentralCache(size_t npage) {
	return _newSpan(npage);	
}

void pageCache::FreeMemFromCentralCache(Span* span) {
	assert(span);
}

void* pageCache::AllocMemFromSystem() {
	extern int errno;
	//只有一个pagecache，不需要实现互斥
	//申请NPAGES大小的页
	void* ptr = mmap(NULL,(NPAGES-1)*4*1024, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS , -1, 0);
	assert(ptr!=MAP_FAILED);//条件不成立。扑捉错误
	return ptr;
}
Span* pageCache::_newSpan(size_t npage){
	//先去找对应的Spanlist的下标
	//assert(npage<NPAGES);
	if(!_pagelist[npage].is_empty()&&_pagelist[npage].begin()!=_pagelist[npage].end()){
		//找到了span
		Span* span =  _pagelist[npage].begin();
		_pagelist[npage].pop_front();
		return span;
	}
	//如果该下标有span，直接获取
	//如果没有，则继续网后面找，后面的有span，拿到后拆分
	for(int i=npage+1;i<NPAGES;i++){
		if(!_pagelist[i].is_empty()){
			//进行拆分，将拆分后的一快返回，其他剩余的挂在freelist中
			Span* origin_span = _pagelist[i].begin();
			_pagelist[i].pop_front();
		       //从后面拆，origin记录的是剩下的
			Span* split = new Span;
			split->_page_id = origin_span->_page_id + origin_span->_npage-npage;
			split->_npage = npage;
			//处理剩下的
			origin_span->_npage = origin_span->_npage - npage;//计算剩余内存快的页数，
			_pagelist[origin_span->_npage].push_front(origin_span);			
			//更新map，记录split的页
			for(size_t j=0;j<split->_npage;j++){
				_id_span_map[split->_page_id+j] = split;
			}
			return split;
		}
	}
	//如果后面没有向系统申请NPAGES页内存
	void* ptr = AllocMemFromSystem();
	//将其挂在最后面，也就是size最大的freelist中
	Span* maxSpan = new Span;
	maxSpan->_objlist = ptr;
	maxSpan->_page_id = (PageID)ptr>>12;//4<<12=4k   4k>>12=4
	maxSpan->_npage = NPAGES-1;
	_pagelist[NPAGES-1].push_back(maxSpan);
	//申请完了后，继续_newSpan分配
	return _newSpan(npage);
};

Span* pageCache::MapToObj(void* ptr){
	PageID page_id = (PageID)ptr>>12;	
	auto it = _id_span_map.find(page_id);
	if(it==_id_span_map.end()){
		assert(false);
	}
	return it->second;
}
//回收分配出去的span块，在map里面寻找没有使用的span，将其合并，回收到pagecache
void pageCache::TakeSpanToPage(Span* span){
	//向前合并
	auto prev_it = _id_span_map.find(span->_page_id-1);
	while(prev_it!=_id_span_map.end()){
		Span* prev_span = prev_it->second;
		if(prev_span->_use_count!=0){
			break;
		}
		size_t new_page = prev_span->_npage+span->_npage;
		if(new_page>NPAGES-1){
			break;
		}
		//在map中将prev删除
		_id_span_map.erase(prev_it);
		//合并span和prev_span
		prev_span->_npage = new_page;
		span = prev_span;
		prev_it = _id_span_map.find(span->_page_id-1);

	}
	//向后合并	
	auto next_it = _id_span_map.find(span->_page_id+span->_npage);
	while(next_it!=_id_span_map.end()){
		Span* next_span = next_it->second;
		if(next_span->_use_count!=0)
			break;
		size_t new_page = next_span->_npage+span->_npage;
		if(new_page>NPAGES-1){
			break;
		}
		//在map中删除next
		_id_span_map.erase(next_it);
		span->_npage = new_page;
		next_it = _id_span_map.find(span->_page_id+span->_npage);
 	}
	_pagelist[span->_npage].push_front(span);
}
void pageCache::printMap(){
	for(auto it = _id_span_map.begin();it!=_id_span_map.end();it++){
		std::cout<<"page_id:"<<it->first<<",span起始地址:"<<it->second<<std::endl;
	}
};
