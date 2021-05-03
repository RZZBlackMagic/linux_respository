//#include "centralCache.h"
#include "threadCache.h"
//#include "pageCache.h"
/**
void printList(Spanlist* list){
	Span* span = list->begin();
	while(span->_page_id!=0){
		std::cout<<"page_id:"<<span->_page_id<<std::endl;
		span = span->_next;
	}
}
void freeListTest(){
	Spanlist* list = new Spanlist();
	for(int i=1;i<10;i++){
		Span* span = new Span;
		span->_page_id = i;
		list->push_front(span);
	}
	list->pop_front();
	list->pop_back();
	printList(list);
}
*/
class A{
	public:
	A(){};
	void p(){
		std::cout<<"hhhh"<<std::endl;
	}
};
void spanlistTest(){
	Spanlist list[10];
	if(list[11].is_empty()){
		std::cout<<"aaaa"<<std::endl;
	}else{
		std::cout<<"bbbb"<<std::endl;
	}
}
void pageTest(){
	pageCache* pageObj = pageCache::getPageCacheObj();
	Span* span = pageObj->AllocMemForCentralCache(NPAGES-1);
	if(span==nullptr){
		std::cout<<"page的AllocMemForCentrakCache出错"<<std::endl;
	}
}
void centralTest(){
//	centralCache* centralObj = centralCache::getInstance();
//	size_t res = provideRangeObjToThreadCache(void*& start,void*& end,size_t n,size_t bytes);
}
void NEXT_OBJ_test(){
	/*
	int b = 10;
	int a = b;
	std::cout<<&b<<std::endl;
	int *ptr = &a;
	std::cout<<ptr<<std::endl;
	std::cout<<*((int**)ptr)<<std::endl;
	*/
	void* ptr = mmap(NULL,(NPAGES-1)*4*1024, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS , -1, 0);
	std::cout<<ptr<<std::endl;
	Freelist list;
	void* temp1 = (void*)ptr;
	std::cout<<"a:"<<temp1<<std::endl;
	void * temp = *((void**)ptr);
//	int* a = (int*)temp;
//	*a = 10;
	std::cout<<"b:"<<((void**)ptr)<<std::endl;
//	list.push(ptr);
}
void classSizeTest(){
//	classSize cs;
	size_t res = classSize::index(164);
	//std::cout<<res<<std::endl;
	size_t r = res>>1;
	std::cout<<res<<std::endl;
	std::cout<<r<<std::endl;

}
void centralCacheTest(){
	void* start;
	void* end;
	size_t num = 10;
	size_t size = 16;
	size_t get_size = centralCache::getInstance()->provideRangeObjToThreadCache(start,end,num,size);	
	std::cout<<get_size<<std::endl;
	std::cout<<start<<std::endl;
	std::cout<<end<<std::endl;
}
void threadcacheTest(){
	threadCache tc ;
	void* start = tc.Allocate(164);	
	std::cout<<"test adress:"<<start<<std::endl;
	tc.Allocate(165);
}
int main(){
	//freeListTest();
//	threadCache* thread = new threadCache();
//	thread->Allocate(10);
	//int i =1;
	//std::cout<<(i>>12)<<std::endl;
//	A a[7] ;
//	a[6].p();
	//spanlistTest();
//	pageTest();
	//void* ptr = mmap(NULL,(NPAGES-1)*4*1024, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS , -1, 0);
	//std::cout<<(void*)ptr<<std::endl;
        //perror("reason:");
//      char * mesg = strerror(errno);
        //std::cout<<errno<<std::endl;
	//if(ptr==MAP_FAILED){
	//	std::cout<<"出错了"<<std::endl;	
	//}
//	NEXT_OBJ_test();
//	classSizeTest();
//	centralCacheTest();
	threadcacheTest();
	return 0;
}
