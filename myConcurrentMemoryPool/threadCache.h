
#include "centralCache.h"
class threadCache{
public:
	threadCache()=default;
	// 面向线程的接口
	void* Allocate(size_t size);
	void Deallocate(void* start);
	void printThreadCache();

private:
	//从central获取:getFormCentralCache(
	void* getFormCentralCache(size_t, size_t);
	//void* getFromCentralCache(size_t index,size_t size);
	//向central释放
	void returnObjToCentralcache(Freelist* list);

	Freelist _freelist[NLISTS];
};
