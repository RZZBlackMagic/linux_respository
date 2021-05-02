#include "threadCache.h"
void* threadCache::Allocate(size_t size){
	//通过size计算所需内存大小（内存对其
	size_t size = classSize::round_up(size);
	//计算该大小内存在链表中的位置
	size_t index = classSize::index(size);
	if(_freelist[index].is_empty())
		return getFormCentralCache(index,size);
	void* res = _freelist[index].pop();
	_freelist[index].clear();	
	//std::cout<<"threadcache::Allocate->getFormcentralCache"<<std::endl;
	//void* start = threadCache::getFormCentralCache(index,size);
	return res;
}

void threadCache::Deallocate(void* start){

}

void* threadCache::getFormCentralCache(size_t index,size_t size){
	std::cout<<"threadcache::getFormCentralCache->provideRangeObjThreadCache"<<std::endl;
	//1. 当内存申请size<=64k时在thread cache中申请内存，计算size在自由链表中的位置，如果自由链表中
	//有内存对象时，直接从FistList[i]中Pop一下对象，时间复杂度是O(1)，且没有锁竞争。
	//2. 当FreeList[i]中没有对象时，则批量从central cache中获取一定数量的对象，插入到自由链表并返回一
	//个对象。
	
	//通过size大小来计算所需内存块的大小（内存对其
	size_t size = classSize::round_up(size);
	//size_t size = Roundup(size);
        //通过内存快大小来计算所需内存快在自由链表中的位置下表
        //如果对应位置有内存快，取出返回
        //如果没有，去centralCache取内存块 getInstance
	size_t num = 10;//一次从central获取十个
	void* start;
	void* end;
	//get_size表示真正拿到的块数
	size_t get_size = centralCache::getInstance()->provideRangeObjToThreadCache(start,end,size,num);
	if(get_size==1){
		//只有一块，直接返回
		return start;
	}
	//将剩余的挂在自由链表上,start返回
	_freelist[index].push_range(NEXT_OBJ(start),end,get_size-1);
	//centralCache->provideRangeObjToThreadCache(void*& start,void*& end,size_t n,size_t bytes_size);	
	return start;
}

void threadCache::returnObjToCentralcache(Freelist* list,size_t size){

}
