// xmemory0 internal header
#pragma once
#ifndef _XMEMORY0_
#define _XMEMORY0_
#ifndef RC_INVOKED
#include <cstdint>	/* for uintptr_t */
#include <cstdlib>
#include <limits>
#include <new>
#include <xutility>

#pragma pack(push,_CRT_PACKING)
#pragma warning(push,_STL_WARNING_LEVEL)
#pragma warning(disable: _STL_DISABLED_WARNINGS)
_STL_DISABLE_CLANG_WARNINGS
#pragma push_macro("new")
#undef new

_STD_BEGIN
// FUNCTION TEMPLATE _Get_size_of_n
template<size_t _Ty_size> inline
size_t _Get_size_of_n(const size_t _Count)
{	// gets the size of _Count copies of a type sized _Ty_size
	constexpr size_t _Max_possible = static_cast<size_t>(-1) / _Ty_size;
	size_t _Result = _Count * _Ty_size;
	if (_Max_possible < _Count)
	{	// multiply overflow, try allocating all of memory and assume the
		// allocation function will throw bad_alloc
		_Result = static_cast<size_t>(-1);
	}

	return (_Result);
}

template<> inline
size_t _Get_size_of_n<1>(const size_t _Count)
{	// gets the size of _Count copies of a type with size 1
	return (_Count);
}

// VARIABLE TEMPLATE _New_alignof
template<class _Ty>
_INLINE_VAR constexpr size_t _New_alignof = _Max_value(alignof(_Ty),
	static_cast<size_t>(__STDCPP_DEFAULT_NEW_ALIGNMENT__) // TRANSITION, VSO#522105
);

// STRUCT _Default_allocate_traits
struct _Default_allocate_traits
{
	_DECLSPEC_ALLOCATOR static void * _Allocate(const size_t _Bytes)
	{
		return (::operator new(_Bytes));
	}

#if _HAS_ALIGNED_NEW
	_DECLSPEC_ALLOCATOR static void * _Allocate_aligned(const size_t _Bytes, const size_t _Align)
	{
		return (::operator new(_Bytes, align_val_t{ _Align }));
	}
#endif /* _HAS_ALIGNED_NEW */
};

constexpr bool _Is_pow_2(const size_t _Value) noexcept
{
	return (_Value != 0 && (_Value & (_Value - 1)) == 0);
}

#if defined(_M_IX86) || defined(_M_X64)
constexpr size_t _Big_allocation_threshold = 4096;
constexpr size_t _Big_allocation_alignment = 32;

static_assert(2 * sizeof(void *) <= _Big_allocation_alignment,
	"Big allocation alignment should at least match vector register alignment");
static_assert(_Is_pow_2(_Big_allocation_alignment),
	"Big allocation alignment must be a power of two");

#ifdef _DEBUG
constexpr size_t _Non_user_size = 2 * sizeof(void *) + _Big_allocation_alignment - 1;
#else /* _DEBUG */
constexpr size_t _Non_user_size = sizeof(void *) + _Big_allocation_alignment - 1;
#endif /* _DEBUG */

#ifdef _WIN64
constexpr size_t _Big_allocation_sentinel = 0xFAFAFAFAFAFAFAFAULL;
#else /* ^^^ _WIN64 ^^^ // vvv !_WIN64 vvv */
constexpr size_t _Big_allocation_sentinel = 0xFAFAFAFAUL;
#endif /* _WIN64 */

// FUNCTION _Allocate_manually_vector_aligned
template<class _Traits> inline
_DECLSPEC_ALLOCATOR void * _Allocate_manually_vector_aligned(const size_t _Bytes)
{	// allocate _Bytes manually aligned to at least _Big_allocation_alignment
	size_t _Block_size = _Non_user_size + _Bytes;
	if (_Block_size <= _Bytes)
	{	// add overflow, try allocating all of memory and assume the
		// allocation function will throw bad_alloc
		_Block_size = static_cast<size_t>(-1);
	}

	const uintptr_t _Ptr_container = reinterpret_cast<uintptr_t>(_Traits::_Allocate(_Block_size));
	_STL_VERIFY(_Ptr_container != 0, "invalid argument");	// validate even in release since we're doing p[-1]
	void * const _Ptr = reinterpret_cast<void *>((_Ptr_container + _Non_user_size)
		& ~(_Big_allocation_alignment - 1));
	static_cast<uintptr_t *>(_Ptr)[-1] = _Ptr_container;

#ifdef _DEBUG
	static_cast<uintptr_t *>(_Ptr)[-2] = _Big_allocation_sentinel;
#endif /* _DEBUG */
	return (_Ptr);
}

// FUNCTION TEMPLATE _Adjust_manually_vector_aligned
inline void _Adjust_manually_vector_aligned(void *& _Ptr, size_t& _Bytes)
{	// adjust parameters from _Allocate_manually_vector_aligned to pass to operator delete
	_Bytes += _Non_user_size;

	const uintptr_t * const _Ptr_user = reinterpret_cast<uintptr_t *>(_Ptr);
	const uintptr_t _Ptr_container = _Ptr_user[-1];

	// If the following asserts, it likely means that we are performing
	// an aligned delete on memory coming from an unaligned allocation.
	_STL_ASSERT(_Ptr_user[-2] == _Big_allocation_sentinel, "invalid argument");

	// Extra paranoia on aligned allocation/deallocation; ensure _Ptr_container is
	// in range [_Min_back_shift, _Non_user_size]
#ifdef _DEBUG
	constexpr uintptr_t _Min_back_shift = 2 * sizeof(void *);
#else /* ^^^ _DEBUG ^^^ // vvv !_DEBUG vvv */
	constexpr uintptr_t _Min_back_shift = sizeof(void *);
#endif /* _DEBUG */
	const uintptr_t _Back_shift = reinterpret_cast<uintptr_t>(_Ptr) - _Ptr_container;
	_STL_VERIFY(_Back_shift >= _Min_back_shift && _Back_shift <= _Non_user_size, "invalid argument");
	_Ptr = reinterpret_cast<void *>(_Ptr_container);
}
#endif /* defined(_M_IX86) || defined(_M_X64) */

// FUNCTION TEMPLATES _Allocate and _Deallocate
#if _HAS_ALIGNED_NEW
template<size_t _Align,
	class _Traits = _Default_allocate_traits,
	enable_if_t<(_Align > __STDCPP_DEFAULT_NEW_ALIGNMENT__), int> = 0> inline
	_DECLSPEC_ALLOCATOR void *_Allocate(const size_t _Bytes)
{	// allocate _Bytes when _HAS_ALIGNED_NEW && _Align > __STDCPP_DEFAULT_NEW_ALIGNMENT__
	if (_Bytes == 0)
	{
		return (nullptr);
	}

	size_t _Passed_align = _Align;
#if defined(_M_IX86) || defined(_M_X64)
	if (_Bytes >= _Big_allocation_threshold)
	{	// boost the alignment of big allocations to help autovectorization
		_Passed_align = _Max_value(_Align, _Big_allocation_alignment);
	}
#endif /* defined(_M_IX86) || defined(_M_X64) */

	return (_Traits::_Allocate_aligned(_Bytes, _Passed_align));
}

template<size_t _Align,
	enable_if_t<(_Align > __STDCPP_DEFAULT_NEW_ALIGNMENT__), int> = 0> inline
	void _Deallocate(void * _Ptr, const size_t _Bytes)
{	// deallocate storage allocated by _Allocate when _HAS_ALIGNED_NEW && _Align > __STDCPP_DEFAULT_NEW_ALIGNMENT__
	size_t _Passed_align = _Align;
#if defined(_M_IX86) || defined(_M_X64)
	if (_Bytes >= _Big_allocation_threshold)
	{	// boost the alignment of big allocations to help autovectorization
		_Passed_align = _Max_value(_Align, _Big_allocation_alignment);
	}
#endif /* defined(_M_IX86) || defined(_M_X64) */

	::operator delete(_Ptr, _Bytes, align_val_t{ _Passed_align });
}
#endif /* _HAS_ALIGNED_NEW */

template<size_t _Align,
	class _Traits = _Default_allocate_traits,
	enable_if_t<(!_HAS_ALIGNED_NEW || _Align <= __STDCPP_DEFAULT_NEW_ALIGNMENT__), int> = 0> inline
	_DECLSPEC_ALLOCATOR void *_Allocate(const size_t _Bytes)
{	// allocate _Bytes when !_HAS_ALIGNED_NEW || _Align <= __STDCPP_DEFAULT_NEW_ALIGNMENT__
#if defined(_M_IX86) || defined(_M_X64)
	if (_Bytes >= _Big_allocation_threshold)
	{	// boost the alignment of big allocations to help autovectorization
		return (_Allocate_manually_vector_aligned<_Traits>(_Bytes));
	}
#endif /* defined(_M_IX86) || defined(_M_X64) */

	if (_Bytes != 0)
	{
		return (_Traits::_Allocate(_Bytes));
	}

	return (nullptr);
}

template<size_t _Align,
	enable_if_t<(!_HAS_ALIGNED_NEW || _Align <= __STDCPP_DEFAULT_NEW_ALIGNMENT__), int> = 0> inline
	void _Deallocate(void * _Ptr, size_t _Bytes)
{	// deallocate storage allocated by _Allocate when !_HAS_ALIGNED_NEW || _Align <= __STDCPP_DEFAULT_NEW_ALIGNMENT__
#if defined(_M_IX86) || defined(_M_X64)
	if (_Bytes >= _Big_allocation_threshold)
	{	// boost the alignment of big allocations to help autovectorization
		_Adjust_manually_vector_aligned(_Ptr, _Bytes);
	}
#endif /* defined(_M_IX86) || defined(_M_X64) */

	::operator delete(_Ptr, _Bytes);
}

// FUNCTION TEMPLATE _Construct_in_place
template<class _Ty,
	class... _Types> inline
	void _Construct_in_place(_Ty& _Obj, _Types&&... _Args)
	_NOEXCEPT_COND(is_nothrow_constructible_v<_Ty, _Types...>)
{	// invoke True Placement New to initialize the referenced object with _Args...
	::new (const_cast<void *>(static_cast<const volatile void *>(_STD addressof(_Obj))))
		_Ty(_STD forward<_Types>(_Args)...);
}

// FUNCTION TEMPLATE _Global_new
template<class _Ty,
	class... _Types> inline
	_Ty * _Global_new(_Types&&... _Args)
{	// acts as "new" while disallowing user overload selection
	void * const _Result = _Allocate<_New_alignof<_Ty>>(sizeof(_Ty));
	_TRY_BEGIN
		::new (_Result) _Ty(_STD forward<_Types>(_Args)...);
	_CATCH_ALL
		_Deallocate<_New_alignof<_Ty>>(_Result, sizeof(_Ty));
	_RERAISE;
	_CATCH_END
		return (static_cast<_Ty *>(_Result));
}

// STRUCT TEMPLATE _Get_first_parameter
template<class _Ty>
struct _Get_first_parameter;

template<template<class, class...> class _Ty,
	class _First,
	class... _Rest>
	struct _Get_first_parameter<_Ty<_First, _Rest...>>
{	// given _Ty<_First, _Rest...>, extract _First
	using type = _First;
};

// STRUCT TEMPLATE _Replace_first_parameter
template<class _Newfirst,
	class _Ty>
	struct _Replace_first_parameter;

template<class _Newfirst,
	template<class, class...> class _Ty,
	class _First,
	class... _Rest>
	struct _Replace_first_parameter<_Newfirst, _Ty<_First, _Rest...>>
{	// given _Ty<_First, _Rest...>, replace _First
	using type = _Ty<_Newfirst, _Rest...>;
};

// STRUCT TEMPLATE _Get_element_type
template<class _Ty,
	class = void>
	struct _Get_element_type
{	// provide fallback
	using type = typename _Get_first_parameter<_Ty>::type;
};

template<class _Ty>
struct _Get_element_type<_Ty, void_t<typename _Ty::element_type>>
{	// get _Ty::element_type
	using type = typename _Ty::element_type;
};

// STRUCT TEMPLATE _Get_ptr_difference_type
template<class _Ty,
	class = void>
	struct _Get_ptr_difference_type
{	// provide fallback
	using type = ptrdiff_t;
};

template<class _Ty>
struct _Get_ptr_difference_type<_Ty, void_t<typename _Ty::difference_type>>
{	// get _Ty::difference_type
	using type = typename _Ty::difference_type;
};

// STRUCT TEMPLATE _Get_rebind_alias
template<class _Ty,
	class _Other,
	class = void>
	struct _Get_rebind_alias
{	// provide fallback
	using type = typename _Replace_first_parameter<_Other, _Ty>::type;
};

template<class _Ty,
	class _Other>
	struct _Get_rebind_alias<_Ty, _Other, void_t<typename _Ty::template rebind<_Other>>>
{	// get _Ty::rebind<_Other>
	using type = typename _Ty::template rebind<_Other>;
};

// STRUCT TEMPLATE pointer_traits
template<class _Ty>
struct pointer_traits
{	// defines traits for arbitrary pointers
	using element_type = typename _Get_element_type<_Ty>::type;
	using pointer = _Ty;
	using difference_type = typename _Get_ptr_difference_type<_Ty>::type;

	template<class _Other>
	using rebind = typename _Get_rebind_alias<_Ty, _Other>::type;

	using _Reftype = conditional_t<is_void_v<element_type>,
		char&,
		add_lvalue_reference_t<element_type>>;

	_NODISCARD static pointer pointer_to(_Reftype _Val)
	{	// convert raw reference to pointer
		return (_Ty::pointer_to(_Val));
	}
};

template<class _Ty>
struct pointer_traits<_Ty *>
{	// defines traits for raw pointers
	using element_type = _Ty;
	using pointer = _Ty * ;
	using difference_type = ptrdiff_t;

	template<class _Other>
	using rebind = _Other * ;

	using _Reftype = conditional_t<is_void_v<_Ty>,
		char&,
		add_lvalue_reference_t<_Ty>>;

	_NODISCARD static pointer pointer_to(_Reftype _Val)
	{	// convert raw reference to pointer
		return (_STD addressof(_Val));
	}
};

// ALIAS TEMPLATE _Rebind_pointer_t
template<class _Ptr,
	class _Ty>
	using _Rebind_pointer_t = typename pointer_traits<_Ptr>::template rebind<_Ty>;

// FUNCTION TEMPLATE _Refancy
template<class _Pointer,
	enable_if_t<!is_pointer_v<_Pointer>, int> = 0> inline
	_Pointer _Refancy(typename pointer_traits<_Pointer>::element_type * _Ptr)
{	// transform a plain pointer into a fancy pointer
	return (pointer_traits<_Pointer>::pointer_to(*_Ptr));
}

template<class _Pointer,
	enable_if_t<is_pointer_v<_Pointer>, int> = 0> inline
	_Pointer _Refancy(_Pointer _Ptr)
{	// do nothing for plain pointers
	return (_Ptr);
}

// FUNCTION TEMPLATE _Destroy_in_place
template<class _Ty> inline
void _Destroy_in_place(_Ty& _Obj) noexcept
{	// destroy the referenced object
	_Obj.~_Ty();
}

// FUNCTION TEMPLATE _Const_cast
template<class _Ptrty> inline
auto _Const_cast(_Ptrty _Ptr)
{	// remove constness from a fancy pointer
	using _Elem = typename pointer_traits<_Ptrty>::element_type;
	using _Modifiable = remove_const_t<_Elem>;
	using _Dest = typename pointer_traits<_Ptrty>::template rebind<_Modifiable>;

	return (pointer_traits<_Dest>::pointer_to(const_cast<_Modifiable&>(*_Ptr)));
}

template<class _Ty> inline
auto _Const_cast(_Ty * _Ptr)
{	// remove constness from a plain pointer
	return (const_cast<remove_const_t<_Ty> *>(_Ptr));
}

// STRUCT TEMPLATE _Get_pointer_type
template<class _Ty,
	class = void>
	struct _Get_pointer_type
{	// provide fallback
	using type = typename _Ty::value_type *;
};

#pragma warning(push)
#pragma warning(disable: 4996)	// was declared deprecated
template<class _Ty>
struct _Get_pointer_type<_Ty, void_t<typename _Ty::pointer>>
{	// get _Ty::pointer
	using type = typename _Ty::pointer;
};
#pragma warning(pop)

// STRUCT TEMPLATE _Get_const_pointer_type
template<class _Ty,
	class = void>
	struct _Get_const_pointer_type
{	// provide fallback
	using _Ptrty = typename _Get_pointer_type<_Ty>::type;
	using _Valty = typename _Ty::value_type;
	using type = typename pointer_traits<_Ptrty>::template rebind<const _Valty>;
};

#pragma warning(push)
#pragma warning(disable: 4996)	// was declared deprecated
template<class _Ty>
struct _Get_const_pointer_type<_Ty, void_t<typename _Ty::const_pointer>>
{	// get _Ty::const_pointer
	using type = typename _Ty::const_pointer;
};
#pragma warning(pop)

// STRUCT TEMPLATE _Get_void_pointer_type
template<class _Ty,
	class = void>
	struct _Get_void_pointer_type
{	// provide fallback
	using _Ptrty = typename _Get_pointer_type<_Ty>::type;
	using type = typename pointer_traits<_Ptrty>::template rebind<void>;
};

template<class _Ty>
struct _Get_void_pointer_type<_Ty, void_t<typename _Ty::void_pointer>>
{	// get _Ty::void_pointer
	using type = typename _Ty::void_pointer;
};

// STRUCT TEMPLATE _Get_const_void_pointer_type
template<class _Ty,
	class = void>
	struct _Get_const_void_pointer_type
{	// provide fallback
	using _Ptrty = typename _Get_pointer_type<_Ty>::type;
	using type = typename pointer_traits<_Ptrty>::template rebind<const void>;
};

template<class _Ty>
struct _Get_const_void_pointer_type<_Ty, void_t<typename _Ty::const_void_pointer>>
{	// get _Ty::const_void_pointer
	using type = typename _Ty::const_void_pointer;
};

// STRUCT TEMPLATE _Get_difference_type
template<class _Ty,
	class = void>
	struct _Get_difference_type
{	// provide fallback
	using _Ptrty = typename _Get_pointer_type<_Ty>::type;
	using type = typename pointer_traits<_Ptrty>::difference_type;
};

#pragma warning(push)
#pragma warning(disable: 4996)	// was declared deprecated
template<class _Ty>
struct _Get_difference_type<_Ty, void_t<typename _Ty::difference_type>>
{	// get _Ty::difference_type
	using type = typename _Ty::difference_type;
};
#pragma warning(pop)

// STRUCT TEMPLATE _Get_size_type
template<class _Ty,
	class = void>
	struct _Get_size_type
{	// provide fallback
	using type = make_unsigned_t<typename _Get_difference_type<_Ty>::type>;
};

#pragma warning(push)
#pragma warning(disable: 4996)	// was declared deprecated
template<class _Ty>
struct _Get_size_type<_Ty, void_t<typename _Ty::size_type>>
{	// get _Ty::size_type
	using type = typename _Ty::size_type;
};
#pragma warning(pop)

// STRUCT TEMPLATE _Get_propagate_on_container_copy
template<class _Ty,
	class = void>
	struct _Get_propagate_on_container_copy
{	// provide fallback
	using type = false_type;
};

template<class _Ty>
struct _Get_propagate_on_container_copy<_Ty, void_t<typename _Ty::propagate_on_container_copy_assignment>>
{	// get _Ty::propagate_on_container_copy_assignment
	using type = typename _Ty::propagate_on_container_copy_assignment;
};

// STRUCT TEMPLATE _Get_propagate_on_container_move
template<class _Ty,
	class = void>
	struct _Get_propagate_on_container_move
{	// provide fallback
	using type = false_type;
};

template<class _Ty>
struct _Get_propagate_on_container_move<_Ty, void_t<typename _Ty::propagate_on_container_move_assignment>>
{	// get _Ty::propagate_on_container_move_assignment
	using type = typename _Ty::propagate_on_container_move_assignment;
};

// STRUCT TEMPLATE _Get_propagate_on_container_swap
template<class _Ty,
	class = void>
	struct _Get_propagate_on_container_swap
{	// provide fallback
	using type = false_type;
};

template<class _Ty>
struct _Get_propagate_on_container_swap<_Ty, void_t<typename _Ty::propagate_on_container_swap>>
{	// get _Ty::propagate_on_container_swap
	using type = typename _Ty::propagate_on_container_swap;
};

// STRUCT TEMPLATE _Get_is_always_equal
template<class _Ty,
	class = void>
	struct _Get_is_always_equal
{	// provide fallback
	using type = typename is_empty<_Ty>::type;
};

template<class _Ty>
struct _Get_is_always_equal<_Ty, void_t<typename _Ty::is_always_equal>>
{	// get _Ty::is_always_equal
	using type = typename _Ty::is_always_equal;
};

// STRUCT TEMPLATE _Get_rebind_type
template<class _Ty,
	class _Other,
	class = void>
	struct _Get_rebind_type
{	// provide fallback
	using type = typename _Replace_first_parameter<_Other, _Ty>::type;
};

#pragma warning(push)
#pragma warning(disable: 4996)	// was declared deprecated
template<class _Ty,
	class _Other>
	struct _Get_rebind_type<_Ty, _Other, void_t<typename _Ty::template rebind<_Other>::other>>
{	// get _Ty::rebind<_Other>::other
	using type = typename _Ty::template rebind<_Other>::other;
};
#pragma warning(pop)

// STRUCT TEMPLATE _Is_default_allocator
template<class _Ty>
class allocator;

template<class _Alloc,
	class = void>
	struct _Is_default_allocator
	: false_type
{	// tests whether _Alloc is non-specialized default allocator (N4659 23.10.9 [default.allocator])
};

template<class _Ty>
struct _Is_default_allocator<allocator<_Ty>, typename allocator<_Ty>::_Not_user_specialized>
	: true_type
{	// tests whether _Alloc is non-specialized default allocator (N4659 23.10.9 [default.allocator])
};

// ALIAS TEMPLATES _Uses_default_construct AND _Uses_default_construct_t
template<class _Void,
	class... _Types>
	struct _Has_no_alloc_construct
	: true_type
{	// determines whether _Alloc has no construct
};

#pragma warning(push)
#pragma warning(disable: 4996)	// was declared deprecated
template<class _Alloc,
	class _Ptr,
	class... _Args>
	struct _Has_no_alloc_construct<void_t<
	decltype(_STD declval<_Alloc&>().construct(_STD declval<_Ptr>(), _STD declval<_Args>()...))>,
	_Alloc, _Ptr, _Args...>
	: false_type
{	// determines whether _Alloc has no construct
};
#pragma warning(pop)

template<class _Alloc,
	class _Ptr,
	class... _Args>
	using _Uses_default_construct = disjunction<
	_Is_default_allocator<_Alloc>,
	_Has_no_alloc_construct<void, _Alloc, _Ptr, _Args...>>;

template<class _Alloc,
	class _Ptr,
	class... _Args>
	using _Uses_default_construct_t = typename _Uses_default_construct<_Alloc, _Ptr, _Args...>::type;


// ALIAS TEMPLATE _Uses_default_destroy AND _Uses_default_destroy_t
template<class _Alloc,
	class _Ptr,
	class = void>
	struct _Has_no_alloc_destroy
	: true_type
{	// determines whether _Alloc has no destroy
};

#pragma warning(push)
#pragma warning(disable: 4996)	// was declared deprecated
template<class _Alloc,
	class _Ptr>
	struct _Has_no_alloc_destroy<_Alloc, _Ptr, void_t<decltype(_STD declval<_Alloc&>().destroy(_STD declval<_Ptr>()))>>
	: false_type
{	// determines whether _Alloc has no destroy
};
#pragma warning(pop)

template<class _Alloc,
	class _Ptr>
	using _Uses_default_destroy = disjunction<
	_Is_default_allocator<_Alloc>,
	_Has_no_alloc_destroy<_Alloc, _Ptr>>;

template<class _Alloc,
	class _Ptr>
	using _Uses_default_destroy_t = typename _Uses_default_destroy<_Alloc, _Ptr>::type;


// STRUCT TEMPLATE _Has_allocate_hint
template<class _Alloc,
	class _Size_type,
	class _Const_void_pointer,
	class = void>
	struct _Has_allocate_hint
	: false_type
{	// determines whether _Alloc has allocate(n, hint)
};

#pragma warning(push)
#pragma warning(disable: 4996)	// was declared deprecated
template<class _Alloc,
	class _Size_type,
	class _Const_void_pointer>
	struct _Has_allocate_hint<_Alloc, _Size_type, _Const_void_pointer, void_t<
	decltype(_STD declval<_Alloc&>().allocate(
		_STD declval<const _Size_type&>(), _STD declval<const _Const_void_pointer&>()))>>
	: true_type
{	// determines whether _Alloc has allocate(n, hint)
};
#pragma warning(pop)

// STRUCT TEMPLATE _Has_max_size
template<class _Alloc,
	class = void>
	struct _Has_max_size
	: false_type
{	// determines whether _Alloc has max_size()
};

#pragma warning(push)
#pragma warning(disable: 4996)	// was declared deprecated
template<class _Alloc>
struct _Has_max_size<_Alloc, void_t<decltype(_STD declval<const _Alloc&>().max_size())>>
	: true_type
{	// determines whether _Alloc has max_size()
};
#pragma warning(pop)

// STRUCT TEMPLATE _Has_select_on_container_copy_construction
template<class _Alloc,
	class = void>
	struct _Has_select_on_container_copy_construction
	: false_type
{	// determines whether _Alloc has select_on_container_copy_construction()
};

template<class _Alloc>
struct _Has_select_on_container_copy_construction<_Alloc, void_t<
	decltype(_STD declval<const _Alloc&>().select_on_container_copy_construction())>>
	: true_type
{	// determines whether _Alloc has select_on_container_copy_construction()
};


// STRUCT TEMPLATE allocator_traits
template<class _Alloc>
struct allocator_traits;

#pragma warning(push)
#pragma warning(disable: 4996)	// was declared deprecated
template<class _Alloc>
struct _Normal_allocator_traits
{	// defines traits for allocators
	using allocator_type = _Alloc;
	using value_type = typename _Alloc::value_type;

	using pointer = typename _Get_pointer_type<_Alloc>::type;
	using const_pointer = typename _Get_const_pointer_type<_Alloc>::type;
	using void_pointer = typename _Get_void_pointer_type<_Alloc>::type;
	using const_void_pointer = typename _Get_const_void_pointer_type<_Alloc>::type;

	using size_type = typename _Get_size_type<_Alloc>::type;
	using difference_type = typename _Get_difference_type<_Alloc>::type;

	using propagate_on_container_copy_assignment = typename _Get_propagate_on_container_copy<_Alloc>::type;
	using propagate_on_container_move_assignment = typename _Get_propagate_on_container_move<_Alloc>::type;
	using propagate_on_container_swap = typename _Get_propagate_on_container_swap<_Alloc>::type;
	using is_always_equal = typename _Get_is_always_equal<_Alloc>::type;

	template<class _Other>
	using rebind_alloc = typename _Get_rebind_type<_Alloc, _Other>::type;

	template<class _Other>
	using rebind_traits = allocator_traits<rebind_alloc<_Other>>;

	_NODISCARD static _DECLSPEC_ALLOCATOR pointer allocate(_Alloc& _Al, _CRT_GUARDOVERFLOW const size_type _Count)
	{	// allocate array of _Count elements
		return (_Al.allocate(_Count));
	}

	static _DECLSPEC_ALLOCATOR pointer _Allocate1(_Alloc& _Al, _CRT_GUARDOVERFLOW const size_type _Count,
		const const_void_pointer _Hint, true_type)
	{	// allocate array of _Count elements, with hint, allocator-supplied version
		return (_Al.allocate(_Count, _Hint));
	}

	static _DECLSPEC_ALLOCATOR pointer _Allocate1(_Alloc& _Al, _CRT_GUARDOVERFLOW const size_type _Count,
		const_void_pointer, false_type)
	{	// allocate array of _Count elements, with hint, default version
		return (_Al.allocate(_Count));
	}

	_NODISCARD static _DECLSPEC_ALLOCATOR pointer allocate(_Alloc& _Al, _CRT_GUARDOVERFLOW const size_type _Count,
		const const_void_pointer _Hint)
	{	// allocate array of _Count elements, with hint
		return (_Allocate1(_Al, _Count, _Hint, _Has_allocate_hint<_Alloc, size_type, const_void_pointer>{}));
	}

	static void deallocate(_Alloc& _Al, pointer _Ptr, size_type _Count)
	{	// deallocate _Count elements at _Ptr
		_Al.deallocate(_Ptr, _Count);
	}

	template<class _Ty,
		class... _Types>
		static void _Construct1(true_type, _Alloc&, _Ty *_Ptr, _Types&&... _Args)
	{	// construct _Ty(_Types...) at _Ptr, default version
		::new (static_cast<void *>(_Ptr)) _Ty(_STD forward<_Types>(_Args)...);
	}

	template<class _Ty,
		class... _Types>
		static void _Construct1(false_type, _Alloc& _Al, _Ty *_Ptr, _Types&&... _Args)
	{	// construct _Ty(_Types...) at _Ptr, allocator-supplied version
		_Al.construct(_Ptr, _STD forward<_Types>(_Args)...);
	}

	template<class _Ty,
		class... _Types>
		static void construct(_Alloc& _Al, _Ty *_Ptr, _Types&&... _Args)
	{	// construct _Ty(_Types...) at _Ptr
		_Construct1(_Uses_default_construct_t<_Alloc, _Ty *, _Types...>(),
			_Al, _Ptr, _STD forward<_Types>(_Args)...);
	}

	template<class _Ty>
	static void _Destroy1(_Alloc&, _Ty *_Ptr, true_type)
	{	// destroy object at _Ptr, default version
		_Ptr->~_Ty();
	}

	template<class _Ty>
	static void _Destroy1(_Alloc& _Al, _Ty *_Ptr, false_type)
	{	// destroy object at _Ptr, allocator-supplied version
		_Al.destroy(_Ptr);
	}

	template<class _Ty>
	static void destroy(_Alloc& _Al, _Ty *_Ptr)
	{	// destroy object at _Ptr
		_Destroy1(_Al, _Ptr, _Uses_default_destroy_t<_Alloc, _Ty *>());
	}

	static size_type _Max_size1(const _Alloc& _Al, true_type) noexcept
	{	// get maximum size, allocator-supplied version
		return (_Al.max_size());
	}

	static size_type _Max_size1(const _Alloc&, false_type) noexcept
	{	// get maximum size, default version
		return ((numeric_limits<size_type>::max)() / sizeof(value_type));
	}

	_NODISCARD static size_type max_size(const _Alloc& _Al) noexcept
	{	// get maximum size
		return (_Max_size1(_Al, _Has_max_size<_Alloc>{}));
	}

	static _Alloc _Select_on_container_copy_construction1(const _Alloc& _Al, true_type)
	{	// get allocator to use, allocator-supplied version
		return (_Al.select_on_container_copy_construction());
	}

	static _Alloc _Select_on_container_copy_construction1(const _Alloc& _Al, false_type)
	{	// get allocator to use, default version
		return (_Al);
	}

	_NODISCARD static _Alloc select_on_container_copy_construction(const _Alloc& _Al)
	{	// get allocator to use
		return (_Select_on_container_copy_construction1(_Al, _Has_select_on_container_copy_construction<_Alloc>{}));
	}
};
#pragma warning(pop)

template<class _Alloc>
struct _Default_allocator_traits
{	// traits for std::allocator
	using allocator_type = _Alloc;
	using value_type = typename _Alloc::value_type;

	using pointer = value_type * ;
	using const_pointer = const value_type *;
	using void_pointer = void *;
	using const_void_pointer = const void *;

	using size_type = size_t;
	using difference_type = ptrdiff_t;

	using propagate_on_container_copy_assignment = false_type;
	using propagate_on_container_move_assignment = true_type;
	using propagate_on_container_swap = false_type;
	using is_always_equal = true_type;

	template<class _Other>
	using rebind_alloc = allocator<_Other>;

	template<class _Other>
	using rebind_traits = allocator_traits<allocator<_Other>>;

	_NODISCARD static _DECLSPEC_ALLOCATOR pointer allocate(_Alloc&, _CRT_GUARDOVERFLOW const size_type _Count)
	{	// allocate array of _Count elements
		return (static_cast<pointer>(_Allocate<_New_alignof<value_type>>(_Get_size_of_n<sizeof(value_type)>(_Count))));
	}

	_NODISCARD static _DECLSPEC_ALLOCATOR pointer allocate(_Alloc&, _CRT_GUARDOVERFLOW const size_type _Count,
		const_void_pointer)
	{	// allocate array of _Count elements, with hint
		return (static_cast<pointer>(_Allocate<_New_alignof<value_type>>(_Get_size_of_n<sizeof(value_type)>(_Count))));
	}

	static void deallocate(_Alloc&, const pointer _Ptr, const size_type _Count)
	{	// deallocate _Count elements at _Ptr
	// no overflow check on the following multiply; we assume _Allocate did that check
		_Deallocate<_New_alignof<value_type>>(_Ptr, sizeof(value_type) * _Count);
	}

	template<class _Objty,
		class... _Types>
		static void construct(_Alloc&, _Objty * const _Ptr, _Types&&... _Args)
	{	// construct _Objty(_Types...) at _Ptr
		::new (const_cast<void *>(static_cast<const volatile void *>(_Ptr)))
			_Objty(_STD forward<_Types>(_Args)...);
	}

	template<class _Uty>
	static void destroy(_Alloc&, _Uty * const _Ptr)
	{	// destroy object at _Ptr
		_Ptr->~_Uty();
	}

	_NODISCARD static size_type max_size(const _Alloc&) noexcept
	{	// get maximum size
		return (static_cast<size_t>(-1) / sizeof(value_type));
	}

	_NODISCARD static _Alloc select_on_container_copy_construction(const _Alloc& _Al)
	{	// get allocator to use
		return (_Al);
	}
};

template<class _Alloc>
struct allocator_traits
	: conditional_t<_Is_default_allocator<_Alloc>::value,
	_Default_allocator_traits<_Alloc>, _Normal_allocator_traits<_Alloc>>
{	// defines traits for allocators
};

// TYPE TRAIT _Always_equal_after_move
template<class _Alloc>
_INLINE_VAR constexpr bool _Always_equal_after_move = allocator_traits<_Alloc>::is_always_equal::value
|| allocator_traits<_Alloc>::propagate_on_container_move_assignment::value;

// ALIAS TEMPLATE _Rebind_alloc_t
template<class _Alloc,
	class _Value_type>
	using _Rebind_alloc_t = typename allocator_traits<_Alloc>::template rebind_alloc<_Value_type>;

// VARIABLE TEMPLATE _Is_simple_alloc_v
template<class _Alloc>	// tests if allocator has simple addressing
_INLINE_VAR constexpr bool _Is_simple_alloc_v =
is_same_v<typename allocator_traits<_Alloc>::size_type, size_t>
&& is_same_v<typename allocator_traits<_Alloc>::difference_type, ptrdiff_t>
&& is_same_v<typename allocator_traits<_Alloc>::pointer, typename _Alloc::value_type *>
&& is_same_v<typename allocator_traits<_Alloc>::const_pointer, const typename _Alloc::value_type *>;

// STRUCT TEMPLATE _Simple_types
template<class _Value_type>
struct _Simple_types
{	// wraps types from allocators with simple addressing for use in iterators
	// and other SCARY machinery
	using value_type = _Value_type;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using pointer = value_type * ;
	using const_pointer = const value_type *;
};

// CLASS TEMPLATE allocator
template<class _Ty>
class allocator
{	// generic allocator for objects of class _Ty
public:
	static_assert(!is_const_v<_Ty>,
		"The C++ Standard forbids containers of const elements "
		"because allocator<const T> is ill-formed.");

	using _Not_user_specialized = void;

	using value_type = _Ty;

	_CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS typedef _Ty * pointer;
	_CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS typedef const _Ty * const_pointer;

	_CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS typedef _Ty& reference;
	_CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS typedef const _Ty& const_reference;

	_CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS typedef size_t size_type;
	_CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS typedef ptrdiff_t difference_type;

	using propagate_on_container_move_assignment = true_type;
	using is_always_equal = true_type;

	template<class _Other>
	struct _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS rebind
	{	// convert this type to allocator<_Other>
		using other = allocator<_Other>;
	};

	_NODISCARD _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS _Ty * address(_Ty& _Val) const noexcept
	{	// return address of mutable _Val
		return (_STD addressof(_Val));
	}

	_NODISCARD _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS const _Ty * address(const _Ty& _Val) const noexcept
	{	// return address of nonmutable _Val
		return (_STD addressof(_Val));
	}

	constexpr allocator() noexcept
	{	// construct default allocator (do nothing)
	}

	constexpr allocator(const allocator&) noexcept = default;
	template<class _Other>
	constexpr allocator(const allocator<_Other>&) noexcept
	{	// construct from a related allocator (do nothing)
	}

	void deallocate(_Ty * const _Ptr, const size_t _Count)
	{	// deallocate object at _Ptr
	// no overflow check on the following multiply; we assume _Allocate did that check
		_Deallocate<_New_alignof<_Ty>>(_Ptr, sizeof(_Ty) * _Count);
	}

	_NODISCARD _DECLSPEC_ALLOCATOR _Ty * allocate(_CRT_GUARDOVERFLOW const size_t _Count)
	{	// allocate array of _Count elements
		return (static_cast<_Ty *>(_Allocate<_New_alignof<_Ty>>(_Get_size_of_n<sizeof(_Ty)>(_Count))));
	}

	_NODISCARD _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS _DECLSPEC_ALLOCATOR _Ty * allocate(
		_CRT_GUARDOVERFLOW const size_t _Count, const void *)
	{	// allocate array of _Count elements, ignore hint
		return (allocate(_Count));
	}

	template<class _Objty,
		class... _Types>
		_CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS void construct(_Objty * const _Ptr, _Types&&... _Args)
	{	// construct _Objty(_Types...) at _Ptr
		::new (const_cast<void *>(static_cast<const volatile void *>(_Ptr)))
			_Objty(_STD forward<_Types>(_Args)...);
	}

	template<class _Uty>
	_CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS void destroy(_Uty * const _Ptr)
	{	// destroy object at _Ptr
		_Ptr->~_Uty();
	}

	_NODISCARD _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS size_t max_size() const noexcept
	{	// estimate maximum array size
		return (static_cast<size_t>(-1) / sizeof(_Ty));
	}
};

// CLASS allocator<void>
template<>
class _CXX17_DEPRECATE_ALLOCATOR_VOID allocator<void>
{	// generic allocator for type void
public:
	using value_type = void;
	using pointer = void *;
	using const_pointer = const void *;

	template<class _Other>
	struct rebind
	{	// convert this type to an allocator<_Other>
		using other = allocator<_Other>;
	};
};

template<class _Ty,
	class _Other>
	_NODISCARD inline bool operator==(const allocator<_Ty>&,
		const allocator<_Other>&) noexcept
{	// test for allocator equality
	return (true);
}

template<class _Ty,
	class _Other>
	_NODISCARD inline bool operator!=(const allocator<_Ty>&,
		const allocator<_Other>&) noexcept
{	// test for allocator inequality
	return (false);
}

#if _HAS_CXX17
// ALIAS TEMPLATE _Guide_size_type_t FOR DEDUCTION GUIDES, N4687 26.5.4.1 [unord.map.overview]/4
template<class _Alloc>
using _Guide_size_type_t = typename allocator_traits<
	conditional_t<_Is_allocator<_Alloc>::value, _Alloc, allocator<int>>
>::size_type;
#endif /* _HAS_CXX17 */

// FUNCTION TEMPLATE _Pocca
template<class _Alloc> inline
void _Pocca(_Alloc& _Left, const _Alloc& _Right, true_type) noexcept
{	// propagate on container copy assignment
	_Left = _Right;
}

template<class _Alloc> inline
void _Pocca(_Alloc&, const _Alloc&, false_type) noexcept
{	// (don't) propagate on container copy assignment
}

template<class _Alloc> inline
void _Pocca(_Alloc& _Left, const _Alloc& _Right) noexcept
{	// (maybe) propagate on container copy assignment
	typename allocator_traits<_Alloc>::propagate_on_container_copy_assignment _Tag;
	_Pocca(_Left, _Right, _Tag);
}

// FUNCTION TEMPLATE _Pocma
template<class _Alloc> inline
void _Pocma(_Alloc& _Left, _Alloc& _Right, true_type) noexcept
{	// propagate on container move assignment
	_Left = _STD move(_Right);
}

template<class _Alloc> inline
void _Pocma(_Alloc&, _Alloc&, false_type) noexcept
{	// (don't) propagate on container move assignment
}

template<class _Alloc> inline
void _Pocma(_Alloc& _Left, _Alloc& _Right) noexcept
{	// (maybe) propagate on container move assignment
	typename allocator_traits<_Alloc>::propagate_on_container_move_assignment _Tag;
	_Pocma(_Left, _Right, _Tag);
}

// FUNCTION TEMPLATE _Pocs
template<class _Alloc> inline
void _Pocs(_Alloc& _Left, _Alloc& _Right, true_type) noexcept
{	// propagate on container swap
	_Swap_adl(_Left, _Right);
}

template<class _Alloc> inline
void _Pocs(_Alloc& _Left, _Alloc& _Right, false_type) noexcept
{	// (don't) propagate on container swap
	_STL_ASSERT(_Left == _Right, "containers incompatible for swap");
	(void)_Left;
	(void)_Right;
}

template<class _Alloc> inline
void _Pocs(_Alloc& _Left, _Alloc& _Right) noexcept
{	// (maybe) propagate on container swap
	typename allocator_traits<_Alloc>::propagate_on_container_swap _Tag;
	_Pocs(_Left, _Right, _Tag);
}


// FUNCTION TEMPLATE _Destroy_range WITH ALLOC
template<class _Alloc> inline
void _Destroy_range1(
	typename allocator_traits<_Alloc>::pointer _First,
	typename allocator_traits<_Alloc>::pointer _Last,
	_Alloc& _Al, false_type)
{	// destroy [_First, _Last), no special optimization
	for (; _First != _Last; ++_First)
	{
		allocator_traits<_Alloc>::destroy(_Al, _Unfancy(_First));
	}
}

template<class _Alloc> inline
void _Destroy_range1(
	typename allocator_traits<_Alloc>::pointer,
	typename allocator_traits<_Alloc>::pointer,
	_Alloc&, true_type)
{	// destroy [_First, _Last), trivially destructible and default destroy
	// nothing to do
}

template<class _Alloc> inline
void _Destroy_range(
	typename allocator_traits<_Alloc>::pointer _First,
	typename allocator_traits<_Alloc>::pointer _Last,
	_Alloc& _Al)
{	// destroy [_First, _Last), choose optimization
	// note that this is an optimization for debug mode codegen;
	// in release mode the BE removes all of this
	using _Val = typename _Alloc::value_type;
	_Destroy_range1(_First, _Last, _Al, bool_constant<conjunction_v<
		is_trivially_destructible<_Val>,
		_Uses_default_destroy<_Alloc, _Val *>>>{});
}


// FUNCTION TEMPLATE _Destroy_range
template<class _FwdIt> inline
void _Destroy_range1(_FwdIt _First, _FwdIt _Last, false_type)
{	// destroy [_First, _Last), no special optimization
	for (; _First != _Last; ++_First)
	{
		_Destroy_in_place(*_First);
	}
}

template<class _FwdIt> inline
void _Destroy_range1(_FwdIt, _FwdIt, true_type)
{	// destroy [_First, _Last), trivially destructible
	// nothing to do
}

template<class _FwdIt> inline
void _Destroy_range(_FwdIt _First, _FwdIt _Last)
{	// destroy [_First, _Last), choose optimization
	// note that this is an optimization for debug mode codegen;
	// in release mode the BE removes all of this
	_Destroy_range1(_First, _Last, is_trivially_destructible<_Iter_value_t<_FwdIt>>());
}


// FUNCTION TEMPLATE _Convert_size
template<class _Size_type> inline
_Size_type _Convert_size(const size_t _Len)
{	// convert size_t to _Size_type, avoiding truncation
	if (_Len > (numeric_limits<_Size_type>::max)())
	{
		_Xlength_error("size_t too long for _Size_type");
	}

	return (static_cast<_Size_type>(_Len));
}

template<> inline
size_t _Convert_size<size_t>(const size_t _Len)
{	// convert size_t to size_t, unchanged
	return (_Len);
}

// FUNCTION TEMPLATE _Deallocate_plain
template<class _Alloc,
	enable_if_t<is_same_v<typename allocator_traits<_Alloc>::pointer,
	typename _Alloc::value_type *>, int> = 0> inline
	void _Deallocate_plain(_Alloc& _Al, typename _Alloc::value_type * const _Ptr)
{	// deallocate a plain pointer using an allocator, non-fancy pointers special case
	allocator_traits<_Alloc>::deallocate(_Al, _Ptr, 1);
}

template<class _Alloc,
	enable_if_t<!is_same_v<typename allocator_traits<_Alloc>::pointer,
	typename _Alloc::value_type *>, int> = 0> inline
	void _Deallocate_plain(_Alloc& _Al, typename _Alloc::value_type * const _Ptr)
{	// deallocate a plain pointer using an allocator
	using _Alloc_traits = allocator_traits<_Alloc>;
	using _Ptr_traits = pointer_traits<typename _Alloc_traits::pointer>;
	_Alloc_traits::deallocate(_Al, _Ptr_traits::pointer_to(*_Ptr), 1);
}
_STD_END

// ATOMIC REFERENCE COUNTING PRIMITIVES
#include <xatomic0.h>

#if _USE_INTERLOCKED_REFCOUNTING
#include <intrin0.h>

#define _MT_INCR(x) \
	_InterlockedIncrement(reinterpret_cast<volatile long *>(&x))
#define _MT_DECR(x) \
	_InterlockedDecrement(reinterpret_cast<volatile long *>(&x))

#else /* _USE_INTERLOCKED_REFCOUNTING */
#include <xatomic.h>

#define _MT_INCR(x) \
	_Inc_atomic_counter_explicit(x, memory_order_relaxed)
#define _MT_DECR(x) \
	_Dec_atomic_counter_explicit(x, memory_order_acq_rel)
#endif /* _USE_INTERLOCKED_REFCOUNTING */

#pragma pop_macro("new")
_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)
#endif /* RC_INVOKED */
#endif /* _XMEMORY0_ */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0009 */
