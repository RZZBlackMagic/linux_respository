#include "common.h"
class threadCache{
public:
	// 面向线程的接口
	void* Allocate(size_t size);
	void Deallocate(void* start);


private:
	//从central获取
	void* getFromCentralCache(size_t index,size_t size);
	//向central释放
	void returnObjToCentralcache(Freelist* list,size_t size);

	Freelist _freelist[NLISTS];
};
