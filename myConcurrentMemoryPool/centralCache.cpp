#include "centralCache.h"

//给threadCache分配一定数量的对象
//获取一个span，切分为num个大小为byte的块，返回给threadCache，剩下继续放在centralCache
size_t centralCache::provideRangeObjToThreadCache(void*& start,void*& end,size_t n,size_t bytes_size){
	
};

//获取一个span，返回，不够就去pageCache要
Span* centralCache::getOneSpan(Spanlist* list,size_t _size){
	
};

//释放一定数量的对象到page
//以span为单位返回给pageCache
void centralCache::releaseListToSpan(void start,size_t byte_size){

};
