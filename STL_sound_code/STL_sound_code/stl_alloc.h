#ifndef __SGI_STL_INTERNAL_ALLOC_H
#define __SGI_STL_INTERNAL_ALLOC_H

#ifdef __SUNPRO_CC
#  define __PRIVATE public
// Extra access restrictions prevent us from really making some things
// private.
#else
#  define __PRIVATE private
#endif

#ifdef __STL_STATIC_TEMPLATE_MEMBER_BUG
#  define __USE_MALLOC
#endif


// This implements some standard node allocators.  These are
// NOT the same as the allocators in the C++ draft standard or in
// in the original STL.  They do not encapsulate different pointer
// types; indeed we assume that there is only one pointer type.
// The allocation primitives are intended to allocate individual objects,
// not larger arenas as with the original STL allocators.

#ifndef __THROW_BAD_ALLOC
#  if defined(__STL_NO_BAD_ALLOC) || !defined(__STL_USE_EXCEPTIONS)
#    include <stdio.h>
#    include <stdlib.h>
#    define __THROW_BAD_ALLOC fprintf(stderr, "out of memory\n"); exit(1)
#  else /* Standard conforming out-of-memory handling */
#    include <new>
#    define __THROW_BAD_ALLOC throw std::bad_alloc()
#  endif
#endif

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#ifndef __RESTRICT
#  define __RESTRICT
#endif

#ifdef __STL_THREADS
# include <stl_threads.h>
# define __NODE_ALLOCATOR_THREADS true
# ifdef __STL_SGI_THREADS
  // We test whether threads are in use before locking.
  // Perhaps this should be moved into stl_threads.h, but that
  // probably makes it harder to avoid the procedure call when
  // it isn't needed.
extern "C" {
	extern int __us_rsthread_malloc;
}
// The above is copied from malloc.h.  Including <malloc.h>
// would be cleaner but fails with certain levels of standard
// conformance.
#   define __NODE_ALLOCATOR_LOCK if (threads && __us_rsthread_malloc) \
                { _S_node_allocator_lock._M_acquire_lock(); }
#   define __NODE_ALLOCATOR_UNLOCK if (threads && __us_rsthread_malloc) \
                { _S_node_allocator_lock._M_release_lock(); }
# else /* !__STL_SGI_THREADS */
#   define __NODE_ALLOCATOR_LOCK \
        { if (threads) _S_node_allocator_lock._M_acquire_lock(); }
#   define __NODE_ALLOCATOR_UNLOCK \
        { if (threads) _S_node_allocator_lock._M_release_lock(); }
# endif
#else
//  Thread-unsafe
#   define __NODE_ALLOCATOR_LOCK
#   define __NODE_ALLOCATOR_UNLOCK
#   define __NODE_ALLOCATOR_THREADS false
#endif

__STL_BEGIN_NAMESPACE

#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma set woff 1174
#endif

// Malloc-based allocator.  Typically slower than default alloc below.
// Typically thread-safe and more storage efficient.
#ifdef __STL_STATIC_TEMPLATE_MEMBER_BUG
# ifdef __DECLARE_GLOBALS_HERE
void(*__malloc_alloc_oom_handler)() = 0;
// g++ 2.7.2 does not handle static template data members.
# else
extern void(*__malloc_alloc_oom_handler)();
# endif
#endif
//第一级配置

template <int __inst>
class __malloc_alloc_template {

private:

	//oom：out of memory三个处理函数,因为是以malloc(),free(),realloc()等C函数执行实际内存分配释放，所以不能直接使用C++ new-handler机制(set_new_handler())，因为不是用::operator new来分配。
	//如果未设定处理函数，则抛出std::bad_alloc

	static void* _S_oom_malloc(size_t);
	static void* _S_oom_realloc(void*, size_t);

#ifndef __STL_STATIC_TEMPLATE_MEMBER_BUG
	static void(*__malloc_alloc_oom_handler)();
#endif

public:

	static void* allocate(size_t __n)
	{
		void* __result = malloc(__n);
		if (0 == __result) __result = _S_oom_malloc(__n);
		return __result;
	}

	static void deallocate(void* __p, size_t /* __n */)
	{
		free(__p);
	}

	static void* reallocate(void* __p, size_t /* old_sz */, size_t __new_sz)
	{
		void* __result = realloc(__p, __new_sz);
		if (0 == __result) __result = _S_oom_realloc(__p, __new_sz);
		return __result;
	}

	static void(*__set_malloc_handler(void(*__f)()))()
	{
		void(*__old)() = __malloc_alloc_oom_handler;
		__malloc_alloc_oom_handler = __f;
		return(__old);
	}

};

// malloc_alloc out-of-memory handling

#ifndef __STL_STATIC_TEMPLATE_MEMBER_BUG
template <int __inst>
void(*__malloc_alloc_template<__inst>::__malloc_alloc_oom_handler)() = 0;
#endif

template <int __inst>
void*
__malloc_alloc_template<__inst>::_S_oom_malloc(size_t __n)
{
	void(*__my_malloc_handler)();
	void* __result;

	for (;;) {
		__my_malloc_handler = __malloc_alloc_oom_handler;
		if (0 == __my_malloc_handler) { __THROW_BAD_ALLOC; }
		(*__my_malloc_handler)();
		__result = malloc(__n);
		if (__result) return(__result);
	}
}

template <int __inst>
void* __malloc_alloc_template<__inst>::_S_oom_realloc(void* __p, size_t __n)
{
	void(*__my_malloc_handler)();
	void* __result;

	for (;;) {
		__my_malloc_handler = __malloc_alloc_oom_handler;
		if (0 == __my_malloc_handler) { __THROW_BAD_ALLOC; }
		(*__my_malloc_handler)();
		__result = realloc(__p, __n);
		if (__result) return(__result);
	}
}

typedef __malloc_alloc_template<0> malloc_alloc;

//   allocate->_S_oom_malloc->__malloc_alloc_oom_handler
//   reallocate->_S_oom_realloc->__malloc_alloc_oom_handler
//--------------------------------------------------------------------------------------------------

//外面再多一层封装，使Alloc具备标准接口

template<class _Tp, class _Alloc>
class simple_alloc {

public:
	static _Tp* allocate(size_t __n)
	{
		return 0 == __n ? 0 : (_Tp*)_Alloc::allocate(__n * sizeof(_Tp));
	}
	static _Tp* allocate(void)
	{
		return (_Tp*)_Alloc::allocate(sizeof(_Tp));
	}
	static void deallocate(_Tp* __p, size_t __n)
	{
		if (0 != __n) _Alloc::deallocate(__p, __n * sizeof(_Tp));
	}
	static void deallocate(_Tp* __p)
	{
		_Alloc::deallocate(__p, sizeof(_Tp));
	}
};

// Allocator adaptor to check size arguments for debugging.
// Reports errors using assert.  Checking can be disabled with
// NDEBUG, but it's far better to just use the underlying allocator
// instead when no checking is desired.
// There is some evidence that this can confuse Purify.

//带有debug功能的allocator适配器
//新分配地址的头部额外多增加8个字节，用来存放新分配空间的大小，dealloate和reallocate会验证传入的释放空间大小是否等于待释放地址头部8个字节存放的空间大小，

//如果不相等，则abort。

template <class _Alloc>
class debug_alloc {

private:

	enum { _S_extra = 8 };   //存储分配空间大小所需的位数

public:

	static void* allocate(size_t __n)
	{
		char* __result = (char*)_Alloc::allocate(__n + (int)_S_extra);
		*(size_t*)__result = __n;
		return __result + (int)_S_extra;
	}

	static void deallocate(void* __p, size_t __n)
	{
		char* __real_p = (char*)__p - (int)_S_extra;
		assert(*(size_t*)__real_p == __n);
		_Alloc::deallocate(__real_p, __n + (int)_S_extra);
	}

	static void* reallocate(void* __p, size_t __old_sz, size_t __new_sz)
	{
		char* __real_p = (char*)__p - (int)_S_extra;
		assert(*(size_t*)__real_p == __old_sz);
		char* __result = (char*)
			_Alloc::reallocate(__real_p, __old_sz + (int)_S_extra,
				__new_sz + (int)_S_extra);
		*(size_t*)__result = __new_sz;
		return __result + (int)_S_extra;
	}

};


# ifdef __USE_MALLOC

typedef malloc_alloc alloc;
typedef malloc_alloc single_client_alloc;

# else

//  Sun C++ compiler需要在类外定义这些枚举  

#if defined(__SUNPRO_CC) || defined(__GNUC__)
// breaks if we make these template class members:
enum { _ALIGN = 8 };       //小型区块的上调边界
enum { _MAX_BYTES = 128 }; //小型区块的上限
enum { _NFREELISTS = 16 }; // _MAX_BYTES/_ALIGN   free-lists 个数
#endif

//第二级配置器
// 默认的node allocator  
// 如果有合适的编译器, 速度上与原始的STL class-specific allocators大致等价 
// 但是具有产生更少内存碎片的优点  
// Default_alloc_template参数是用于实验性质的, 在未来可能会消失 
// 客户只能在当下使用alloc  
//  
// 重要的实现属性:  
// 1. 如果客户请求一个size > __MAX_BYTE的对象, 则直接使用malloc()分配 
// 2. 对于其它情况下, 我们将请求对象的大小按照内存对齐向上舍入ROUND_UP(requested_size)   
//    Thus the client has enough size information that we can return the object to the proper free list  
//    without permanently losing part of the object.  
//  

//  第一个模板参数指定是否有多于一个线程使用本allocator  // 在一个default_alloc实例中分配对象, 在另一个deallocate实例中释放对象, 是安全的 
// 这有效的转换其所有权到另一个对象 
// 这可能导致对我们引用的区域产生不良影响 
// 第二个模板参数仅仅用于创建多个default_alloc实例 
// 不同容器使用不同allocator实例创建的node拥有不同类型, 这限制了此方法的通用性 

 //allocate()大于128调用第一级适配器，否则在free-list中找，没找到调用refill()从内存池中填充到free-list
 //refill()调用chunk_alloc从内存池中取空间给free-list使用，然后调整free-list链表

 //内存池中有20个或有但不满20个，则调整内存池的起始位置，返回内存地址，

 //内存池中一个也不能满足了，则如果内存池中还要剩余的一些零头，则编入free-list，然后尝试从heap分配40+n块空间到内存池。分配成功后递归调用返回空间地址
//heap上也没有空间分配了，则先检查free-list中有没比所需块大一点的空间，有则分配给他，堆上也没有则调用第一级配置器，改善或排除异常。

template <bool threads, int inst>
class __default_alloc_template {

private:
	// Really we should use static const int x = N
	// instead of enum { x = N }, but few compilers accept the former.
#if ! (defined(__SUNPRO_CC) || defined(__GNUC__))
	enum { _ALIGN = 8 };
	enum { _MAX_BYTES = 128 };
	enum { _NFREELISTS = 16 }; // _MAX_BYTES/_ALIGN
# endif
	static size_t
		_S_round_up(size_t __bytes)          //向上调整至的倍数
	{
		return (((__bytes)+(size_t)_ALIGN - 1) & ~((size_t)_ALIGN - 1));
	}

__PRIVATE:
	union _Obj {
		union _Obj* _M_free_list_link;
		char _M_client_data[1];    /* The client sees this.        */  //柔性数组
	};

	/*
	int main()
	{
		_Obj* test = (_Obj*)"12345678";
		cout << sizeof(_Obj) <<endl;       //4
		cout<<test->_M_client_data<<endl;  //"12345678"
		return 0;
	}
	*/


private:
# if defined(__SUNPRO_CC) || defined(__GNUC__) || defined(__HP_aCC)
	static _Obj* __STL_VOLATILE _S_free_list[];           volatile关键字，优化器在用到这个变量时必须每次都小心地重新读取这个变量的值，而不是使用保存在寄存器里的备份。
		// Specifying a size results in duplicate def for 4.1
# else
	static _Obj* __STL_VOLATILE _S_free_list[_NFREELISTS];
# endif
	static  size_t _S_freelist_index(size_t __bytes) { //根据区块大小，使用第n号free-list
		return (((__bytes)+(size_t)_ALIGN - 1) / (size_t)_ALIGN - 1);//向上取，再减1
	}

	// // 返回一个大小为n的对象，并可能加入大小为n的其他区块到free-list
	static void* _S_refill(size_t __n);

	//配置一大块空间，可容纳nobjs个大小为size的区块
	//如果配置nobjs个区块有所不便,nobjs可能会减小

	static char* _S_chunk_alloc(size_t __size, int& __nobjs);

	// Chunk allocation state.
	static char* _S_start_free;  // 内存池起始点
	static char* _S_end_free;    // 内存池结束点
	static size_t _S_heap_size;  // 已经在堆上分配的空间大小 

# ifdef __STL_THREADS
	static _STL_mutex_lock _S_node_allocator_lock;
# endif

	// It would be nice to use _STL_auto_lock here.  But we
	// don't need the NULL check.  And we do need a test whether
	// threads have actually been started.
	class _Lock;
	friend class _Lock;
	class _Lock {
	public:
		_Lock() { __NODE_ALLOCATOR_LOCK; }
		~_Lock() { __NODE_ALLOCATOR_UNLOCK; }
	};

public:

	/* __n must be > 0      */
	static void* allocate(size_t __n)
	{
		void* __ret = 0;
		// 如果待分配对象大于__MAX_BYTES, 使用一级配置器分配  
		if (__n > (size_t)_MAX_BYTES) {
			__ret = malloc_alloc::allocate(__n);
		}
		else {
			_Obj* __STL_VOLATILE* __my_free_list
				= _S_free_list + _S_freelist_index(__n);
			// Acquire the lock here with a constructor call.
			// This ensures that it is released in exit or during stack
			// unwinding.
#     ifndef _NOTHREADS
	  /*REFERENCED*/
			_Lock __lock_instance;
#     endif
			_Obj* __RESTRICT __result = *__my_free_list;
			//没有可用区块，就填充空间

			if (__result == 0)
				__ret = _S_refill(_S_round_up(__n));
			else {
				*__my_free_list = __result->_M_free_list_link;
				__ret = __result;
			}
		}

		return __ret;
	};

	/* __p may not be 0 */
	static void deallocate(void* __p, size_t __n)
	{
		if (__n > (size_t)_MAX_BYTES)
			malloc_alloc::deallocate(__p, __n);
		else {
			_Obj* __STL_VOLATILE*  __my_free_list
				= _S_free_list + _S_freelist_index(__n);
			_Obj* __q = (_Obj*)__p;

			// acquire lock
#       ifndef _NOTHREADS
	  /*REFERENCED*/
			_Lock __lock_instance;
#       endif /* _NOTHREADS */
			__q->_M_free_list_link = *__my_free_list;
			*__my_free_list = __q;
			// lock is released here
		}
	}

	static void* reallocate(void* __p, size_t __old_sz, size_t __new_sz);

};

typedef __default_alloc_template<__NODE_ALLOCATOR_THREADS, 0> alloc;
typedef __default_alloc_template<false, 0> single_client_alloc;

//任意两个allocator都是可互换的；如果a1和a2的类型都是allocator，我们可以自由地通过a1来allocate()内存然后通过 a2来deallocate()它。

//我们因此定义一个比较操作以表明所有allocator对象是等价的

template <bool __threads, int __inst>
inline bool operator==(const __default_alloc_template<__threads, __inst>&,
	const __default_alloc_template<__threads, __inst>&)
{
	return true;
}

# ifdef __STL_FUNCTION_TMPL_PARTIAL_ORDER
template <bool __threads, int __inst>
inline bool operator!=(const __default_alloc_template<__threads, __inst>&,
	const __default_alloc_template<__threads, __inst>&)
{
	return false;
}
# endif /* __STL_FUNCTION_TMPL_PARTIAL_ORDER */


// 每次分配一大块内存, 防止多次分配小内存块带来的内存碎片  // 进行分配操作时, 根据具体环境决定是否加锁 
// 我们假定要分配的内存满足内存对齐要求 
template <bool __threads, int __inst>
char*
__default_alloc_template<__threads, __inst>::_S_chunk_alloc(size_t __size,
	int& __nobjs)
{
	char* __result;
	size_t __total_bytes = __size * __nobjs;
	size_t __bytes_left = _S_end_free - _S_start_free;  // 计算内存池剩余容量 

	//  如果内存池中剩余内存>=需要分配的内内存, 返回start_free指向的内存块,  
	// 并且重新设置内存池起始点 
	if (__bytes_left >= __total_bytes) {
		__result = _S_start_free;
		_S_start_free += __total_bytes;
		return(__result);
	}
	// 如果内存吃中剩余的容量不够分配, 但是能至少分配一个节点时,   
	// 返回所能分配的最多的节点, 返回start_free指向的内存块 
	// 并且重新设置内存池起始点 
	else if (__bytes_left >= __size) {
		__nobjs = (int)(__bytes_left / __size);
		__total_bytes = __size * __nobjs;
		__result = _S_start_free;
		_S_start_free += __total_bytes;
		return(__result);
	}
	//  内存池剩余内存连一个节点也不够分配  
	else {
		size_t __bytes_to_get =
			2 * __total_bytes + _S_round_up(_S_heap_size >> 4);
		// Try to make use of the left-over piece.
		// 将剩余的内存分配给指定的free_list[FREELIST_INDEX(bytes_left)]  if (__bytes_left > 0) {
		_Obj* __STL_VOLATILE* __my_free_list =
			_S_free_list + _S_freelist_index(__bytes_left);

		((_Obj*)_S_start_free)->_M_free_list_link = *__my_free_list;
		*__my_free_list = (_Obj*)_S_start_free;
	}
	//配置heap空间，补充内存池
	_S_start_free = (char*)malloc(__bytes_to_get);
	if (0 == _S_start_free) {  //malloc()失败
		size_t __i;
		_Obj* __STL_VOLATILE* __my_free_list;
		_Obj* __p;

		//尝试检查我们手上拥有的东西，这不会造成伤害。我们不打算尝试配置较小的区块。
		//因为那在多进程机器上容易造成灾难

		//以下搜寻未使用的比size大的free-list区块
		for (__i = __size;
			__i <= (size_t)_MAX_BYTES;
			__i += (size_t)_ALIGN) {
			__my_free_list = _S_free_list + _S_freelist_index(__i);
			__p = *__my_free_list;
			if (0 != __p) { //有未用区块则调整free-list以释出未用区块
				*__my_free_list = __p->_M_free_list_link;
				_S_start_free = (char*)__p;
				_S_end_free = _S_start_free + __i;
				//递归调用自己，以修正nobjs
				return(_S_chunk_alloc(__size, __nobjs));

				//注意，残余零头终将被编入适当的free-list中备用
			}
		}
		_S_end_free = 0;    // In case of exception.
		_S_start_free = (char*)malloc_alloc::allocate(__bytes_to_get);
		//调用第一级配置器，看看out-of-memory机制能否尽力，或抛出异常。
	}
	_S_heap_size += __bytes_to_get;
	_S_end_free = _S_start_free + __bytes_to_get;
	return(_S_chunk_alloc(__size, __nobjs));
}
}

//  返回一个大小为n的对象, 并且加入到free_list[FREELIST_INDEX(n)]  // 进行分配操作时, 根据具体环境决定是否加锁  // 我们假定要分配的内存满足内存对齐要求
template <bool __threads, int __inst>
void*
__default_alloc_template<__threads, __inst>::_S_refill(size_t __n)
{
	int __nobjs = 20;
	char* __chunk = _S_chunk_alloc(__n, __nobjs);
	_Obj* __STL_VOLATILE* __my_free_list;
	_Obj* __result;
	_Obj* __current_obj;
	_Obj* __next_obj;
	int __i;

	if (1 == __nobjs) return(__chunk);
	__my_free_list = _S_free_list + _S_freelist_index(__n);

	/* Build free list in chunk */
	__result = (_Obj*)__chunk;
	*__my_free_list = __next_obj = (_Obj*)(__chunk + __n);
	for (__i = 1; ; __i++) {
		__current_obj = __next_obj;
		__next_obj = (_Obj*)((char*)__next_obj + __n);
		if (__nobjs - 1 == __i) {
			__current_obj->_M_free_list_link = 0;
			break;
		}
		else {
			__current_obj->_M_free_list_link = __next_obj;
		}
	}
	return(__result);
}

template <bool threads, int inst>
void*
__default_alloc_template<threads, inst>::reallocate(void* __p,
	size_t __old_sz,
	size_t __new_sz)
{
	void* __result;
	size_t __copy_sz;

	if (__old_sz > (size_t)_MAX_BYTES && __new_sz > (size_t)_MAX_BYTES) {
		return(realloc(__p, __new_sz));
	}
	if (_S_round_up(__old_sz) == _S_round_up(__new_sz)) return(__p);
	__result = allocate(__new_sz);
	__copy_sz = __new_sz > __old_sz ? __old_sz : __new_sz;
	memcpy(__result, __p, __copy_sz);
	deallocate(__p, __old_sz);
	return(__result);
}

#ifdef __STL_THREADS
template <bool __threads, int __inst>
_STL_mutex_lock
__default_alloc_template<__threads, __inst>::_S_node_allocator_lock
__STL_MUTEX_INITIALIZER;
#endif


template <bool __threads, int __inst>
char* __default_alloc_template<__threads, __inst>::_S_start_free = 0;

template <bool __threads, int __inst>
char* __default_alloc_template<__threads, __inst>::_S_end_free = 0;

template <bool __threads, int __inst>
size_t __default_alloc_template<__threads, __inst>::_S_heap_size = 0;

template <bool __threads, int __inst>
typename __default_alloc_template<__threads, __inst>::_Obj* __STL_VOLATILE
__default_alloc_template<__threads, __inst> ::_S_free_list[
# if defined(__SUNPRO_CC) || defined(__GNUC__) || defined(__HP_aCC)
	_NFREELISTS
# else
	__default_alloc_template<__threads, __inst>::_NFREELISTS
# endif
] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
	// The 16 zeros are necessary to make version 4.1 of the SunPro
	// compiler happy.  Otherwise it appears to allocate too little
	// space for the array.

#endif /* ! __USE_MALLOC */

// This implements allocators as specified in the C++ standard.  
//
// Note that standard-conforming allocators use many language features
// that are not yet widely implemented.  In particular, they rely on
// member templates, partial specialization, partial ordering of function
// templates, the typename keyword, and the use of the template keyword
// to refer to a template member of a dependent type.

#ifdef __STL_USE_STD_ALLOCATORS

	template <class _Tp>
	class allocator {
		typedef alloc _Alloc;          // The underlying allocator.
	public:
		typedef size_t     size_type;
		typedef ptrdiff_t  difference_type;
		typedef _Tp*       pointer;
		typedef const _Tp* const_pointer;
		typedef _Tp&       reference;
		typedef const _Tp& const_reference;
		typedef _Tp        value_type;

		template <class _Tp1> struct rebind {
			typedef allocator<_Tp1> other;
		};

		allocator() __STL_NOTHROW {}
		allocator(const allocator&) __STL_NOTHROW {}
		template <class _Tp1> allocator(const allocator<_Tp1>&) __STL_NOTHROW {}
		~allocator() __STL_NOTHROW {}

		pointer address(reference __x) const { return &__x; }
		const_pointer address(const_reference __x) const { return &__x; }

		// __n is permitted to be 0.  The C++ standard says nothing about what
		// the return value is when __n == 0.
		_Tp* allocate(size_type __n, const void* = 0) {
			return __n != 0 ? static_cast<_Tp*>(_Alloc::allocate(__n * sizeof(_Tp)))
				: 0;
		}

		// __p is not permitted to be a null pointer.
		void deallocate(pointer __p, size_type __n)
		{
			_Alloc::deallocate(__p, __n * sizeof(_Tp));
		}

		size_type max_size() const __STL_NOTHROW
		{
			return size_t(-1) / sizeof(_Tp);
		}

		void construct(pointer __p, const _Tp& __val) { new(__p) _Tp(__val); }
		void destroy(pointer __p) { __p->~_Tp(); }
	};

	template<>
	class allocator<void> {
	public:
		typedef size_t      size_type;
		typedef ptrdiff_t   difference_type;
		typedef void*       pointer;
		typedef const void* const_pointer;
		typedef void        value_type;

		template <class _Tp1> struct rebind {
			typedef allocator<_Tp1> other;
		};
	};


	template <class _T1, class _T2>
	inline bool operator==(const allocator<_T1>&, const allocator<_T2>&)
	{
		return true;
	}

	template <class _T1, class _T2>
	inline bool operator!=(const allocator<_T1>&, const allocator<_T2>&)
	{
		return false;
	}

	// Allocator adaptor to turn an SGI-style allocator (e.g. alloc, malloc_alloc)
	// into a standard-conforming allocator.   Note that this adaptor does
	// *not* assume that all objects of the underlying alloc class are
	// identical, nor does it assume that all of the underlying alloc's
	// member functions are static member functions.  Note, also, that 
	// __allocator<_Tp, alloc> is essentially the same thing as allocator<_Tp>.

	template <class _Tp, class _Alloc>
	struct __allocator {
		_Alloc __underlying_alloc;

		typedef size_t    size_type;
		typedef ptrdiff_t difference_type;
		typedef _Tp*       pointer;
		typedef const _Tp* const_pointer;
		typedef _Tp&       reference;
		typedef const _Tp& const_reference;
		typedef _Tp        value_type;

		template <class _Tp1> struct rebind {
			typedef __allocator<_Tp1, _Alloc> other;
		};

		__allocator() __STL_NOTHROW {}
		__allocator(const __allocator& __a) __STL_NOTHROW
			: __underlying_alloc(__a.__underlying_alloc) {}
		template <class _Tp1>
		__allocator(const __allocator<_Tp1, _Alloc>& __a) __STL_NOTHROW
			: __underlying_alloc(__a.__underlying_alloc) {}
		~__allocator() __STL_NOTHROW {}

		pointer address(reference __x) const { return &__x; }
		const_pointer address(const_reference __x) const { return &__x; }

		// __n is permitted to be 0.
		_Tp* allocate(size_type __n, const void* = 0) {
			return __n != 0
				? static_cast<_Tp*>(__underlying_alloc.allocate(__n * sizeof(_Tp)))
				: 0;
		}

		// __p is not permitted to be a null pointer.
		void deallocate(pointer __p, size_type __n)
		{
			__underlying_alloc.deallocate(__p, __n * sizeof(_Tp));
		}

		size_type max_size() const __STL_NOTHROW
		{
			return size_t(-1) / sizeof(_Tp);
		}

		void construct(pointer __p, const _Tp& __val) { new(__p) _Tp(__val); }
		void destroy(pointer __p) { __p->~_Tp(); }
	};

	template <class _Alloc>
	class __allocator<void, _Alloc> {
		typedef size_t      size_type;
		typedef ptrdiff_t   difference_type;
		typedef void*       pointer;
		typedef const void* const_pointer;
		typedef void        value_type;

		template <class _Tp1> struct rebind {
			typedef __allocator<_Tp1, _Alloc> other;
		};
	};

	template <class _Tp, class _Alloc>
	inline bool operator==(const __allocator<_Tp, _Alloc>& __a1,
		const __allocator<_Tp, _Alloc>& __a2)
	{
		return __a1.__underlying_alloc == __a2.__underlying_alloc;
	}

#ifdef __STL_FUNCTION_TMPL_PARTIAL_ORDER
	template <class _Tp, class _Alloc>
	inline bool operator!=(const __allocator<_Tp, _Alloc>& __a1,
		const __allocator<_Tp, _Alloc>& __a2)
	{
		return __a1.__underlying_alloc != __a2.__underlying_alloc;
	}
#endif /* __STL_FUNCTION_TMPL_PARTIAL_ORDER */

	// Comparison operators for all of the predifined SGI-style allocators.
	// This ensures that __allocator<malloc_alloc> (for example) will
	// work correctly.

	template <int inst>
	inline bool operator==(const __malloc_alloc_template<inst>&,
		const __malloc_alloc_template<inst>&)
	{
		return true;
	}

#ifdef __STL_FUNCTION_TMPL_PARTIAL_ORDER
	template <int __inst>
	inline bool operator!=(const __malloc_alloc_template<__inst>&,
		const __malloc_alloc_template<__inst>&)
	{
		return false;
	}
#endif /* __STL_FUNCTION_TMPL_PARTIAL_ORDER */


	template <class _Alloc>
	inline bool operator==(const debug_alloc<_Alloc>&,
		const debug_alloc<_Alloc>&) {
		return true;
	}

#ifdef __STL_FUNCTION_TMPL_PARTIAL_ORDER
	template <class _Alloc>
	inline bool operator!=(const debug_alloc<_Alloc>&,
		const debug_alloc<_Alloc>&) {
		return false;
	}
#endif /* __STL_FUNCTION_TMPL_PARTIAL_ORDER */

	// Another allocator adaptor: _Alloc_traits.  This serves two
	// purposes.  First, make it possible to write containers that can use
	// either SGI-style allocators or standard-conforming allocator.
	// Second, provide a mechanism so that containers can query whether or
	// not the allocator has distinct instances.  If not, the container
	// can avoid wasting a word of memory to store an empty object.

	// This adaptor uses partial specialization.  The general case of
	// _Alloc_traits<_Tp, _Alloc> assumes that _Alloc is a
	// standard-conforming allocator, possibly with non-equal instances
	// and non-static members.  (It still behaves correctly even if _Alloc
	// has static member and if all instances are equal.  Refinements
	// affect performance, not correctness.)

	// There are always two members: allocator_type, which is a standard-
	// conforming allocator type for allocating objects of type _Tp, and
	// _S_instanceless, a static const member of type bool.  If
	// _S_instanceless is true, this means that there is no difference
	// between any two instances of type allocator_type.  Furthermore, if
	// _S_instanceless is true, then _Alloc_traits has one additional
	// member: _Alloc_type.  This type encapsulates allocation and
	// deallocation of objects of type _Tp through a static interface; it
	// has two member functions, whose signatures are
	//    static _Tp* allocate(size_t)
	//    static void deallocate(_Tp*, size_t)

	// The fully general version.
	/*
	另一个allocator适配器_Alloc_traits, 它有两个意图
	  1.使容易既看使用sgi风格的allocator，也可以使用标准规格的allocator
	  2.提供了一种机制:容器可以查询allocator是否拥有明确的实体，如果没有，容易可以避免浪费内存去存储空对象
	这个适配器使用了偏特化。
	_Alloc_traits<_Tp, _Alloc>假定_Alloc是一个服从标准的allocator，可能有不相等的实例和非静态成员。
	(如果各实例相等或拥有静态成员，也表现正确。 优化影响执行效率，不影响正确性)
	有两个成员
	1. allocator_type 服从标准的分配_Tp类型对象的allocator类型
	2. _S_instanceless一个static const bool成员
	   如果为true，则表示任何两个allocator_type实例之间没有差别,同时,_Alloc_traits有额外的成员_Alloc_type
	_Alloc_type通过一个静态接口封装了分配和回收_Tp类型对象，它有两个成员函数，原型为
	 static _Tp* allocate(size_t)
	 static void deallocate(_Tp*, size_t)
	*/

	//写自定义分配器的时候也是，必须重写rebind，且不能有非静态成员。并假定任何同类型的allocator相等，即_S_instanceless为true
	template <class _Tp, class _Allocator>
	struct _Alloc_traits
	{
		static const bool _S_instanceless = false;
		typedef typename _Allocator::__STL_TEMPLATE rebind<_Tp>::other
			allocator_type;
	};

	template <class _Tp, class _Allocator>
	const bool _Alloc_traits<_Tp, _Allocator>::_S_instanceless;

	// The version for the default allocator.
	//给默认allocator的版本
	template <class _Tp, class _Tp1>
	struct _Alloc_traits<_Tp, allocator<_Tp1> >
	{
		static const bool _S_instanceless = true;
		typedef simple_alloc<_Tp, alloc> _Alloc_type;
		typedef allocator<_Tp> allocator_type;
	};

	// Versions for the predefined SGI-style allocators.

	template <class _Tp, int __inst>
	struct _Alloc_traits<_Tp, __malloc_alloc_template<__inst> >
	{
		static const bool _S_instanceless = true;
		typedef simple_alloc<_Tp, __malloc_alloc_template<__inst> > _Alloc_type;
		typedef __allocator<_Tp, __malloc_alloc_template<__inst> > allocator_type;
	};

	template <class _Tp, bool __threads, int __inst>
	struct _Alloc_traits<_Tp, __default_alloc_template<__threads, __inst> >
	{
		static const bool _S_instanceless = true;
		typedef simple_alloc<_Tp, __default_alloc_template<__threads, __inst> >
			_Alloc_type;
		typedef __allocator<_Tp, __default_alloc_template<__threads, __inst> >
			allocator_type;
	};

	template <class _Tp, class _Alloc>
	struct _Alloc_traits<_Tp, debug_alloc<_Alloc> >
	{
		static const bool _S_instanceless = true;
		typedef simple_alloc<_Tp, debug_alloc<_Alloc> > _Alloc_type;
		typedef __allocator<_Tp, debug_alloc<_Alloc> > allocator_type;
	};

	// Versions for the __allocator adaptor used with the predefined
	// SGI-style allocators.

	template <class _Tp, class _Tp1, int __inst>
	struct _Alloc_traits<_Tp,
		__allocator<_Tp1, __malloc_alloc_template<__inst> > >
	{
		static const bool _S_instanceless = true;
		typedef simple_alloc<_Tp, __malloc_alloc_template<__inst> > _Alloc_type;
		typedef __allocator<_Tp, __malloc_alloc_template<__inst> > allocator_type;
	};

	template <class _Tp, class _Tp1, bool __thr, int __inst>
	struct _Alloc_traits<_Tp,
		__allocator<_Tp1,
		__default_alloc_template<__thr, __inst> > >
	{
		static const bool _S_instanceless = true;
		typedef simple_alloc<_Tp, __default_alloc_template<__thr, __inst> >
			_Alloc_type;
		typedef __allocator<_Tp, __default_alloc_template<__thr, __inst> >
			allocator_type;
	};

	template <class _Tp, class _Tp1, class _Alloc>
	struct _Alloc_traits<_Tp, __allocator<_Tp1, debug_alloc<_Alloc> > >
	{
		static const bool _S_instanceless = true;
		typedef simple_alloc<_Tp, debug_alloc<_Alloc> > _Alloc_type;
		typedef __allocator<_Tp, debug_alloc<_Alloc> > allocator_type;
	};


#endif /* __STL_USE_STD_ALLOCATORS */

#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma reset woff 1174
#endif

	__STL_END_NAMESPACE

#undef __PRIVATE

#endif /* __SGI_STL_INTERNAL_ALLOC_H */

		// Local Variables:
		// mode:C++
		// End:
