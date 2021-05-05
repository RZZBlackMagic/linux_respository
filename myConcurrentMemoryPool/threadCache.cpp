#include "threadCache.h"
void* threadCache::Allocate(size_t size){
	assert(size<=MAXBYTES);
	//通过size计算所需内存大小（内存对其
	size = classSize::round_up(size);
	//计算该大小内存在链表中的位置
	size_t _index = classSize::index(size);
	if(_freelist[_index].is_empty())
		return getFormCentralCache(_index,size);
	void* res = _freelist[_index].pop();
	//_freelist[_index].clear();
	return res;
}

void threadCache::printThreadCache(){
        std::cout<<"<<<<<<<<<<<<<<<<<<<<<<<<"<<std::endl;
        std::cout<<"ThreadCache的span情况:"<<std::endl;
        for(size_t i=0;i<NLISTS;i++){
                if(_freelist[i].is_empty())
                        continue;
		void* cur = _freelist[i].get_ptr();
		std::cout<<">>>"<<std::endl;
		while(cur!=nullptr){
                        std::cout<<"该对象在数组中的下标："<<i<<std::endl;
                        std::cout<<"该对象大小："<<_freelist[i].size()<<std::endl;
			std::cout<<"该对象的起始地址："<<cur<<std::endl;
			std::cout<<std::endl;
			cur = NEXT_OBJ(cur);
                }
                std::cout<<""<<std::endl;
        }
        std::cout<<"<<<<<<<<<<<<<<<<<<<<<<<<<<"<<std::endl;
	std::cout<<std::endl;
}


void threadCache::Deallocate(void* start){
	std::cout<<"回收前thread的自由链表："<<std::endl;
	printThreadCache();
	//1 回收的内存大小小于64k时，直接挂在freelist上
	Span* span = pageCache::getPageCacheObj()->MapToObj(start);	
	assert(span->_obj_size<=MAXBYTES);
	if(span->_obj_size>MAXBYTES){
		//通过Allocate申请的内存快小于64k，所以不会到这。
		//	PageCache::getPageCacheObj()->TakeSpanToPage(span);
	}
	size_t _index = classSize::index(span->_obj_size);
	_freelist[_index].push(start);//回收自由链表的一块，[start,NEXT_OBJ(start)];
	//printThreadCache();
	//2 如果链表的长度大于max_size，则回收到central
	//为什么要大于max_size再回收，因为一次申请一个size大小的内存。但是分配max_size个，所以当大于max_size时，至少申请了两次，里面最少有2×maxsize个
	//但是只需要一个，内存浪费严重，所以回收
	if(_freelist[_index].size() >= _freelist[_index].max_size()){
		_freelist[_index].clear();
		centralCache::getInstance()->returnObjToCentral(start);
	}
}

void* threadCache::getFormCentralCache(size_t index,size_t size){
	//1. 当内存申请size<=64k时在thread cache中申请内存，计算size在自由链表中的位置，如果自由链表中
	//有内存对象时，直接从FistList[i]中Pop一下对象，时间复杂度是O(1)，且没有锁竞争。
	//2. 当FreeList[i]中没有对象时，则批量从central cache中获取一定数量的对象，插入到自由链表并返回一
	//个对象。
	
	//通过size大小来计算所需内存块的大小（内存对其
	size = classSize::round_up(size);
	//size_t size = Roundup(size);
        //通过内存快大小来计算所需内存快在自由链表中的位置下表
        //如果对应位置有内存快，取出返回
        //如果没有，去centralCache取内存块 getInstance
	size_t num = THREAD_MAX_SIZE;//一次从central获取十个,这里一次分配十个，这十个是以自由链表形状链接，末尾为nullptr，回收时，也以10为单位回收
	void* start;
	void* end;
	//get_size表示真正拿到的块数
	size_t get_size = centralCache::getInstance()->provideRangeObjToThreadCache(start,end,num,size);
	if(get_size==1){
		//只有一块，直接返回
		return start;
	}
	//将剩余的挂在自由链表上,start返回(已经在central分割好了，这里直接push就可以了)
	_freelist[index].push_range(NEXT_OBJ(start),end,get_size-1);
	return start;
}

void threadCache::returnObjToCentralcache(Freelist* list){
	void* start = list->clear();
	centralCache::getInstance()->returnObjToCentral(start);
}
