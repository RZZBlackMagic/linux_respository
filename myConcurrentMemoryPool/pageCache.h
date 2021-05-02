#pragma once
#include <iostream>
#include <sys/mman.h>
#include <mutex>
#include <unordered_map>
#include "common.h"
/*
	页缓存是在central cache缓存上面的一层缓存，存储的内存是以页为单位存储及分配
	central cache没有内存对象时，从page cache分配出一定数量的page，并切割成定长的
	小块内存，分配给central cache。page cache会回收central cache满足条件的span对象，
	并且合并相邻的页，组成更大的页，缓解内存碎片的问题
*/

class pageCache {
public:
//	static pageCache* pagecache;
	static pageCache* getPageCacheObj() {
		if (page == nullptr)
			page =  new pageCache();
		return page;
	};
	static void init(){
		getPageCacheObj();
	};
	/*
	1. 当central cache向page cache申请内存时，page cache先检查对应位置有没有span，如果没有则向更
	大页寻找一个span，如果找到则分裂成两个。比如：申请的是4page，4page后面没有挂span，则向后
	面寻找更大的span，假设在10page位置找到一个span，则将10page span分裂为一个4page span和一
	个6page span。
	2. 如果找到128 page都没有合适的span，则向系统使用mmap、brk或者是VirtualAlloc等方式申请
	128page span挂在自由链表中，再重复1中的过程。*/
	Span* AllocMemForCentralCache(size_t npage);
	/*
	如果central cache释放回一个span，则依次寻找span的前后page id的span，看是否可以合并，如果合
	并继续向前寻找。这样就可以将切小的内存合并收缩成大的span，减少内存碎片。*/
	void FreeMemFromCentralCache(Span* span);
	void ReturnSpanToPage(Span* span);
private:
	pageCache() {
		//for(size_t i=0;i<NPAGES;i++){
		//	_pagelist[i] = *(new Spanlist());
		//}			
	};//默认构造函数
	pageCache(const pageCache&) = delete;//禁止拷贝构造
	pageCache& operator=(const pageCache&) = delete;//禁止参数赋值
	/*向系统申请k页的内存
	*/
	void* AllocMemFromSystem();

	Span* _newSpan(size_t npage);
	static pageCache* page;	
	//static pageCache _inst;
	Spanlist _pagelist[NPAGES];//链表数组：数组的每个元素代表了size不同的内存块，size相同的内存块放在一个链表中。
	std::mutex _mtx;
	std::unordered_map<PageID, Span*> _id_span_map;
};

