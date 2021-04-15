#include <mutex>
#include "common.h"
class centralCache{
public:
	static centralCache* getInstance(){
		if(_Inst==nullptr)
			return new centralcache();
		return &_Inst
	}
	 //从spanlist中获取一个span
        Span* getOneSpan(Spanlist* list,size_t _size);
	//给threadCache分配一定数量的对象
	size_t provideRangeObjToThreadCache(void*& start,void*& end,size_t n,size_t bytes_size);
	//释放spanlist的一部分对象
	void releaseListToSpan(void start,size_t byte_size);
private:
	static centralCache _Inst;
	centralCache() = default;
	centralCache(const centralCache&) = delete;
	centralCache& operator=(const centralcache&) = delete;

	Spanlist _spanlist[NLISTS];
	std::mutex mtx;
};
