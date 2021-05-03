#include <mutex>
#include "pageCache.h"
class centralCache{
public:
	static centralCache* getInstance(){
		if(_Inst==nullptr)
			return new centralCache();
		return _Inst;
	};
	 //从spanlist中获取一个span
        Span* getOneSpan(Spanlist* list,size_t _size);
	//给threadCache分配一定数量的对象
	size_t provideRangeObjToThreadCache(void*& start,void*& end,size_t n,size_t bytes_size);
	//释放spanlist的一部分对象
	void releaseListToSpan(void* start,size_t byte_size);
	void printCentralCache();
private:
	centralCache() = default;
	centralCache(const centralCache&) = delete;
	centralCache& operator=(const centralCache&) = delete;

	static centralCache* _Inst ;
	//centralCache是span数组,以span为单位存储.central向page申请时，就将其构造成相应的自由链表，objsize就是此次申请的大小
	//将需要数量的obj返回，剩下的就挂在自由链表上，下次如果还申请同样大小的obj，就直接在自由链表上拿。如果下次申请的比这个大，就去page
	//重点：central表面上是按照span为单位存储的，但其实是按照自由链表存储的。
	Spanlist _spanlist[NLISTS];
	std::mutex mtx;
};
/*
 *thread和central都是自由链表，区别在于，thread的链表节点是void*块，central的链表节点都是span块，central的每一个span内都是自由链表，
 *相同点：都是自由链表，都是通过NEXT_OBJ链接的。数组存储的格式一样，下标计算方式也一样。
 *
 * */
