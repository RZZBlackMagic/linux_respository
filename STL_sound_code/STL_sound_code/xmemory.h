// xmemory internal header
#pragma once
#ifndef _XMEMORY_
#define _XMEMORY_
#ifndef RC_INVOKED
#include <xmemory0>

#pragma pack(push,_CRT_PACKING)
#pragma warning(push,_STL_WARNING_LEVEL)
#pragma warning(disable: _STL_DISABLED_WARNINGS)
_STL_DISABLE_CLANG_WARNINGS
#pragma push_macro("new")
#undef new

_STD_BEGIN
// FUNCTION TEMPLATE _Get_temporary_buffer
template<class _Ty> inline
pair<_Ty *, ptrdiff_t> _Get_temporary_buffer(ptrdiff_t _Count) noexcept
{	// get raw temporary buffer of up to _Count elements
	if (static_cast<size_t>(_Count) <= static_cast<size_t>(-1) / sizeof(_Ty))
	{
		for (; 0 < _Count; _Count /= 2)
		{
			const auto _Size = static_cast<size_t>(_Count) * sizeof(_Ty);
			void * _Pbuf;
#if _HAS_ALIGNED_NEW
#pragma warning(push)
#pragma warning(disable: 4127) // conditional expression is constant
#pragma warning(disable: 6326) // Potential comparison of a constant with another constant
			if /* constexpr */ (alignof(_Ty) > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
			{
				_Pbuf = ::operator new(_Size, align_val_t{ alignof(_Ty) }, nothrow);
			}
			else
#pragma warning(pop)
#endif /* _HAS_ALIGNED_NEW */
			{
				_Pbuf = ::operator new(_Size, nothrow);
			}
			if (_Pbuf)
			{
				return { static_cast<_Ty *>(_Pbuf), _Count };
			}
		}
	}

	return { nullptr, 0 };
}

// FUNCTION TEMPLATE _Return_temporary_buffer
template<class _Ty> inline
void _Return_temporary_buffer(_Ty * const _Pbuf) noexcept
{	// delete raw temporary buffer
#if _HAS_ALIGNED_NEW
#pragma warning(push)
#pragma warning(disable: 4127) // conditional expression is constant
#pragma warning(disable: 6326) // Potential comparison of a constant with another constant
	if /* constexpr */ (alignof(_Ty) > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
	{
		::operator delete(_Pbuf, align_val_t{ alignof(_Ty) });
	}
	else
#pragma warning(pop)
#endif /* _HAS_ALIGNED_NEW */
	{
		::operator delete(_Pbuf);
	}
}

// STRUCT TEMPLATE _Uninitialized_backout
template<class _FwdIt>
struct _Uninitialized_backout
{	// struct to undo partially constructed ranges in _Uninitialized_xxx algorithms
	_FwdIt _First;
	_FwdIt _Last;

	explicit _Uninitialized_backout(_FwdIt _Dest)
		: _First(_Dest),
		_Last(_Dest)
	{}

	_Uninitialized_backout(_FwdIt _First_, _FwdIt _Last_)
		: _First(_First_),
		_Last(_Last_)
	{}

	_Uninitialized_backout(const _Uninitialized_backout&) = delete;
	_Uninitialized_backout& operator=(const _Uninitialized_backout&) = delete;

	~_Uninitialized_backout()
	{	// destroy all elements guarded by this instance
		_Destroy_range(_First, _Last);
	}

	template<class... _Types>
	void _Emplace_back(_Types&&... _Vals)
	{	// construct a new element at *_Last and increment
		_Construct_in_place(*_Last, _STD forward<_Types>(_Vals)...);
		++_Last;
	}

	_FwdIt _Release()
	{	// suppress any exception handling backout and return _Last
		_First = _Last;
		return (_Last);
	}
};

// FUNCTION TEMPLATE _Uninitialized_move_unchecked
template<class _InIt,
	class _FwdIt> inline
	_FwdIt _Uninitialized_move_unchecked1(_InIt _First, const _InIt _Last,
		const _FwdIt _Dest, _General_ptr_iterator_tag)
{	// move [_First, _Last) to raw [_Dest, ...), no special optimization
	_Uninitialized_backout<_FwdIt> _Backout{ _Dest };
	for (; _First != _Last; ++_First)
	{
		_Backout._Emplace_back(_STD move(*_First));
	}

	return (_Backout._Release());
}

template<class _InIt,
	class _FwdIt> inline
	_FwdIt _Uninitialized_move_unchecked1(const _InIt _First, const _InIt _Last,
		const _FwdIt _Dest, _Really_trivial_ptr_iterator_tag)
{	// move [_First, _Last) to raw [_Dest, ...), memmove optimization
	return (_Copy_memmove(_First, _Last, _Dest));
}

template<class _InIt,
	class _FwdIt> inline
	_FwdIt _Uninitialized_move_unchecked(const _InIt _First, const _InIt _Last,
		const _FwdIt _Dest)
{	// move [_First, _Last) to raw [_Dest, ...), choose optimization
	return (_Uninitialized_move_unchecked1(_First, _Last, _Dest, _Ptr_move_cat(_First, _Dest)));
}

// STRUCT TEMPLATE _Uninitialized_backout_al
template<class _FwdIt,
	class _Alloc>
	class _Uninitialized_backout_al
{	// struct to undo partially constructed ranges in _Uninitialized_xxx_al algorithms
public:
	_Uninitialized_backout_al(_FwdIt _Dest, _Alloc& _Al_)
		: _First(_Dest),
		_Last(_Dest),
		_Al(_Al_)
	{
	}

	_Uninitialized_backout_al(const _Uninitialized_backout_al&) = delete;
	_Uninitialized_backout_al& operator=(const _Uninitialized_backout_al&) = delete;

	~_Uninitialized_backout_al()
	{	// destroy all elements guarded by this instance
		_Destroy_range(_First, _Last, _Al);
	}

	template<class... _Types>
	void _Emplace_back(_Types&&... _Vals)
	{	// construct a new element at *_Last and increment
		allocator_traits<_Alloc>::construct(_Al, _Unfancy(_Last), _STD forward<_Types>(_Vals)...);
		++_Last;
	}

	_FwdIt _Release()
	{	// suppress any exception handling backout and return _Last
		_First = _Last;
		return (_Last);
	}

private:
	_FwdIt _First;
	_FwdIt _Last;
	_Alloc& _Al;
};

// FUNCTION TEMPLATE _Uninitialized_copy WITH ALLOCATOR
template<class _InIt,
	class _FwdIt,
	class _Alloc> inline
	_FwdIt _Uninitialized_copy_al_unchecked(_InIt _First, const _InIt _Last, const _FwdIt _Dest, _Alloc& _Al,
		_General_ptr_iterator_tag, _Any_tag)
{	// copy [_First, _Last) to raw _Dest, using _Al, no special optimization
	_Uninitialized_backout_al<_FwdIt, _Alloc> _Backout{ _Dest, _Al };
	for (; _First != _Last; ++_First)
	{
		_Backout._Emplace_back(*_First);
	}

	return (_Backout._Release());
}

template<class _Ty1,
	class _Ty2,
	class _Alloc> inline
	_Ty2 *_Uninitialized_copy_al_unchecked(_Ty1 * const _First, _Ty1 * const _Last, _Ty2 * const _Dest, _Alloc&,
		_Really_trivial_ptr_iterator_tag, true_type)
{	// copy [_First, _Last) to raw _Dest, using default _Alloc construct, memmove optimization
	return (_Copy_memmove(_First, _Last, _Dest));
}

template<class _InIt,
	class _FwdIt,
	class _Alloc> inline
	_FwdIt _Uninitialized_copy(const _InIt _First, const _InIt _Last, _FwdIt _Dest, _Alloc& _Al)
{	// copy [_First, _Last) to raw _Dest, using _Al
	// note: only called internally from elsewhere in the STL
	const auto _UFirst = _Get_unwrapped(_First);
	const auto _ULast = _Get_unwrapped(_Last);
	const auto _UDest = _Get_unwrapped_n(_Dest, _Idl_distance<_InIt>(_UFirst, _ULast));
	_Seek_wrapped(_Dest,
		_Uninitialized_copy_al_unchecked(_UFirst, _ULast, _UDest, _Al,
			_Ptr_copy_cat(_UFirst, _UDest),
			_Uses_default_construct_t<_Alloc, decltype(_Unfancy(_UDest)), decltype(*_UFirst)>()));

	return (_Dest);
}

// FUNCTION TEMPLATE _Uninitialized_move WITH ALLOCATOR
template<class _InIt,
	class _FwdIt,
	class _Alloc> inline
	_FwdIt _Uninitialized_move_al_unchecked(_InIt _First, const _InIt _Last, const _FwdIt _Dest, _Alloc& _Al,
		_General_ptr_iterator_tag, _Any_tag)
{	// move [_First, _Last) to raw _Dest, using _Al, no special optimization
	_Uninitialized_backout_al<_FwdIt, _Alloc> _Backout{ _Dest, _Al };
	for (; _First != _Last; ++_First)
	{
		_Backout._Emplace_back(_STD move(*_First));
	}

	return (_Backout._Release());
}

template<class _Ty1,
	class _Ty2,
	class _Alloc> inline
	_Ty2 *_Uninitialized_move_al_unchecked(_Ty1 * const _First, _Ty1 * const _Last, _Ty2 * const _Dest, _Alloc&,
		_Really_trivial_ptr_iterator_tag, true_type)
{	// move [_First, _Last) to raw _Dest, using default _Alloc construct, memmove optimization
	return (_Copy_memmove(_First, _Last, _Dest));
}

template<class _InIt,
	class _FwdIt,
	class _Alloc> inline
	_FwdIt _Uninitialized_move(const _InIt _First, const _InIt _Last, _FwdIt _Dest, _Alloc& _Al)
{	// move [_First, _Last) to raw _Dest, using _Al
	// note: only called internally from elsewhere in the STL
	const auto _UFirst = _Get_unwrapped(_First);
	const auto _ULast = _Get_unwrapped(_Last);
	const auto _UDest = _Get_unwrapped_n(_Dest, _Idl_distance<_InIt>(_UFirst, _ULast));
	_Seek_wrapped(_Dest,
		_Uninitialized_move_al_unchecked(_UFirst, _ULast, _UDest, _Al,
			_Ptr_move_cat(_UFirst, _UDest),
			_Uses_default_construct_t<_Alloc, decltype(_Unfancy(_UDest)), decltype(_STD move(*_UFirst))>()));
	return (_Dest);
}

// FUNCTION TEMPLATE _Uninitialized_fill_n WITH ALLOCATOR
template<class _FwdIt,
	class _Diff,
	class _Alloc> inline
	_FwdIt _Uninit_alloc_fill_n1(const _FwdIt _First, _Diff _Count, const _Iter_value_t<_FwdIt>& _Val,
		_Alloc& _Al, false_type)
{	// copy _Count copies of _Val to raw _First, using _Al, no special optimization
	_Uninitialized_backout_al<_FwdIt, _Alloc> _Backout{ _First, _Al };
	for (; 0 < _Count; --_Count)
	{
		_Backout._Emplace_back(_Val);
	}

	return (_Backout._Release());
}

template<class _FwdIt,
	class _Diff,
	class _Alloc> inline
	_FwdIt _Uninit_alloc_fill_n1(const _FwdIt _First, const _Diff _Count, const _Iter_value_t<_FwdIt>& _Val,
		_Alloc&, true_type)
{	// copy _Count copies of _Val to raw _First, using default _Alloc construct, memset optimization
	_CSTD memset(_First, static_cast<unsigned char>(_Val), static_cast<size_t>(_Count));
	return (_First + _Count);
}

template<class _FwdIt,
	class _Diff,
	class _Alloc> inline
	_FwdIt _Uninitialized_fill_n(const _FwdIt _First, const _Diff _Count, const _Iter_value_t<_FwdIt>& _Val,
		_Alloc& _Al)
{	// copy _Count copies of _Val to raw _First, using _Al
	return (_Uninit_alloc_fill_n1(_First, _Count, _Val, _Al,
		bool_constant<conjunction_v<decltype(_Fill_memset_is_safe(_First, _Val)),
		_Uses_default_construct<_Alloc, decltype(_Unfancy(_First)), decltype(_Val)>>>{}));
}

// FUNCTION TEMPLATE _Uninitialized_value_construct_n WITH ALLOCATOR
template<class _FwdIt>
using _Use_memset_value_construct_t = bool_constant<conjunction_v<
	is_pointer<_FwdIt>,
	is_scalar<_Iter_value_t<_FwdIt>>,
	negation<is_volatile<_Iter_value_t<_FwdIt>>>,
	negation<is_member_pointer<_Iter_value_t<_FwdIt>>>>>;

template<class _FwdIt> inline
_FwdIt _Zero_range(const _FwdIt _First, const _FwdIt _Last)
{	// fill [_First, _Last) with zeroes
	char * const _First_ch = reinterpret_cast<char *>(_First);
	char * const _Last_ch = reinterpret_cast<char *>(_Last);
	_CSTD memset(_First_ch, 0, static_cast<size_t>(_Last_ch - _First_ch));
	return (_Last);
}

template<class _FwdIt,
	class _Diff,
	class _Alloc> inline
	_FwdIt _Uninitialized_value_construct_n1(const _FwdIt _First, _Diff _Count, _Alloc& _Al, false_type)
{	// value-initialize _Count objects to raw _First, using _Al, no special optimization
	_Uninitialized_backout_al<_FwdIt, _Alloc> _Backout{ _First, _Al };
	for (; 0 < _Count; --_Count)
	{
		_Backout._Emplace_back();
	}

	return (_Backout._Release());
}

template<class _FwdIt,
	class _Diff,
	class _Alloc> inline
	_FwdIt _Uninitialized_value_construct_n1(_FwdIt _First, _Diff _Count, _Alloc&, true_type)
{	// value-initialize _Count objects to raw _First, using default _Alloc construct, all-bits-zero type
	return (_Zero_range(_First, _First + _Count));
}

template<class _FwdIt,
	class _Diff,
	class _Alloc> inline
	_FwdIt _Uninitialized_value_construct_n(_FwdIt _First, _Diff _Count, _Alloc& _Al)
{	// value-initialize _Count objects to raw _First, using _Al
	return (_Uninitialized_value_construct_n1(_First, _Count, _Al,
		bool_constant<conjunction_v<_Use_memset_value_construct_t<_FwdIt>,
		_Uses_default_construct<_Alloc, decltype(_Unfancy(_First))>>>{}));
}

// STRUCT _Not_a_node_tag
struct _Not_a_node_tag
{	// Used as the type of a "pointer" to a non-node.
};

// FUNCTION TEMPLATE get_temporary_buffer
template<class _Ty>
_NODISCARD _CXX17_DEPRECATE_TEMPORARY_BUFFER pair<_Ty *, ptrdiff_t> get_temporary_buffer(ptrdiff_t _Count) noexcept
{	// get raw temporary buffer of up to _Count elements
	return (_Get_temporary_buffer<_Ty>(_Count));
}

// FUNCTION TEMPLATE return_temporary_buffer
template<class _Ty>
_CXX17_DEPRECATE_TEMPORARY_BUFFER void return_temporary_buffer(_Ty * _Pbuf)
{	// delete raw temporary buffer
	_Return_temporary_buffer(_Pbuf);
}
_STD_END

#pragma pop_macro("new")
_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)
#endif /* RC_INVOKED */
#endif /* _XMEMORY_ */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0009 */
