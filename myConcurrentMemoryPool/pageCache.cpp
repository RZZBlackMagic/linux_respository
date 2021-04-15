#include "pageCache.h"

//申请npage的span，span是page的整数倍
//npage:多少个page
Span* pageCache::AllocMemForCentralCache(size_t npage) {
	return _newSpan(npage);	
}

void pageCache::FreeMemFromCentralCache(Span* span) {
	assert(span);
	
}

void* pageCache::AllocMemFromSystem() {
	//只有一个pagecache，不需要实现互斥
	//申请NPAGES大小的页
	void* ptr = mmap(NULL,(NPAGES-1)*4*1024, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | 0x40000 , -1, 0);
	assert(ptr!=MAP_FAILED);
	return ptr
}
Span* pageCache::_newSpan(size_t npage){
	//先去找对应的Spanlist的下标
	assert(npage<NPAGES);
	if(_pagelist[npage]!=nullptr)
		//找到了span
		return _pagelist[npage].pop_back();	
	//如果该下标有span，直接获取
	//如果没有，则继续网后面找，后面的有span，拿到后拆分
	for(int i=npage+1;i<NPAGES;i++){
		if(_pagelist[i]!=nullptr){
			//进行拆分，将拆分后的一快返回，其他剩余的挂在freelist中
			Span* origin_span = _pagelist[i].pop_front();
		       //从后面拆，origin记录的是剩下的	
			Span* split = new Span;
			split->_page_id = origin_span->_page_id+origin_span->_npage-npage;
			split->_npage = npage;
			//处理剩下的
			origin_span->_npage = origin_span->_npage-npage;//计算剩余内存快的页数，
			_pagelist[origin_span->_npage].push_front(origin_span);			
			//更新map，记录split的页
			for(size_t j=0;j<split->_npage;j++){
				_id_span_map[split->_page_id+j] = split;
			}
			return split;
		}
	}
	//如果后面没有向系统申请
	void* ptr = AllocMemFromSystem(npage);
	//将其挂在最后面，也就是size最大的freelist中
	Span* maxSpan = new Span;
	maxSpan->_objlist = ptr;
	maxSpan->_page_id = (PageID)ptr>>12;//4<<12=4k   4k>>12=4
	maxSpan->_npage = NPAGES-1;
	_pagelist[NPAGES-1].push_back(maxspan);
	//申请完了后，继续_newSpan分配
	return _newSpan(npage);
};
