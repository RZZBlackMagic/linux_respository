// vector standard header
#pragma once
#ifndef _VECTOR_
#define _VECTOR_
#ifndef RC_INVOKED
#include <xmemory>
#include <stdexcept>

#if _HAS_CXX17
#include <xpolymorphic_allocator.h>
#endif /* _HAS_CXX17 */

#pragma pack(push,_CRT_PACKING)
#pragma warning(push,_STL_WARNING_LEVEL)
#pragma warning(disable: _STL_DISABLED_WARNINGS)
_STL_DISABLE_CLANG_WARNINGS
#pragma push_macro("new")
#undef new

_STD_BEGIN
// CLASS TEMPLATE _Vector_const_iterator
template<class _Myvec>
class _Vector_const_iterator
	: public _Iterator_base
{	// iterator for nonmutable vector
public:
	using iterator_category = random_access_iterator_tag;

	using value_type = typename _Myvec::value_type;
	using difference_type = typename _Myvec::difference_type;
	using pointer = typename _Myvec::const_pointer;
	using reference = const value_type&;
	using _Tptr = typename _Myvec::pointer;

	_Vector_const_iterator()
		: _Ptr()
	{	// construct with null pointer
	}

	_Vector_const_iterator(_Tptr _Parg, const _Container_base *_Pvector)
		: _Ptr(_Parg)
	{	// construct with pointer _Parg
		this->_Adopt(_Pvector);
	}

	_NODISCARD reference operator*() const
	{	// return designated object
#if _ITERATOR_DEBUG_LEVEL != 0
		const auto _Mycont = static_cast<const _Myvec *>(this->_Getcont());
		_STL_VERIFY(_Ptr, "can't dereference value-initialized vector iterator");
		_STL_VERIFY(_Mycont->_Myfirst <= _Ptr && _Ptr < _Mycont->_Mylast,
			"can't dereference out of range vector iterator");
#endif /* _ITERATOR_DEBUG_LEVEL != 0 */

		return (*_Ptr);
	}

	_NODISCARD pointer operator->() const
	{	// return pointer to class object
#if _ITERATOR_DEBUG_LEVEL != 0
		const auto _Mycont = static_cast<const _Myvec *>(this->_Getcont());
		_STL_VERIFY(_Ptr, "can't dereference value-initialized vector iterator");
		_STL_VERIFY(_Mycont->_Myfirst <= _Ptr && _Ptr < _Mycont->_Mylast,
			"can't dereference out of range vector iterator");
#endif /* _ITERATOR_DEBUG_LEVEL != 0 */

		return (_Ptr);
	}

	_Vector_const_iterator& operator++()
	{	// preincrement
#if _ITERATOR_DEBUG_LEVEL != 0
		const auto _Mycont = static_cast<const _Myvec *>(this->_Getcont());
		_STL_VERIFY(_Ptr, "can't increment value-initialized vector iterator");
		_STL_VERIFY(_Ptr < _Mycont->_Mylast, "can't increment vector iterator past end");
#endif /* _ITERATOR_DEBUG_LEVEL != 0 */

		++_Ptr;
		return (*this);
	}

	_Vector_const_iterator operator++(int)
	{	// postincrement
		_Vector_const_iterator _Tmp = *this;
		++*this;
		return (_Tmp);
	}

	_Vector_const_iterator& operator--()
	{	// predecrement
#if _ITERATOR_DEBUG_LEVEL != 0
		const auto _Mycont = static_cast<const _Myvec *>(this->_Getcont());
		_STL_VERIFY(_Ptr, "can't decrement value-initialized vector iterator");
		_STL_VERIFY(_Mycont->_Myfirst < _Ptr, "can't decrement vector iterator before begin");
#endif /* _ITERATOR_DEBUG_LEVEL != 0 */

		--_Ptr;
		return (*this);
	}

	_Vector_const_iterator operator--(int)
	{	// postdecrement
		_Vector_const_iterator _Tmp = *this;
		--*this;
		return (_Tmp);
	}

	void _Verify_offset(const difference_type _Off) const
	{
#if _ITERATOR_DEBUG_LEVEL == 0
		(void)_Off;
#else /* ^^^ _ITERATOR_DEBUG_LEVEL == 0 ^^^ // vvv _ITERATOR_DEBUG_LEVEL != 0 vvv */
		const auto _Mycont = static_cast<const _Myvec *>(this->_Getcont());
		_STL_VERIFY(_Off == 0 || _Ptr, "cannot seek value-initialized vector iterator");
		if (_Off < 0)
		{
			_STL_VERIFY(_Off >= _Mycont->_Myfirst - _Ptr, "cannot seek vector iterator before begin");
		}

		if (_Off > 0)
		{
			_STL_VERIFY(_Off <= _Mycont->_Mylast - _Ptr, "cannot seek vector iterator after end");
		}
#endif /* _ITERATOR_DEBUG_LEVEL == 0 */
	}

	_Vector_const_iterator& operator+=(const difference_type _Off)
	{	// increment by integer
		_Verify_offset(_Off);
		_Ptr += _Off;
		return (*this);
	}

	_NODISCARD _Vector_const_iterator operator+(const difference_type _Off) const
	{	// return this + integer
		_Vector_const_iterator _Tmp = *this;
		return (_Tmp += _Off);
	}

	_Vector_const_iterator& operator-=(const difference_type _Off)
	{	// decrement by integer
		return (*this += -_Off);
	}

	_NODISCARD _Vector_const_iterator operator-(const difference_type _Off) const
	{	// return this - integer
		_Vector_const_iterator _Tmp = *this;
		return (_Tmp -= _Off);
	}

	_NODISCARD difference_type operator-(const _Vector_const_iterator& _Right) const
	{	// return difference of iterators
		_Compat(_Right);
		return (_Ptr - _Right._Ptr);
	}

	_NODISCARD reference operator[](const difference_type _Off) const
	{	// subscript
		return (*(*this + _Off));
	}

	_NODISCARD bool operator==(const _Vector_const_iterator& _Right) const
	{	// test for iterator equality
		_Compat(_Right);
		return (_Ptr == _Right._Ptr);
	}

	_NODISCARD bool operator!=(const _Vector_const_iterator& _Right) const
	{	// test for iterator inequality
		return (!(*this == _Right));
	}

	_NODISCARD bool operator<(const _Vector_const_iterator& _Right) const
	{	// test if this < _Right
		_Compat(_Right);
		return (_Ptr < _Right._Ptr);
	}

	_NODISCARD bool operator>(const _Vector_const_iterator& _Right) const
	{	// test if this > _Right
		return (_Right < *this);
	}

	_NODISCARD bool operator<=(const _Vector_const_iterator& _Right) const
	{	// test if this <= _Right
		return (!(_Right < *this));
	}

	_NODISCARD bool operator>=(const _Vector_const_iterator& _Right) const
	{	// test if this >= _Right
		return (!(*this < _Right));
	}

	void _Compat(const _Vector_const_iterator& _Right) const
	{	// test for compatible iterator pair
#if _ITERATOR_DEBUG_LEVEL == 0
		(void)_Right;
#else /* ^^^ _ITERATOR_DEBUG_LEVEL == 0 ^^^ // vvv _ITERATOR_DEBUG_LEVEL != 0 vvv */
		_STL_VERIFY(this->_Getcont() == _Right._Getcont(), "vector iterators incompatible");
#endif /* _ITERATOR_DEBUG_LEVEL == 0 */
	}

#if _ITERATOR_DEBUG_LEVEL != 0
	friend void _Verify_range(const _Vector_const_iterator& _First, const _Vector_const_iterator& _Last)
	{
		_STL_VERIFY(_First._Getcont() == _Last._Getcont(),
			"vector iterators in range are from different containers");
		_STL_VERIFY(_First._Ptr <= _Last._Ptr, "vector iterator range transposed");
	}
#endif /* _ITERATOR_DEBUG_LEVEL != 0 */

	_NODISCARD pointer _Unwrapped() const
	{
		return (_Ptr);
	}

	void _Seek_to(pointer _It)
	{
		_Ptr = _Const_cast(_It);
	}

	_Tptr _Ptr;	// pointer to element in vector
};

template<class _Myvec>
_NODISCARD inline _Vector_const_iterator<_Myvec> operator+(
	typename _Vector_const_iterator<_Myvec>::difference_type _Off,
	_Vector_const_iterator<_Myvec> _Next)
{	// add offset to iterator
	return (_Next += _Off);
}

// CLASS TEMPLATE _Vector_iterator
template<class _Myvec>
class _Vector_iterator
	: public _Vector_const_iterator<_Myvec>
{	// iterator for mutable vector
public:
	using _Mybase = _Vector_const_iterator<_Myvec>;
	using iterator_category = random_access_iterator_tag;

	using value_type = typename _Myvec::value_type;
	using difference_type = typename _Myvec::difference_type;
	using pointer = typename _Myvec::pointer;
	using reference = value_type & ;

	_Vector_iterator()
	{	// construct with null vector pointer
	}

	_Vector_iterator(pointer _Parg, const _Container_base *_Pvector)
		: _Mybase(_Parg, _Pvector)
	{	// construct with pointer _Parg
	}

	_NODISCARD reference operator*() const
	{	// return designated object
		return (const_cast<reference>(_Mybase::operator*()));
	}

	_NODISCARD pointer operator->() const
	{	// return pointer to class object
		return (_Const_cast(_Mybase::operator->()));
	}

	_Vector_iterator& operator++()
	{	// preincrement
		++*(_Mybase *)this;
		return (*this);
	}

	_Vector_iterator operator++(int)
	{	// postincrement
		_Vector_iterator _Tmp = *this;
		++*this;
		return (_Tmp);
	}

	_Vector_iterator& operator--()
	{	// predecrement
		--*(_Mybase *)this;
		return (*this);
	}

	_Vector_iterator operator--(int)
	{	// postdecrement
		_Vector_iterator _Tmp = *this;
		--*this;
		return (_Tmp);
	}

	_Vector_iterator& operator+=(const difference_type _Off)
	{	// increment by integer
		*(_Mybase *)this += _Off;
		return (*this);
	}

	_NODISCARD _Vector_iterator operator+(const difference_type _Off) const
	{	// return this + integer
		_Vector_iterator _Tmp = *this;
		return (_Tmp += _Off);
	}

	_Vector_iterator& operator-=(const difference_type _Off)
	{	// decrement by integer
		return (*this += -_Off);
	}

	_NODISCARD _Vector_iterator operator-(const difference_type _Off) const
	{	// return this - integer
		_Vector_iterator _Tmp = *this;
		return (_Tmp -= _Off);
	}

	_NODISCARD difference_type operator-(const _Mybase& _Right) const
	{	// return difference of iterators
		return (*(_Mybase *)this - _Right);
	}

	_NODISCARD reference operator[](const difference_type _Off) const
	{	// subscript
		return (*(*this + _Off));
	}

	_NODISCARD pointer _Unwrapped() const
	{
		return (this->_Ptr);
	}
};

template<class _Myvec>
_NODISCARD inline _Vector_iterator<_Myvec> operator+(
	typename _Vector_iterator<_Myvec>::difference_type _Off,
	_Vector_iterator<_Myvec> _Next)
{	// add offset to iterator
	return (_Next += _Off);
}

// vector TYPE WRAPPERS
template<class _Value_type,
	class _Size_type,
	class _Difference_type,
	class _Pointer,
	class _Const_pointer,
	class _Reference,
	class _Const_reference>
	struct _Vec_iter_types
{	// wraps types needed by iterators
	using value_type = _Value_type;
	using size_type = _Size_type;
	using difference_type = _Difference_type;
	using pointer = _Pointer;
	using const_pointer = _Const_pointer;
};

template<class _Ty,
	class _Alloc>
	struct _Vec_base_types
{	// types needed for a container base
	using _Alty = _Rebind_alloc_t<_Alloc, _Ty>;
	using _Alty_traits = allocator_traits<_Alty>;

	using _Val_types = conditional_t<_Is_simple_alloc_v<_Alty>,
		_Simple_types<_Ty>,
		_Vec_iter_types<_Ty,
		typename _Alty_traits::size_type,
		typename _Alty_traits::difference_type,
		typename _Alty_traits::pointer,
		typename _Alty_traits::const_pointer,
		_Ty&,
		const _Ty&>>;
};

// CLASS TEMPLATE _Vector_val
template<class _Val_types>
class _Vector_val
	: public _Container_base
{	// base class for vector to hold data
public:
	using value_type = typename _Val_types::value_type;
	using size_type = typename _Val_types::size_type;
	using difference_type = typename _Val_types::difference_type;
	using pointer = typename _Val_types::pointer;
	using const_pointer = typename _Val_types::const_pointer;
	using reference = value_type & ;
	using const_reference = const value_type&;

	_Vector_val()
		: _Myfirst(),
		_Mylast(),
		_Myend()
	{	// initialize values
	}

	pointer _Myfirst;	// pointer to beginning of array
	pointer _Mylast;	// pointer to current end of sequence
	pointer _Myend;	// pointer to end of array
};

// CLASS TEMPLATE _Vector_alloc
template<class _Alloc_types>
class _Vector_alloc
{	// base class for vector to hold allocator
public:
	using _Alty = typename _Alloc_types::_Alty;
	using _Alty_traits = typename _Alloc_types::_Alty_traits;
	using _Alproxy = _Rebind_alloc_t<_Alty, _Container_proxy>;
	using _Alproxy_traits = allocator_traits<_Alproxy>;
	using _Val_types = typename _Alloc_types::_Val_types;

	using size_type = typename _Val_types::size_type;
	using difference_type = typename _Val_types::difference_type;
	using pointer = typename _Val_types::pointer;
	using const_pointer = typename _Val_types::const_pointer;

	using iterator = _Vector_iterator<_Vector_val<_Val_types>>;
	using const_iterator = _Vector_const_iterator<_Vector_val<_Val_types>>;

#if _ITERATOR_DEBUG_LEVEL == 0
	_Vector_alloc()
		: _Mypair(_Zero_then_variadic_args_t())
	{	// default construct allocator
	}

	template<class _Any_alloc,
		class = enable_if_t<!is_same_v<remove_cv_t<remove_reference_t<_Any_alloc>>, _Vector_alloc>>>
		_Vector_alloc(_Any_alloc&& _Al)
		: _Mypair(_One_then_variadic_args_t(),
			_STD forward<_Any_alloc>(_Al))
	{	// construct allocator from _Al
	}

	void _Copy_alloc(const _Alty& _Al)
	{	// replace old allocator
		_Pocca(_Getal(), _Al);
	}

	void _Move_alloc(_Alty& _Al)
	{	// replace old allocator
		_Pocma(_Getal(), _Al);
	}

#else /* ^^^ _ITERATOR_DEBUG_LEVEL == 0 ^^^ // vvv _ITERATOR_DEBUG_LEVEL != 0 vvv */
	_Vector_alloc()
		: _Mypair(_Zero_then_variadic_args_t())
	{	// default construct allocator
		_Alloc_proxy();
	}

	template<class _Any_alloc,
		class = enable_if_t<!is_same_v<remove_cv_t<remove_reference_t<_Any_alloc>>, _Vector_alloc>>>
		_Vector_alloc(_Any_alloc&& _Al)
		: _Mypair(_One_then_variadic_args_t(),
			_STD forward<_Any_alloc>(_Al))
	{	// construct allocator from _Al
		_Alloc_proxy();
	}

	~_Vector_alloc() noexcept
	{	// destroy proxy
		_Free_proxy();
	}

	void _Copy_alloc(const _Alty& _Al)
	{	// replace old allocator
		const bool _Reload = _Alty_traits::propagate_on_container_copy_assignment::value
			&& _Getal() != _Al;

		if (_Reload)
		{
			_Free_proxy();
		}

		_Pocca(_Getal(), _Al);

		if (_Reload)
		{
			_Alloc_proxy();
		}
	}

	void _Move_alloc(_Alty& _Al)
	{	// replace old allocator
		const bool _Reload = _Alty_traits::propagate_on_container_move_assignment::value
			&& _Getal() != _Al;

		if (_Reload)
		{
			_Free_proxy();
		}

		_Pocma(_Getal(), _Al);

		if (_Reload)
		{
			_Alloc_proxy();
		}
	}

	void _Alloc_proxy()
	{	// construct proxy
		_Alproxy _Proxy_allocator(_Getal());
		_Myproxy() = _Unfancy(_Proxy_allocator.allocate(1));
		_Alproxy_traits::construct(_Proxy_allocator, _Myproxy(), _Container_proxy());
		_Myproxy()->_Mycont = _STD addressof(_Get_data());
	}

	void _Free_proxy()
	{	// destroy proxy
		_Alproxy _Proxy_allocator(_Getal());
		_Orphan_all();
		_Alproxy_traits::destroy(_Proxy_allocator, _Myproxy());
		_Deallocate_plain(_Proxy_allocator, _Myproxy());
		_Myproxy() = nullptr;
	}

	_Iterator_base12 **_Getpfirst() const
	{	// get address of iterator chain
		return (_Get_data()._Getpfirst());
	}

	_Container_proxy * & _Myproxy() noexcept
	{	// return reference to _Myproxy
		return (_Get_data()._Myproxy);
	}

	_Container_proxy * const & _Myproxy() const noexcept
	{	// return const reference to _Myproxy
		return (_Get_data()._Myproxy);
	}
#endif /* _ITERATOR_DEBUG_LEVEL == 0 */

	void _Orphan_all()
	{	// orphan all iterators
		_Get_data()._Orphan_all();
	}

	void _Swap_all(_Vector_alloc& _Right)
	{	// swap all iterators
		_Get_data()._Swap_all(_Right._Get_data());
	}

	_Alty& _Getal() noexcept
	{	// return reference to allocator
		return (_Mypair._Get_first());
	}

	const _Alty& _Getal() const noexcept
	{	// return const reference to allocator
		return (_Mypair._Get_first());
	}

	_Vector_val<_Val_types>& _Get_data() noexcept
	{	// return reference to _Vector_val
		return (_Mypair._Get_second());
	}

	const _Vector_val<_Val_types>& _Get_data() const noexcept
	{	// return const reference to _Vector_val
		return (_Mypair._Get_second());
	}

	iterator _Make_iterator(const pointer _Ptr) noexcept
	{	// return an iterator for pointer _Ptr
		return (iterator(_Ptr, _STD addressof(_Mypair._Get_second())));
	}

	iterator _Make_iterator_offset(const size_type _Offset) noexcept
	{	// return the iterator begin() + _Offset without a debugging check
		auto& _My_data = _Mypair._Get_second();
		return (iterator(_My_data._Myfirst + _Offset, _STD addressof(_My_data)));
	}

	pointer& _Myfirst() noexcept
	{	// return reference to _Myfirst
		return (_Get_data()._Myfirst);
	}

	const pointer& _Myfirst() const noexcept
	{	// return const reference to _Myfirst
		return (_Get_data()._Myfirst);
	}

	pointer& _Mylast() noexcept
	{	// return reference to _Mylast
		return (_Get_data()._Mylast);
	}

	const pointer& _Mylast() const noexcept
	{	// return const reference to _Mylast
		return (_Get_data()._Mylast);
	}

	pointer& _Myend() noexcept
	{	// return reference to _Myend
		return (_Get_data()._Myend);
	}

	const pointer& _Myend() const noexcept
	{	// return const reference to _Myend
		return (_Get_data()._Myend);
	}

private:
	_Compressed_pair<_Alty, _Vector_val<_Val_types>> _Mypair;
};

// FUNCTION TEMPLATE _Unfancy_maybe_null
template<class _Ptrty> inline
auto _Unfancy_maybe_null(_Ptrty _Ptr)
{	// converts from a (potentially null) fancy pointer to a plain pointer
	return (_Ptr ? _STD addressof(*_Ptr) : nullptr);
}

template<class _Ty> inline
_Ty * _Unfancy_maybe_null(_Ty * _Ptr)
{	// do nothing for plain pointers
	return (_Ptr);
}

// CLASS TEMPLATE vector
template<class _Ty,
	class _Alloc = allocator<_Ty>>
	class vector
	: public _Vector_alloc<_Vec_base_types<_Ty, _Alloc>>
{	// varying size array of values
private:
	using _Mybase = _Vector_alloc<_Vec_base_types<_Ty, _Alloc>>;
	using _Alty = typename _Mybase::_Alty;
	using _Alty_traits = typename _Mybase::_Alty_traits;

public:
	static_assert(!_ENFORCE_MATCHING_ALLOCATORS || is_same_v<_Ty, typename _Alloc::value_type>,
		_MISMATCHED_ALLOCATOR_MESSAGE("vector<T, Allocator>", "T"));

	using value_type = _Ty;
	using allocator_type = _Alloc;
	using pointer = typename _Mybase::pointer;
	using const_pointer = typename _Mybase::const_pointer;
	using reference = _Ty & ;
	using const_reference = const _Ty&;
	using size_type = typename _Mybase::size_type;
	using difference_type = typename _Mybase::difference_type;
	using iterator = typename _Mybase::iterator;
	using const_iterator = typename _Mybase::const_iterator;
	using reverse_iterator = _STD reverse_iterator<iterator>;
	using const_reverse_iterator = _STD reverse_iterator<const_iterator>;

	vector() _NOEXCEPT_COND(is_nothrow_default_constructible_v<_Alty>)
		: _Mybase()
	{	// construct empty vector
	}

	explicit vector(const _Alloc& _Al) noexcept
		: _Mybase(_Al)
	{	// construct empty vector, allocator
	}

	explicit vector(_CRT_GUARDOVERFLOW const size_type _Count, const _Alloc& _Al = _Alloc())
		: _Mybase(_Al)
	{	// construct from _Count * _Ty(), optional allocator
		if (_Buy(_Count))
		{	// nonzero, fill it
			_TRY_BEGIN
				this->_Mylast() = _Udefault(this->_Myfirst(), _Count);
			_CATCH_ALL
				_Tidy();
			_RERAISE;
			_CATCH_END
		}
	}

	vector(_CRT_GUARDOVERFLOW const size_type _Count, const _Ty& _Val, const _Alloc& _Al = _Alloc())
		: _Mybase(_Al)
	{	// construct from _Count * _Val, optional allocator
		if (_Buy(_Count))
		{	// nonzero, fill it
			_TRY_BEGIN
				this->_Mylast() = _Ufill(this->_Myfirst(), _Count, _Val);
			_CATCH_ALL
				_Tidy();
			_RERAISE;
			_CATCH_END
		}
	}

private:
	template<class _Iter>
	void _Range_construct_or_tidy(_Iter _First, _Iter _Last, input_iterator_tag)
	{	// initialize with [_First, _Last), avoid leaking, input iterators
		_TRY_BEGIN
			for (; _First != _Last; ++_First)
			{
				emplace_back(*_First);	// performance note: emplace_back()'s strong guarantee is unnecessary here
			}
		_CATCH_ALL
			_Tidy();
		_RERAISE;
		_CATCH_END
	}

	template<class _Iter>
	void _Range_construct_or_tidy(_Iter _First, _Iter _Last, forward_iterator_tag)
	{	// initialize with [_First, _Last), avoid leaking, forward iterators
		if (_Buy(_Convert_size<size_type>(static_cast<size_t>(_STD distance(_First, _Last)))))
		{	// nonzero, fill it
			_TRY_BEGIN
				this->_Mylast() = _Ucopy(_First, _Last, this->_Myfirst());
			_CATCH_ALL
				_Tidy();
			_RERAISE;
			_CATCH_END
		}
	}

public:
	template<class _Iter,
		class = enable_if_t<_Is_iterator_v<_Iter>>>
		vector(_Iter _First, _Iter _Last, const _Alloc& _Al = _Alloc())
		: _Mybase(_Al)
	{	// construct from [_First, _Last) with optional allocator
		_Adl_verify_range(_First, _Last);
		_Range_construct_or_tidy(_Get_unwrapped(_First), _Get_unwrapped(_Last), _Iter_cat_t<_Iter>{});
	}

	vector(initializer_list<_Ty> _Ilist, const _Alloc& _Al = _Alloc())
		: _Mybase(_Al)
	{	// construct from initializer_list, optional allocator
		_Range_construct_or_tidy(_Ilist.begin(), _Ilist.end(), random_access_iterator_tag{});
	}

	vector(const vector& _Right)
		: _Mybase(_Alty_traits::select_on_container_copy_construction(_Right._Getal()))
	{	// construct by copying _Right
		if (_Buy(_Right.size()))
		{	// nonzero, fill it
			_TRY_BEGIN
				this->_Mylast() = _Ucopy(_Right._Myfirst(), _Right._Mylast(), this->_Myfirst());
			_CATCH_ALL
				_Tidy();
			_RERAISE;
			_CATCH_END
		}
	}

	vector(const vector& _Right, const _Alloc& _Al)
		: _Mybase(_Al)
	{	// construct by copying _Right, allocator
		if (_Buy(_Right.size()))
		{	// nonzero, fill it
			_TRY_BEGIN
				this->_Mylast() = _Ucopy(_Right._Myfirst(), _Right._Mylast(), this->_Myfirst());
			_CATCH_ALL
				_Tidy();
			_RERAISE;
			_CATCH_END
		}
	}

private:
	void _Move_from(vector&& _Right, true_type) noexcept
	{	// move from _Right, stealing its contents
		this->_Swap_all(_Right);

		this->_Myfirst() = _Right._Myfirst();
		this->_Mylast() = _Right._Mylast();
		this->_Myend() = _Right._Myend();

		_Right._Myfirst() = pointer();
		_Right._Mylast() = pointer();
		_Right._Myend() = pointer();
	}

	void _Move_from(vector&& _Right, false_type)
	{	// move from _Right, possibly moving its contents
		if (this->_Getal() == _Right._Getal())
		{
			_Move_from(_STD move(_Right), true_type{});
		}
		else if (_Buy(_Right.size()))
		{	// nonzero, fill it
			this->_Mylast() = _Umove(_Right._Myfirst(), _Right._Mylast(), this->_Myfirst());
		}
	}

public:
	vector(vector&& _Right) noexcept
		: _Mybase(_STD move(_Right._Getal()))
	{	// construct by moving _Right
		_Move_from(_STD move(_Right), true_type{});
	}

	vector(vector&& _Right, const _Alloc& _Al)
		_NOEXCEPT_COND(_Alty_traits::is_always_equal::value) // strengthened
		: _Mybase(_Al)
	{	// construct by moving _Right, allocator
		if /* constexpr */ (_Alty_traits::is_always_equal::value)
		{
			_Move_from(_STD move(_Right), true_type{});
		}
		else
		{
#pragma warning(push) // TRANSITION, if constexpr
#pragma warning(disable: 4297) // function assumed not to throw an exception but does
			_TRY_BEGIN
				_Move_from(_STD move(_Right), false_type{});
			_CATCH_ALL
				_Tidy();
			_RERAISE;
			_CATCH_END
#pragma warning(pop)
		}
	}

private:
	void _Move_assign_from(vector&& _Right, true_type) noexcept
	{	// move from _Right, stealing its contents
		_Move_from(_STD move(_Right), true_type{});
	}

	void _Move_assign_from(vector&& _Right, false_type)
	{	// move from _Right, possibly moving its contents
		if (this->_Getal() == _Right._Getal())
		{
			_Move_from(_STD move(_Right), true_type{});
			return;
		}

		const pointer _First = _Right._Myfirst();
		const pointer _Last = _Right._Mylast();
		const size_type _Newsize = _Right.size();

		this->_Orphan_all();

		const size_type _Oldsize = size();
		const size_type _Oldcapacity = capacity();

		if (_Newsize > _Oldcapacity)
		{	// reallocate
			if (_Newsize > max_size())
			{
				_Xlength();
			}

			const size_type _Newcapacity = _Calculate_growth(_Newsize);

			if (this->_Myfirst() != pointer())
			{	// destroy and deallocate old array
				_Destroy(this->_Myfirst(), this->_Mylast());
				this->_Getal().deallocate(this->_Myfirst(), _Oldcapacity);
			}

			_Buy(_Newcapacity);

			this->_Mylast() = _Umove(_First, _Last, this->_Myfirst());
		}
		else if (_Newsize > _Oldsize)
		{
			const pointer _Mid = _First + _Oldsize;
			_Move_unchecked(_First, _Mid, this->_Myfirst());
			this->_Mylast() = _Umove(_Mid, _Last, this->_Mylast());
		}
		else
		{
			const pointer _Newlast = this->_Myfirst() + _Newsize;
			_Move_unchecked(_First, _Last, this->_Myfirst());
			_Destroy(_Newlast, this->_Mylast());
			this->_Mylast() = _Newlast;
		}
	}

public:
	vector& operator=(vector&& _Right)
		_NOEXCEPT_COND(_Always_equal_after_move<_Alty>)
	{	// assign by moving _Right
		if (this != _STD addressof(_Right))
		{	// different, assign it
#pragma warning(push)
#pragma warning(disable: 4127)	// conditional expression is constant
			if (_Always_equal_after_move<_Alty>
				|| this->_Getal() == _Right._Getal())
			{	// will steal _Right's contents (also, POCMA non-equal must reload array)
				_Tidy();
			}
#pragma warning(pop)
			this->_Move_alloc(_Right._Getal());
			_Move_assign_from(_STD move(_Right), bool_constant<_Always_equal_after_move<_Alty>>{});
		}
		return (*this);
	}

	~vector() noexcept
	{	// destroy the object
		_Tidy();
	}

private:
	template<class... _Valty>
	decltype(auto) _Emplace_back_with_unused_capacity(_Valty&&... _Val)
	{	// insert by perfectly forwarding into element at end, provide strong guarantee
		// pre: _Has_unused_capacity()
		_Alty_traits::construct(this->_Getal(), _Unfancy(this->_Mylast()), _STD forward<_Valty>(_Val)...);
		_Orphan_range(this->_Mylast(), this->_Mylast());
		_Ty& _Result = *this->_Mylast();
		++this->_Mylast();
#if _HAS_CXX17
		return (_Result);
#else /* ^^^ _HAS_CXX17 ^^^ // vvv !_HAS_CXX17 vvv */
		(void)_Result;
#endif /* _HAS_CXX17 */
	}

public:
	template<class... _Valty>
	decltype(auto) emplace_back(_Valty&&... _Val)
	{	// insert by perfectly forwarding into element at end, provide strong guarantee
		if (_Has_unused_capacity())
		{
			return (_Emplace_back_with_unused_capacity(_STD forward<_Valty>(_Val)...));
		}

		_Ty& _Result = *_Emplace_reallocate(this->_Mylast(), _STD forward<_Valty>(_Val)...);
#if _HAS_CXX17
		return (_Result);
#else /* ^^^ _HAS_CXX17 ^^^ // vvv !_HAS_CXX17 vvv */
		(void)_Result;
#endif /* _HAS_CXX17 */
	}

	void push_back(const _Ty& _Val)
	{	// insert element at end, provide strong guarantee
		emplace_back(_Val);
	}

	void push_back(_Ty&& _Val)
	{	// insert by moving into element at end, provide strong guarantee
		emplace_back(_STD move(_Val));
	}

	template<class... _Valty>
	pointer _Emplace_reallocate(const pointer _Whereptr, _Valty&&... _Val)
	{	// reallocate and insert by perfectly forwarding _Val at _Whereptr
		// pre: !_Has_unused_capacity()
		const size_type _Whereoff = static_cast<size_type>(_Whereptr - this->_Myfirst());
		_Alty& _Al = this->_Getal();
		const size_type _Oldsize = size();

		if (_Oldsize == max_size())
		{
			_Xlength();
		}

		const size_type _Newsize = _Oldsize + 1;
		const size_type _Newcapacity = _Calculate_growth(_Newsize);

		const pointer _Newvec = _Al.allocate(_Newcapacity);
		const pointer _Constructed_last = _Newvec + _Whereoff + 1;
		pointer _Constructed_first = _Constructed_last;

		_TRY_BEGIN
			_Alty_traits::construct(_Al, _Unfancy(_Newvec + _Whereoff), _STD forward<_Valty>(_Val)...);
		_Constructed_first = _Newvec + _Whereoff;

		if (_Whereptr == this->_Mylast())
		{	// at back, provide strong guarantee
			_Umove_if_noexcept(this->_Myfirst(), this->_Mylast(), _Newvec);
		}
		else
		{	// provide basic guarantee
			_Umove(this->_Myfirst(), _Whereptr, _Newvec);
			_Constructed_first = _Newvec;
			_Umove(_Whereptr, this->_Mylast(), _Newvec + _Whereoff + 1);
		}
		_CATCH_ALL
			_Destroy(_Constructed_first, _Constructed_last);
		_Al.deallocate(_Newvec, _Newcapacity);
		_RERAISE;
		_CATCH_END

			_Change_array(_Newvec, _Newsize, _Newcapacity);
		return (this->_Myfirst() + _Whereoff);
	}

	template<class... _Valty>
	iterator emplace(const_iterator _Where, _Valty&&... _Val)
	{	// insert by perfectly forwarding _Val at _Where
		const pointer _Whereptr = _Where._Ptr;
		const pointer _Oldlast = this->_Mylast();
#if _ITERATOR_DEBUG_LEVEL == 2
		_STL_VERIFY(_Where._Getcont() == _STD addressof(this->_Get_data())
			&& _Whereptr >= this->_Myfirst()
			&& _Oldlast >= _Whereptr, "vector emplace iterator outside range");
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */

		if (_Has_unused_capacity())
		{
			if (_Whereptr == _Oldlast)
			{	// at back, provide strong guarantee
				_Emplace_back_with_unused_capacity(_STD forward<_Valty>(_Val)...);
			}
			else
			{
				_Ty _Obj(_STD forward<_Valty>(_Val)...);	// handle aliasing
				// after constructing _Obj, provide basic guarantee
				_Orphan_range(_Whereptr, _Oldlast);
				_Alty_traits::construct(this->_Getal(), _Unfancy(_Oldlast), _STD move(_Oldlast[-1]));
				++this->_Mylast();
				_Move_backward_unchecked(_Whereptr, _Oldlast - 1, _Oldlast);
				*_Whereptr = _STD move(_Obj);
			}

			return (this->_Make_iterator(_Whereptr));
		}

		return (this->_Make_iterator(_Emplace_reallocate(_Whereptr, _STD forward<_Valty>(_Val)...)));
	}

	iterator insert(const_iterator _Where, const _Ty& _Val)
	{	// insert _Val at _Where
		return (emplace(_Where, _Val));
	}

	iterator insert(const_iterator _Where, _Ty&& _Val)
	{	// insert by moving _Val at _Where
		return (emplace(_Where, _STD move(_Val)));
	}

	iterator insert(const_iterator _Where, _CRT_GUARDOVERFLOW const size_type _Count, const _Ty& _Val)
	{	// insert _Count * _Val at _Where
#if _ITERATOR_DEBUG_LEVEL == 2
		_STL_VERIFY(_Where._Getcont() == _STD addressof(this->_Get_data())
			&& _Where._Ptr >= this->_Myfirst()
			&& this->_Mylast() >= _Where._Ptr, "vector insert iterator outside range");
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */

		const size_type _Whereoff = static_cast<size_type>(_Where._Ptr - this->_Myfirst());
		const bool _One_at_back = _Count == 1 && _Where._Ptr == this->_Mylast();

		if (_Count == 0)
		{	// nothing to do, avoid invalidating iterators
		}
		else if (_Count > _Unused_capacity())
		{	// reallocate
			const size_type _Oldsize = size();

			if (_Count > max_size() - _Oldsize)
			{
				_Xlength();
			}

			const size_type _Newsize = _Oldsize + _Count;
			const size_type _Newcapacity = _Calculate_growth(_Newsize);

			const pointer _Newvec = this->_Getal().allocate(_Newcapacity);
			const pointer _Constructed_last = _Newvec + _Whereoff + _Count;
			pointer _Constructed_first = _Constructed_last;

			_TRY_BEGIN
				_Ufill(_Newvec + _Whereoff, _Count, _Val);
			_Constructed_first = _Newvec + _Whereoff;

			if (_One_at_back)
			{	// provide strong guarantee
				_Umove_if_noexcept(this->_Myfirst(), this->_Mylast(), _Newvec);
			}
			else
			{	// provide basic guarantee
				_Umove(this->_Myfirst(), _Where._Ptr, _Newvec);
				_Constructed_first = _Newvec;
				_Umove(_Where._Ptr, this->_Mylast(), _Newvec + _Whereoff + _Count);
			}
			_CATCH_ALL
				_Destroy(_Constructed_first, _Constructed_last);
			this->_Getal().deallocate(_Newvec, _Newcapacity);
			_RERAISE;
			_CATCH_END

				_Change_array(_Newvec, _Newsize, _Newcapacity);
		}
		else if (_One_at_back)
		{	// provide strong guarantee
			_Emplace_back_with_unused_capacity(_Val);
		}
		else
		{	// provide basic guarantee
			const _Ty _Tmp = _Val;	// handle aliasing
			const pointer _Oldlast = this->_Mylast();
			const size_type _Affected_elements = static_cast<size_type>(_Oldlast - _Where._Ptr);
			_Orphan_range(_Where._Ptr, _Oldlast);

			if (_Count > _Affected_elements)
			{	// new stuff spills off end
				this->_Mylast() = _Ufill(_Oldlast, _Count - _Affected_elements, _Tmp);
				this->_Mylast() = _Umove(_Where._Ptr, _Oldlast, this->_Mylast());
				_Fill_unchecked(_Where._Ptr, _Oldlast, _Tmp);
			}
			else
			{	// new stuff can all be assigned
				this->_Mylast() = _Umove(_Oldlast - _Count, _Oldlast, _Oldlast);
				_Move_backward_unchecked(_Where._Ptr, _Oldlast - _Count, _Oldlast);
				_Fill_unchecked(_Where._Ptr, _Where._Ptr + _Count, _Tmp);
			}
		}

		return (this->_Make_iterator_offset(_Whereoff));
	}

private:
	template<class _Iter>
	void _Insert_range(const_iterator _Where, _Iter _First, _Iter _Last, input_iterator_tag)
	{	// insert [_First, _Last) at _Where, input iterators
		if (_First == _Last)
		{
			return;	// nothing to do, avoid invalidating iterators
		}

		const size_type _Whereoff = static_cast<size_type>(_Where._Ptr - this->_Myfirst());
		const size_type _Oldsize = size();

		// For one-at-back, provide strong guarantee.
		// Otherwise, provide basic guarantee (despite N4659 26.3.11.5 [vector.modifiers]/1).
		// Performance note: except for one-at-back, emplace_back()'s strong guarantee is unnecessary here.

		for (; _First != _Last; ++_First)
		{
			emplace_back(*_First);
		}

		_Orphan_range(this->_Myfirst() + _Whereoff, this->_Myfirst() + _Oldsize);

		_Rotate_unchecked(this->_Myfirst() + _Whereoff, this->_Myfirst() + _Oldsize, this->_Mylast());
	}

	template<class _Iter>
	void _Insert_range(const_iterator _Where, _Iter _First, _Iter _Last, forward_iterator_tag)
	{	// insert [_First, _Last) at _Where, forward iterators
		const size_type _Count = _Convert_size<size_type>(static_cast<size_t>(_STD distance(_First, _Last)));
		const size_type _Whereoff = static_cast<size_type>(_Where._Ptr - this->_Myfirst());
		const bool _One_at_back = _Count == 1 && _Where._Ptr == this->_Mylast();

		if (_Count == 0)
		{	// nothing to do, avoid invalidating iterators
		}
		else if (_Count > _Unused_capacity())
		{	// reallocate
			const size_type _Oldsize = size();

			if (_Count > max_size() - _Oldsize)
			{
				_Xlength();
			}

			const size_type _Newsize = _Oldsize + _Count;
			const size_type _Newcapacity = _Calculate_growth(_Newsize);

			const pointer _Newvec = this->_Getal().allocate(_Newcapacity);
			const pointer _Constructed_last = _Newvec + _Whereoff + _Count;
			pointer _Constructed_first = _Constructed_last;

			_TRY_BEGIN
				_Ucopy(_First, _Last, _Newvec + _Whereoff);
			_Constructed_first = _Newvec + _Whereoff;

			if (_One_at_back)
			{	// provide strong guarantee
				_Umove_if_noexcept(this->_Myfirst(), this->_Mylast(), _Newvec);
			}
			else
			{	// provide basic guarantee
				_Umove(this->_Myfirst(), _Where._Ptr, _Newvec);
				_Constructed_first = _Newvec;
				_Umove(_Where._Ptr, this->_Mylast(), _Newvec + _Whereoff + _Count);
			}
			_CATCH_ALL
				_Destroy(_Constructed_first, _Constructed_last);
			this->_Getal().deallocate(_Newvec, _Newcapacity);
			_RERAISE;
			_CATCH_END

				_Change_array(_Newvec, _Newsize, _Newcapacity);
		}
		else
		{	// Attempt to provide the strong guarantee for EmplaceConstructible failure.
			// If we encounter copy/move construction/assignment failure, provide the basic guarantee.
			// (For one-at-back, this provides the strong guarantee.)

			const pointer _Oldlast = this->_Mylast();
			const size_type _Affected_elements = static_cast<size_type>(_Oldlast - _Where._Ptr);

			if (_Count < _Affected_elements)
			{	// some affected elements must be assigned
				this->_Mylast() = _Umove(_Oldlast - _Count, _Oldlast, _Oldlast);
				_Move_backward_unchecked(_Where._Ptr, _Oldlast - _Count, _Oldlast);
				_Destroy(_Where._Ptr, _Where._Ptr + _Count);

				_TRY_BEGIN
					_Ucopy(_First, _Last, _Where._Ptr);
				_CATCH_ALL
					// glue the broken pieces back together

					_TRY_BEGIN
					_Umove(_Where._Ptr + _Count, _Where._Ptr + 2 * _Count, _Where._Ptr);
				_CATCH_ALL
					// vaporize the detached piece
					_Orphan_range(_Where._Ptr, _Oldlast);
				_Destroy(_Where._Ptr + _Count, this->_Mylast());
				this->_Mylast() = _Where._Ptr;
				_RERAISE;
				_CATCH_END

					_Move_unchecked(_Where._Ptr + 2 * _Count, this->_Mylast(), _Where._Ptr + _Count);
				_Destroy(_Oldlast, this->_Mylast());
				this->_Mylast() = _Oldlast;
				_RERAISE;
				_CATCH_END
			}
			else
			{	// affected elements don't overlap before/after
				const pointer _Relocated = _Where._Ptr + _Count;
				this->_Mylast() = _Umove(_Where._Ptr, _Oldlast, _Relocated);
				_Destroy(_Where._Ptr, _Oldlast);

				_TRY_BEGIN
					_Ucopy(_First, _Last, _Where._Ptr);
				_CATCH_ALL
					// glue the broken pieces back together

					_TRY_BEGIN
					_Umove(_Relocated, this->_Mylast(), _Where._Ptr);
				_CATCH_ALL
					// vaporize the detached piece
					_Orphan_range(_Where._Ptr, _Oldlast);
				_Destroy(_Relocated, this->_Mylast());
				this->_Mylast() = _Where._Ptr;
				_RERAISE;
				_CATCH_END

					_Destroy(_Relocated, this->_Mylast());
				this->_Mylast() = _Oldlast;
				_RERAISE;
				_CATCH_END
			}

			_Orphan_range(_Where._Ptr, _Oldlast);
		}
	}

public:
	template<class _Iter,
		class = enable_if_t<_Is_iterator_v<_Iter>>>
		iterator insert(const_iterator _Where, _Iter _First, _Iter _Last)
	{	// insert [_First, _Last) at _Where
#if _ITERATOR_DEBUG_LEVEL == 2
		_STL_VERIFY(_Where._Getcont() == _STD addressof(this->_Get_data())
			&& _Where._Ptr >= this->_Myfirst()
			&& this->_Mylast() >= _Where._Ptr, "vector insert iterator outside range");
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */

		_Adl_verify_range(_First, _Last);
		const size_type _Whereoff = static_cast<size_type>(_Where._Ptr - this->_Myfirst());
		_Insert_range(_Where, _Get_unwrapped(_First), _Get_unwrapped(_Last), _Iter_cat_t<_Iter>{});
		return (this->_Make_iterator_offset(_Whereoff));
	}

	iterator insert(const_iterator _Where, initializer_list<_Ty> _Ilist)
	{	// insert initializer_list at _Where
		return (insert(_Where, _Ilist.begin(), _Ilist.end()));
	}

	void assign(_CRT_GUARDOVERFLOW const size_type _Newsize, const _Ty& _Val)
	{	// assign _Newsize * _Val
		this->_Orphan_all();

		const size_type _Oldsize = size();
		const size_type _Oldcapacity = capacity();

		if (_Newsize > _Oldcapacity)
		{	// reallocate
			if (_Newsize > max_size())
			{
				_Xlength();
			}

			const size_type _Newcapacity = _Calculate_growth(_Newsize);

			if (this->_Myfirst() != pointer())
			{	// destroy and deallocate old array
				_Destroy(this->_Myfirst(), this->_Mylast());
				this->_Getal().deallocate(this->_Myfirst(), _Oldcapacity);
			}

			_Buy(_Newcapacity);

			this->_Mylast() = _Ufill(this->_Myfirst(), _Newsize, _Val);
		}
		else if (_Newsize > _Oldsize)
		{
			_Fill_unchecked(this->_Myfirst(), this->_Mylast(), _Val);
			this->_Mylast() = _Ufill(this->_Mylast(), _Newsize - _Oldsize, _Val);
		}
		else
		{
			const pointer _Newlast = this->_Myfirst() + _Newsize;
			_Fill_unchecked(this->_Myfirst(), _Newlast, _Val);
			_Destroy(_Newlast, this->_Mylast());
			this->_Mylast() = _Newlast;
		}
	}

private:
	template<class _Iter>
	void _Assign_range(_Iter _First, _Iter _Last, input_iterator_tag)
	{	// assign [_First, _Last), input iterators
		this->_Orphan_all();

		pointer _Next = this->_Myfirst();

		for (; _First != _Last && _Next != this->_Mylast(); ++_First, (void)++_Next)
		{
			*_Next = *_First;
		}

		// Code size optimization: we've exhausted only the source, only the dest, or both.
		// If we've exhausted only the source: we Trim, then Append does nothing.
		// If we've exhausted only the dest: Trim does nothing, then we Append.
		// If we've exhausted both: Trim does nothing, then Append does nothing.

		// Trim.
		_Destroy(_Next, this->_Mylast());
		this->_Mylast() = _Next;

		// Append.
		for (; _First != _Last; ++_First)
		{
			emplace_back(*_First);	// performance note: emplace_back()'s strong guarantee is unnecessary here
		}
	}

	template<class _Iter>
	void _Assign_range(_Iter _First, _Iter _Last, forward_iterator_tag)
	{	// assign [_First, _Last), forward iterators
		const size_type _Newsize = _Convert_size<size_type>(static_cast<size_t>(_STD distance(_First, _Last)));

		this->_Orphan_all();

		const size_type _Oldsize = size();
		const size_type _Oldcapacity = capacity();

		if (_Newsize > _Oldcapacity)
		{	// reallocate
			if (_Newsize > max_size())
			{
				_Xlength();
			}

			const size_type _Newcapacity = _Calculate_growth(_Newsize);

			if (this->_Myfirst() != pointer())
			{	// destroy and deallocate old array
				_Destroy(this->_Myfirst(), this->_Mylast());
				this->_Getal().deallocate(this->_Myfirst(), _Oldcapacity);
			}

			_Buy(_Newcapacity);

			this->_Mylast() = _Ucopy(_First, _Last, this->_Myfirst());
		}
		else if (_Newsize > _Oldsize)
		{
			// performance note: traversing [_First, _Mid) twice
			const _Iter _Mid = _STD next(_First, static_cast<difference_type>(_Oldsize));
			_Copy_unchecked(_First, _Mid, this->_Myfirst());
			this->_Mylast() = _Ucopy(_Mid, _Last, this->_Mylast());
		}
		else
		{
			const pointer _Newlast = this->_Myfirst() + _Newsize;
			_Copy_unchecked(_First, _Last, this->_Myfirst());
			_Destroy(_Newlast, this->_Mylast());
			this->_Mylast() = _Newlast;
		}
	}

public:
	template<class _Iter,
		class = enable_if_t<_Is_iterator_v<_Iter>>>
		void assign(_Iter _First, _Iter _Last)
	{	// assign [_First, _Last)
		_Adl_verify_range(_First, _Last);
		_Assign_range(_Get_unwrapped(_First), _Get_unwrapped(_Last), _Iter_cat_t<_Iter>{});
	}

	void assign(initializer_list<_Ty> _Ilist)
	{	// assign initializer_list
		_Assign_range(_Ilist.begin(), _Ilist.end(), random_access_iterator_tag{});
	}

	vector& operator=(const vector& _Right)
	{	// assign _Right
		if (this != _STD addressof(_Right))
		{	// different, assign it
#pragma warning(push)
#pragma warning(disable: 4127)	// conditional expression is constant
			if (_Alty_traits::propagate_on_container_copy_assignment::value
				&& this->_Getal() != _Right._Getal())
			{	// reload array
				_Tidy();
			}
#pragma warning(pop)

			this->_Copy_alloc(_Right._Getal());

			assign(_Right._Myfirst(), _Right._Mylast());
		}

		return (*this);
	}

	vector& operator=(initializer_list<_Ty> _Ilist)
	{	// assign initializer_list
		_Assign_range(_Ilist.begin(), _Ilist.end(), random_access_iterator_tag{});
		return (*this);
	}

private:
	template<class _Lambda>
	void _Resize(const size_type _Newsize, _Lambda _Udefault_or_fill)
	{	// trim or append elements, provide strong guarantee
		const size_type _Oldsize = size();
		const size_type _Oldcapacity = capacity();

		if (_Newsize > _Oldcapacity)
		{	// reallocate
			if (_Newsize > max_size())
			{
				_Xlength();
			}

			const size_type _Newcapacity = _Calculate_growth(_Newsize);

			const pointer _Newvec = this->_Getal().allocate(_Newcapacity);
			const pointer _Appended_first = _Newvec + _Oldsize;
			pointer _Appended_last = _Appended_first;

			_TRY_BEGIN
				_Appended_last = _Udefault_or_fill(_Appended_first, _Newsize - _Oldsize);
			_Umove_if_noexcept(this->_Myfirst(), this->_Mylast(), _Newvec);
			_CATCH_ALL
				_Destroy(_Appended_first, _Appended_last);
			this->_Getal().deallocate(_Newvec, _Newcapacity);
			_RERAISE;
			_CATCH_END

				_Change_array(_Newvec, _Newsize, _Newcapacity);
		}
		else if (_Newsize > _Oldsize)
		{	// append
			const pointer _Oldlast = this->_Mylast();
			this->_Mylast() = _Udefault_or_fill(_Oldlast, _Newsize - _Oldsize);
			_Orphan_range(_Oldlast, _Oldlast);
		}
		else if (_Newsize == _Oldsize)
		{	// nothing to do, avoid invalidating iterators
		}
		else
		{	// trim
			const pointer _Newlast = this->_Myfirst() + _Newsize;
			_Orphan_range(_Newlast, this->_Mylast());
			_Destroy(_Newlast, this->_Mylast());
			this->_Mylast() = _Newlast;
		}
	}

public:
	void resize(_CRT_GUARDOVERFLOW const size_type _Newsize)
	{	// trim or append value-initialized elements, provide strong guarantee
		const auto _Lambda_default = [this](pointer _Dest, const size_type _Count)
		{
			return (_Udefault(_Dest, _Count));
		};

		_Resize(_Newsize, _Lambda_default);
	}

	void resize(_CRT_GUARDOVERFLOW const size_type _Newsize, const _Ty& _Val)
	{	// trim or append copies of _Val, provide strong guarantee
		auto _Lambda_fill = [this, &_Val](pointer _Dest, const size_type _Count)
		{
			return (_Ufill(_Dest, _Count, _Val));
		};

		_Resize(_Newsize, _Lambda_fill);
	}

private:
	void _Reallocate_exactly(const size_type _Newcapacity)
	{	// set capacity to _Newcapacity (without geometric growth), provide strong guarantee
		const size_type _Size = size();

		const pointer _Newvec = this->_Getal().allocate(_Newcapacity);

		_TRY_BEGIN
			_Umove_if_noexcept(this->_Myfirst(), this->_Mylast(), _Newvec);
		_CATCH_ALL
			this->_Getal().deallocate(_Newvec, _Newcapacity);
		_RERAISE;
		_CATCH_END

			_Change_array(_Newvec, _Size, _Newcapacity);
	}

public:
	void reserve(_CRT_GUARDOVERFLOW const size_type _Newcapacity)
	{	// increase capacity to _Newcapacity (without geometric growth), provide strong guarantee
		if (_Newcapacity > capacity())
		{	// something to do (reserve() never shrinks)
			if (_Newcapacity > max_size())
			{
				_Xlength();
			}

			_Reallocate_exactly(_Newcapacity);
		}
	}

	void shrink_to_fit()
	{	// reduce capacity to size, provide strong guarantee
		if (_Has_unused_capacity())
		{	// something to do
			if (empty())
			{
				_Tidy();
			}
			else
			{
				_Reallocate_exactly(size());
			}
		}
	}

	void pop_back()
	{	// erase element at end
#if _ITERATOR_DEBUG_LEVEL == 2
		_STL_VERIFY(!empty(), "vector empty before pop");
		_Orphan_range(this->_Mylast() - 1, this->_Mylast());
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */

		_Alty_traits::destroy(this->_Getal(), _Unfancy(this->_Mylast() - 1));
		--this->_Mylast();
	}

	iterator erase(const_iterator _Where)
	{	// erase element at _Where
#if _ITERATOR_DEBUG_LEVEL == 2
		_STL_VERIFY(_Where._Getcont() == _STD addressof(this->_Get_data())
			&& _Where._Ptr >= this->_Myfirst()
			&& this->_Mylast() > _Where._Ptr, "vector erase iterator outside range");
		_Orphan_range(_Where._Ptr, this->_Mylast());
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */

		_Move_unchecked(_Where._Ptr + 1, this->_Mylast(), _Where._Ptr);
		_Alty_traits::destroy(this->_Getal(), _Unfancy(this->_Mylast() - 1));
		--this->_Mylast();
		return (iterator(_Where._Ptr, _STD addressof(this->_Get_data())));
	}

	iterator erase(const_iterator _First, const_iterator _Last)
	{	// erase [_First, _Last)
#if _ITERATOR_DEBUG_LEVEL == 2
		_STL_VERIFY(_First._Getcont() == _STD addressof(this->_Get_data())
			&& _Last._Getcont() == _STD addressof(this->_Get_data())
			&& _First._Ptr >= this->_Myfirst()
			&& _Last._Ptr >= _First._Ptr
			&& this->_Mylast() >= _Last._Ptr, "vector erase iterator outside range");
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */

		if (_First._Ptr != _Last._Ptr)
		{	// something to do, invalidate iterators
			_Orphan_range(_First._Ptr, this->_Mylast());
			const pointer _Newlast = _Move_unchecked(_Last._Ptr, this->_Mylast(), _First._Ptr);
			_Destroy(_Newlast, this->_Mylast());
			this->_Mylast() = _Newlast;
		}

		return (iterator(_First._Ptr, _STD addressof(this->_Get_data())));
	}

	void clear() noexcept
	{	// erase all
		this->_Orphan_all();
		_Destroy(this->_Myfirst(), this->_Mylast());
		this->_Mylast() = this->_Myfirst();
	}

	void swap(vector& _Right) noexcept // strengthened
	{	// exchange contents with _Right
		if (this != _STD addressof(_Right))
		{	// (maybe) swap allocators, swap control information
			_Pocs(this->_Getal(), _Right._Getal());
			this->_Swap_all(_Right);
			_Swap_adl(this->_Myfirst(), _Right._Myfirst());
			_Swap_adl(this->_Mylast(), _Right._Mylast());
			_Swap_adl(this->_Myend(), _Right._Myend());
		}
	}

	_NODISCARD _Ty * data() noexcept
	{	// return address of first element
		return (_Unfancy_maybe_null(this->_Myfirst()));
	}

	_NODISCARD const _Ty * data() const noexcept
	{	// return address of first element
		return (_Unfancy_maybe_null(this->_Myfirst()));
	}

	_NODISCARD iterator begin() noexcept
	{	// return iterator for beginning of mutable sequence
		return (iterator(this->_Myfirst(), _STD addressof(this->_Get_data())));
	}

	_NODISCARD const_iterator begin() const noexcept
	{	// return iterator for beginning of nonmutable sequence
		return (const_iterator(this->_Myfirst(), _STD addressof(this->_Get_data())));
	}

	_NODISCARD iterator end() noexcept
	{	// return iterator for end of mutable sequence
		return (iterator(this->_Mylast(), _STD addressof(this->_Get_data())));
	}

	_NODISCARD const_iterator end() const noexcept
	{	// return iterator for end of nonmutable sequence
		return (const_iterator(this->_Mylast(), _STD addressof(this->_Get_data())));
	}

	_NODISCARD reverse_iterator rbegin() noexcept
	{	// return iterator for beginning of reversed mutable sequence
		return (reverse_iterator(end()));
	}

	_NODISCARD const_reverse_iterator rbegin() const noexcept
	{	// return iterator for beginning of reversed nonmutable sequence
		return (const_reverse_iterator(end()));
	}

	_NODISCARD reverse_iterator rend() noexcept
	{	// return iterator for end of reversed mutable sequence
		return (reverse_iterator(begin()));
	}

	_NODISCARD const_reverse_iterator rend() const noexcept
	{	// return iterator for end of reversed nonmutable sequence
		return (const_reverse_iterator(begin()));
	}

	_NODISCARD const_iterator cbegin() const noexcept
	{	// return iterator for beginning of nonmutable sequence
		return (begin());
	}

	_NODISCARD const_iterator cend() const noexcept
	{	// return iterator for end of nonmutable sequence
		return (end());
	}

	_NODISCARD const_reverse_iterator crbegin() const noexcept
	{	// return iterator for beginning of reversed nonmutable sequence
		return (rbegin());
	}

	_NODISCARD const_reverse_iterator crend() const noexcept
	{	// return iterator for end of reversed nonmutable sequence
		return (rend());
	}

	pointer _Unchecked_begin() noexcept
	{	// return pointer for beginning of mutable sequence
		return (this->_Myfirst());
	}

	const_pointer _Unchecked_begin() const noexcept
	{	// return pointer for beginning of nonmutable sequence
		return (this->_Myfirst());
	}

	pointer _Unchecked_end() noexcept
	{	// return pointer for end of mutable sequence
		return (this->_Mylast());
	}

	const_pointer _Unchecked_end() const noexcept
	{	// return pointer for end of nonmutable sequence
		return (this->_Mylast());
	}

	_NODISCARD bool empty() const noexcept
	{	// test if sequence is empty
		return (this->_Myfirst() == this->_Mylast());
	}

	_NODISCARD size_type size() const noexcept
	{	// return length of sequence
		return (static_cast<size_type>(this->_Mylast() - this->_Myfirst()));
	}

	_NODISCARD size_type max_size() const noexcept
	{	// return maximum possible length of sequence
		return (_Min_value(static_cast<size_type>((numeric_limits<difference_type>::max)()),
			_Alty_traits::max_size(this->_Getal())));
	}

	_NODISCARD size_type capacity() const noexcept
	{	// return current length of allocated storage
		return (static_cast<size_type>(this->_Myend() - this->_Myfirst()));
	}

private:
	size_type _Unused_capacity() const noexcept
	{	// micro-optimization for capacity() - size()
		return (static_cast<size_type>(this->_Myend() - this->_Mylast()));
	}

	bool _Has_unused_capacity() const noexcept
	{	// micro-optimization for capacity() != size()
		return (this->_Myend() != this->_Mylast());
	}

public:
	_NODISCARD _Ty& operator[](const size_type _Pos)
	{	// subscript mutable sequence
#if _ITERATOR_DEBUG_LEVEL != 0
		_STL_VERIFY(_Pos < size(), "vector subscript out of range");
#endif /* _ITERATOR_DEBUG_LEVEL != 0 */

		return (this->_Myfirst()[_Pos]);
	}

	_NODISCARD const _Ty& operator[](const size_type _Pos) const
	{	// subscript nonmutable sequence
#if _ITERATOR_DEBUG_LEVEL != 0
		_STL_VERIFY(_Pos < size(), "vector subscript out of range");
#endif /* _ITERATOR_DEBUG_LEVEL != 0 */

		return (this->_Myfirst()[_Pos]);
	}

	_NODISCARD _Ty& at(const size_type _Pos)
	{	// subscript mutable sequence with checking
		if (size() <= _Pos)
		{
			_Xrange();
		}

		return (this->_Myfirst()[_Pos]);
	}

	_NODISCARD const _Ty& at(const size_type _Pos) const
	{	// subscript nonmutable sequence with checking
		if (size() <= _Pos)
		{
			_Xrange();
		}

		return (this->_Myfirst()[_Pos]);
	}

	_NODISCARD _Ty& front()
	{	// return first element of mutable sequence
#if _ITERATOR_DEBUG_LEVEL != 0
		_STL_VERIFY(!empty(), "front() called on empty vector");
#endif /* _ITERATOR_DEBUG_LEVEL != 0 */

		return (*this->_Myfirst());
	}

	_NODISCARD const _Ty& front() const
	{	// return first element of nonmutable sequence
#if _ITERATOR_DEBUG_LEVEL != 0
		_STL_VERIFY(!empty(), "front() called on empty vector");
#endif /* _ITERATOR_DEBUG_LEVEL != 0 */

		return (*this->_Myfirst());
	}

	_NODISCARD _Ty& back()
	{	// return last element of mutable sequence
#if _ITERATOR_DEBUG_LEVEL != 0
		_STL_VERIFY(!empty(), "back() called on empty vector");
#endif /* _ITERATOR_DEBUG_LEVEL != 0 */

		return (this->_Mylast()[-1]);
	}

	_NODISCARD const _Ty& back() const
	{	// return last element of nonmutable sequence
#if _ITERATOR_DEBUG_LEVEL != 0
		_STL_VERIFY(!empty(), "back() called on empty vector");
#endif /* _ITERATOR_DEBUG_LEVEL != 0 */

		return (this->_Mylast()[-1]);
	}

	_NODISCARD allocator_type get_allocator() const noexcept
	{	// return allocator object for values
		return (static_cast<allocator_type>(this->_Getal()));
	}

private:
	pointer _Udefault(pointer _Dest, const size_type _Count)
	{	// fill raw _Dest with _Count value-initialized objects, using allocator
		return (_Uninitialized_value_construct_n(_Dest, _Count, this->_Getal()));
	}

	pointer _Ufill(pointer _Dest, const size_type _Count, const _Ty& _Val)
	{	// fill raw _Dest with _Count copies of _Val, using allocator
		return (_Uninitialized_fill_n(_Dest, _Count, _Val, this->_Getal()));
	}

	template<class _Iter>
	pointer _Ucopy(_Iter _First, _Iter _Last, pointer _Dest)
	{	// copy [_First, _Last) to raw _Dest, using allocator
		return (_Uninitialized_copy(_First, _Last, _Dest, this->_Getal()));
	}

	pointer _Umove(pointer _First, pointer _Last, pointer _Dest)
	{	// move [_First, _Last) to raw _Dest, using allocator
		return (_Uninitialized_move(_First, _Last, _Dest, this->_Getal()));
	}

	void _Umove_if_noexcept1(pointer _First, pointer _Last, pointer _Dest, true_type)
	{	// move [_First, _Last) to raw _Dest, using allocator
		_Uninitialized_move(_First, _Last, _Dest, this->_Getal());
	}

	void _Umove_if_noexcept1(pointer _First, pointer _Last, pointer _Dest, false_type)
	{	// copy [_First, _Last) to raw _Dest, using allocator
		_Uninitialized_copy(_First, _Last, _Dest, this->_Getal());
	}

	void _Umove_if_noexcept(pointer _First, pointer _Last, pointer _Dest)
	{	// move_if_noexcept [_First, _Last) to raw _Dest, using allocator
		_Umove_if_noexcept1(_First, _Last, _Dest,
			bool_constant<disjunction_v<is_nothrow_move_constructible<_Ty>, negation<is_copy_constructible<_Ty>>>>{});
	}

	void _Destroy(pointer _First, pointer _Last)
	{	// destroy [_First, _Last) using allocator
		_Destroy_range(_First, _Last, this->_Getal());
	}

	size_type _Calculate_growth(const size_type _Newsize) const
	{	// given _Oldcapacity and _Newsize, calculate geometric growth
		const size_type _Oldcapacity = capacity();

		if (_Oldcapacity > max_size() - _Oldcapacity / 2)
		{
			return (_Newsize);	// geometric growth would overflow
		}

		const size_type _Geometric = _Oldcapacity + _Oldcapacity / 2;

		if (_Geometric < _Newsize)
		{
			return (_Newsize);	// geometric growth would be insufficient
		}

		return (_Geometric);	// geometric growth is sufficient
	}

	bool _Buy(const size_type _Newcapacity)
	{	// allocate array with _Newcapacity elements
		this->_Myfirst() = pointer();
		this->_Mylast() = pointer();
		this->_Myend() = pointer();

		if (_Newcapacity == 0)
		{
			return (false);
		}

		if (_Newcapacity > max_size())
		{
			_Xlength();
		}

		this->_Myfirst() = this->_Getal().allocate(_Newcapacity);
		this->_Mylast() = this->_Myfirst();
		this->_Myend() = this->_Myfirst() + _Newcapacity;

		return (true);
	}

	void _Change_array(const pointer _Newvec, const size_type _Newsize, const size_type _Newcapacity)
	{	// orphan all iterators, discard old array, acquire new array
		this->_Orphan_all();

		if (this->_Myfirst() != pointer())
		{	// destroy and deallocate old array
			_Destroy(this->_Myfirst(), this->_Mylast());
			this->_Getal().deallocate(this->_Myfirst(), capacity());
		}

		this->_Myfirst() = _Newvec;
		this->_Mylast() = _Newvec + _Newsize;
		this->_Myend() = _Newvec + _Newcapacity;
	}

	void _Tidy()
	{	// free all storage
		this->_Orphan_all();

		if (this->_Myfirst() != pointer())
		{	// destroy and deallocate old array
			_Destroy(this->_Myfirst(), this->_Mylast());
			this->_Getal().deallocate(this->_Myfirst(), capacity());

			this->_Myfirst() = pointer();
			this->_Mylast() = pointer();
			this->_Myend() = pointer();
		}
	}

	[[noreturn]] static void _Xlength()
	{	// report a length_error
		_Xlength_error("vector<T> too long");
	}

	[[noreturn]] static void _Xrange()
	{	// report an out_of_range error
		_Xout_of_range("invalid vector<T> subscript");
	}

#if _ITERATOR_DEBUG_LEVEL == 2
	void _Orphan_range(pointer _First, pointer _Last) const
	{	// orphan iterators within specified (inclusive) range
		_Lockit _Lock(_LOCK_DEBUG);

		const_iterator ** _Pnext = reinterpret_cast<const_iterator **>(this->_Getpfirst());

		if (_Pnext)
		{
			while (*_Pnext)
			{
				if ((*_Pnext)->_Ptr < _First || _Last < (*_Pnext)->_Ptr)
				{	// skip the iterator
					_Pnext = reinterpret_cast<const_iterator **>((*_Pnext)->_Getpnext());
				}
				else
				{	// orphan the iterator
					(*_Pnext)->_Clrcont();
					*_Pnext = *reinterpret_cast<const_iterator **>((*_Pnext)->_Getpnext());
				}
			}
		}
	}
#else /* _ITERATOR_DEBUG_LEVEL == 2 */
	void _Orphan_range(pointer, pointer) const
	{	// orphan iterators within specified (inclusive) range
	}
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */
};

#if _HAS_CXX17
template<class _Iter,
	class _Alloc = allocator<_Iter_value_t<_Iter>>,
	enable_if_t<conjunction_v<
	_Is_iterator<_Iter>,
	_Is_allocator<_Alloc>
>, int> = 0>
vector(_Iter, _Iter, _Alloc = _Alloc())
->vector<_Iter_value_t<_Iter>, _Alloc>;
#endif /* _HAS_CXX17 */

template<class _Ty,
	class _Alloc> inline
	void swap(vector<_Ty, _Alloc>& _Left, vector<_Ty, _Alloc>& _Right) noexcept // strengthened
{	// swap _Left and _Right vectors
	_Left.swap(_Right);
}

template<class _Ty,
	class _Alloc>
	_NODISCARD inline bool operator==(const vector<_Ty, _Alloc>& _Left,
		const vector<_Ty, _Alloc>& _Right)
{	// test for vector equality
	return (_Left.size() == _Right.size()
		&& _STD equal(_Left.begin(), _Left.end(), _Right.begin()));
}

template<class _Ty,
	class _Alloc>
	_NODISCARD inline bool operator!=(const vector<_Ty, _Alloc>& _Left,
		const vector<_Ty, _Alloc>& _Right)
{	// test for vector inequality
	return (!(_Left == _Right));
}

template<class _Ty,
	class _Alloc>
	_NODISCARD inline bool operator<(const vector<_Ty, _Alloc>& _Left,
		const vector<_Ty, _Alloc>& _Right)
{	// test if _Left < _Right for vectors
	return (_STD lexicographical_compare(_Left.begin(), _Left.end(),
		_Right.begin(), _Right.end()));
}

template<class _Ty,
	class _Alloc>
	_NODISCARD inline bool operator>(const vector<_Ty, _Alloc>& _Left,
		const vector<_Ty, _Alloc>& _Right)
{	// test if _Left > _Right for vectors
	return (_Right < _Left);
}

template<class _Ty,
	class _Alloc>
	_NODISCARD inline bool operator<=(const vector<_Ty, _Alloc>& _Left,
		const vector<_Ty, _Alloc>& _Right)
{	// test if _Left <= _Right for vectors
	return (!(_Right < _Left));
}

template<class _Ty,
	class _Alloc>
	_NODISCARD inline bool operator>=(const vector<_Ty, _Alloc>& _Left,
		const vector<_Ty, _Alloc>& _Right)
{	// test if _Left >= _Right for vectors
	return (!(_Left < _Right));
}

// CLASS TEMPLATE vector<bool, Alloc> AND FRIENDS
using _Vbase = unsigned int;	// word type for vector<bool> representation
constexpr int _VBITS = 8 * sizeof(_Vbase);	// at least CHAR_BITS bits per word

template<class _Alloc0>
struct _Wrap_alloc
{	// TRANSITION, ABI compat, preserves symbol names of vector<bool>::iterator
	using _Alloc = _Alloc0;
};

// CLASS _Vb_iter_base
template<class _Alvbase_wrapped>
class _Vb_iter_base
	: public _Iterator_base
{	// store information common to reference and iterators
public:
	using _Alvbase = typename _Alvbase_wrapped::_Alloc;
	using _Sizet = typename allocator_traits<_Alvbase>::size_type;
	using _Difft = typename allocator_traits<_Alvbase>::difference_type;
	using _Mycont = vector<bool, _Rebind_alloc_t<_Alvbase, bool>>;

	_Vb_iter_base()
		: _Myptr(nullptr), _Myoff(0)
	{	// construct with null pointer
	}

	_Vb_iter_base(const _Vbase *_Ptr, _Sizet _Off,
		const _Container_base *_Mypvbool)
		: _Myptr(_Ptr), _Myoff(_Off)
	{	// construct with offset and pointer
		this->_Adopt(_Mypvbool);
	}

	void _Advance(_Sizet _Off)
	{	// advance iterator by _Off
		_Myoff += _Off;
		_Myptr += _Myoff / _VBITS;
		_Myoff %= _VBITS;
	}

	int _Valid(_Sizet _Inc) const
	{	// test for valid incremented offset
#if _ITERATOR_DEBUG_LEVEL == 2
		const auto _Cont = static_cast<const _Mycont *>(this->_Getcont());
		_Sizet _Mysize = _Cont->_Mysize;

		_Inc += _Myoff;
		_Inc += static_cast<_Sizet>(_VBITS * (_Myptr - _Cont->_Myvec.data()));
		return (_Inc < _Mysize ? -1 : _Inc == _Mysize ? 0 : +1);

#else /* _ITERATOR_DEBUG_LEVEL == 2 */
		(void)_Inc;
		return (-1);
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */
	}

	const _Vbase *_Myptr;
	_Sizet _Myoff;
};

// CLASS _Vb_reference
template<class _Alvbase_wrapped>
class _Vb_reference
	: public _Vb_iter_base<_Alvbase_wrapped>
{	// reference to a bit within a base word
	using _Mybase = _Vb_iter_base<_Alvbase_wrapped>;

	_Vb_reference() noexcept
	{	// construct with null pointer (private)
	}

public:
	_Vb_reference(const _Mybase& _Right)
		: _Mybase(_Right._Myptr, _Right._Myoff, _Right._Getcont())
	{	// construct with base
	}

	_Vb_reference& operator=(const _Vb_reference& _Right) noexcept
	{	// assign _Vb_reference _Right to bit
		return (*this = bool(_Right));
	}

	_Vb_reference& operator=(bool _Val) noexcept
	{	// assign _Val to bit
		if (_Val)
			*const_cast<_Vbase *>(_Getptr()) |= _Mask();
		else
			*const_cast<_Vbase *>(_Getptr()) &= ~_Mask();
		return (*this);
	}

	void flip() noexcept
	{	// toggle the bit
		*const_cast<_Vbase *>(_Getptr()) ^= _Mask();
	}

	operator bool() const noexcept
	{	// test if bit is set
		return ((*_Getptr() & _Mask()) != 0);
	}

	const _Vbase *_Getptr() const
	{	// get pointer to base word
#if _ITERATOR_DEBUG_LEVEL != 0
		_STL_VERIFY(this->_Getcont() && this->_Myptr && this->_Valid(0) < 0,
			"vector<bool> iterator not dereferencable");
#endif /* _ITERATOR_DEBUG_LEVEL != 0 */

		return (this->_Myptr);
	}

protected:
	_Vbase _Mask() const
	{	// convert offset to mask
		return (static_cast<_Vbase>(1) << this->_Myoff);
	}
};

template<class _Alvbase_wrapped> inline
void swap(_Vb_reference<_Alvbase_wrapped> _Left, _Vb_reference<_Alvbase_wrapped> _Right) noexcept
{	// swap _Left and _Right vector<bool> elements
	bool _Val = _Left;	// NOT _STD swap
	_Left = _Right;
	_Right = _Val;
}

// CLASS _Vb_const_iterator
template<class _Alvbase_wrapped>
class _Vb_const_iterator
	: public _Vb_iter_base<_Alvbase_wrapped>
{	// iterator for nonmutable vector<bool>
private:
	using _Mybase = _Vb_iter_base<_Alvbase_wrapped>;
	using _Size_type = typename _Mybase::_Sizet;

public:
	using _Reft = _Vb_reference<_Alvbase_wrapped>;
	using const_reference = bool;

	using iterator_category = random_access_iterator_tag;
	using value_type = bool;
	using difference_type = typename _Mybase::_Difft;
	using pointer = const_reference * ;
	using reference = const_reference;

	_Vb_const_iterator()
	{	// construct with null reference
	}

	_Vb_const_iterator(const _Vbase *_Ptr, const _Container_base *_Mypvbool)
		: _Mybase(_Ptr, 0, _Mypvbool)
	{	// construct with offset and pointer
	}

	_NODISCARD const_reference operator*() const
	{	// return (reference to) designated object
		return (_Reft(*this));
	}

	_Vb_const_iterator& operator++()
	{	// preincrement
		_Inc();
		return (*this);
	}

	_Vb_const_iterator operator++(int)
	{	// postincrement
		_Vb_const_iterator _Tmp = *this;
		++*this;
		return (_Tmp);
	}

	_Vb_const_iterator& operator--()
	{	// predecrement
		_Dec();
		return (*this);
	}

	_Vb_const_iterator operator--(int)
	{	// postdecrement
		_Vb_const_iterator _Tmp = *this;
		--*this;
		return (_Tmp);
	}

	_Vb_const_iterator& operator+=(const difference_type _Off)
	{	// increment by integer
		if (_Off < 0 && this->_Myoff < 0 - static_cast<_Size_type>(_Off))
		{	/* add negative increment */
			this->_Myoff += _Off;
			this->_Myptr -= 1 + (static_cast<_Size_type>(-1) - this->_Myoff) / _VBITS;
			this->_Myoff %= _VBITS;
		}
		else
		{	/* add non-negative increment */
			this->_Myoff += _Off;
			this->_Myptr += this->_Myoff / _VBITS;
			this->_Myoff %= _VBITS;
		}
		return (*this);
	}

	_NODISCARD _Vb_const_iterator operator+(const difference_type _Off) const
	{	// return this + integer
		_Vb_const_iterator _Tmp = *this;
		return (_Tmp += _Off);
	}

	_Vb_const_iterator& operator-=(const difference_type _Off)
	{	// decrement by integer
		return (*this += -_Off);
	}

	_NODISCARD _Vb_const_iterator operator-(const difference_type _Off) const
	{	// return this - integer
		_Vb_const_iterator _Tmp = *this;
		return (_Tmp -= _Off);
	}

	_NODISCARD difference_type operator-(const _Vb_const_iterator& _Right) const
	{	// return difference of iterators
		_Compat(_Right);
		return (static_cast<difference_type>(_VBITS * (this->_Myptr - _Right._Myptr))
			+ static_cast<difference_type>(this->_Myoff)
			- static_cast<difference_type>(_Right._Myoff));
	}

	_NODISCARD const_reference operator[](const difference_type _Off) const
	{	// subscript
		return (*(*this + _Off));
	}

	_NODISCARD bool operator==(const _Vb_const_iterator& _Right) const
	{	// test for iterator equality
		_Compat(_Right);
		return (this->_Myptr == _Right._Myptr
			&& this->_Myoff == _Right._Myoff);
	}

	_NODISCARD bool operator!=(const _Vb_const_iterator& _Right) const
	{	// test for iterator inequality
		return (!(*this == _Right));
	}

	_NODISCARD bool operator<(const _Vb_const_iterator& _Right) const
	{	// test if this < _Right
		_Compat(_Right);
		return (this->_Myptr < _Right._Myptr
			|| (this->_Myptr == _Right._Myptr
				&& this->_Myoff < _Right._Myoff));
	}

	_NODISCARD bool operator>(const _Vb_const_iterator& _Right) const
	{	// test if this > _Right
		return (_Right < *this);
	}

	_NODISCARD bool operator<=(const _Vb_const_iterator& _Right) const
	{	// test if this <= _Right
		return (!(_Right < *this));
	}

	_NODISCARD bool operator>=(const _Vb_const_iterator& _Right) const
	{	// test if this >= _Right
		return (!(*this < _Right));
	}

	void _Compat(const _Vb_const_iterator& _Right) const
	{	// test for compatible iterator pair
#if _ITERATOR_DEBUG_LEVEL == 0
		(void)_Right;
#else /* _ITERATOR_DEBUG_LEVEL == 0 */
		_STL_VERIFY(this->_Getcont() == _Right._Getcont(), "vector<bool> iterators incompatible");
#endif /* _ITERATOR_DEBUG_LEVEL */
	}

#if _ITERATOR_DEBUG_LEVEL != 0
	friend void _Verify_range(const _Vb_const_iterator& _First, const _Vb_const_iterator& _Last)
	{
		// note _Compat check inside <=
		_STL_VERIFY(_First <= _Last, "vector<bool> iterator range transposed");
	}
#endif /* _ITERATOR_DEBUG_LEVEL != 0 */

	void _Dec()
	{	// decrement bit position
		if (this->_Myoff != 0)
			--this->_Myoff;
		else
		{	// move to previous word
#if _ITERATOR_DEBUG_LEVEL != 0
			_STL_VERIFY(this->_Getcont(), "cannot decrement value-initialized vector<bool> iterator");
			_STL_VERIFY(this->_Valid(static_cast<_Size_type>(-1)) <= 0,
				"cannot decrement invalid vector<bool> iterator");
#endif /* _ITERATOR_DEBUG_LEVEL */

			this->_Myoff = _VBITS - 1;
			--this->_Myptr;
		}
	}

	void _Inc()
	{	// increment bit position
		if (this->_Myoff < _VBITS - 1)
			++this->_Myoff;
		else
		{	// move to next word
#if _ITERATOR_DEBUG_LEVEL != 0
			_STL_VERIFY(this->_Getcont(), "cannot increment value-initialized vector<bool> iterator");
			_STL_VERIFY(this->_Valid(1) <= 0,
				"cannot increment invalid vector<bool> iterator");
#endif /* _ITERATOR_DEBUG_LEVEL */

			this->_Myoff = 0;
			++this->_Myptr;
		}
	}
};

template<class _Alvbase_wrapped>
_NODISCARD _Vb_const_iterator<_Alvbase_wrapped> operator+(
	typename _Vb_const_iterator<_Alvbase_wrapped>::difference_type _Off,
	_Vb_const_iterator<_Alvbase_wrapped> _Right)
{	// return _Right + integer
	return (_Right += _Off);
}

// CLASS _Vb_iterator
template<class _Alvbase_wrapped>
class _Vb_iterator
	: public _Vb_const_iterator<_Alvbase_wrapped>
{	// iterator for mutable vector<bool>
public:
	using _Mybase = _Vb_const_iterator<_Alvbase_wrapped>;

	using _Reft = _Vb_reference<_Alvbase_wrapped>;
	using const_reference = bool;

	using iterator_category = random_access_iterator_tag;
	using value_type = bool;
	using difference_type = typename _Mybase::difference_type;
	using pointer = _Reft * ;
	using reference = _Reft;

	_Vb_iterator()
	{	// construct with null reference
	}

	_Vb_iterator(_Vbase *_Ptr, _Container_base *_Mypvbool)
		: _Mybase(_Ptr, _Mypvbool)
	{	// construct with offset and pointer
	}

	_NODISCARD reference operator*() const
	{	// return (reference to) designated object
		return (_Reft(*this));
	}

	_Vb_iterator& operator++()
	{	// preincrement
		++*(_Mybase *)this;
		return (*this);
	}

	_Vb_iterator operator++(int)
	{	// postincrement
		_Vb_iterator _Tmp = *this;
		++*this;
		return (_Tmp);
	}

	_Vb_iterator& operator--()
	{	// predecrement
		--*(_Mybase *)this;
		return (*this);
	}

	_Vb_iterator operator--(int)
	{	// postdecrement
		_Vb_iterator _Tmp = *this;
		--*this;
		return (_Tmp);
	}

	_Vb_iterator& operator+=(const difference_type _Off)
	{	// increment by integer
		*(_Mybase *)this += _Off;
		return (*this);
	}

	_NODISCARD _Vb_iterator operator+(const difference_type _Off) const
	{	// return this + integer
		_Vb_iterator _Tmp = *this;
		return (_Tmp += _Off);
	}

	_Vb_iterator& operator-=(const difference_type _Off)
	{	// decrement by integer
		return (*this += -_Off);
	}

	_NODISCARD _Vb_iterator operator-(const difference_type _Off) const
	{	// return this - integer
		_Vb_iterator _Tmp = *this;
		return (_Tmp -= _Off);
	}

	_NODISCARD difference_type operator-(const _Mybase& _Right) const
	{	// return difference of iterators
		return (*(_Mybase *)this - _Right);
	}

	_NODISCARD reference operator[](const difference_type _Off) const
	{	// subscript
		return (*(*this + _Off));
	}
};

template<class _Alvbase_wrapped>
_NODISCARD inline _Vb_iterator<_Alvbase_wrapped> operator+(
	typename _Vb_iterator<_Alvbase_wrapped>::difference_type _Off,
	_Vb_iterator<_Alvbase_wrapped> _Right)
{	// return _Right + integer
	return (_Right += _Off);
}

// CLASS TEMPLATE _Vb_val
template<class _Alloc>
class _Vb_val
	: public _Container_base
{	// base class for vector<bool> to hold data
public:
	using _Alproxy = _Rebind_alloc_t<_Alloc, _Container_proxy>;
	using _Alproxy_traits = allocator_traits<_Alproxy>;
	using _Alvbase = _Rebind_alloc_t<_Alloc, _Vbase>;
	using _Alvbase_traits = allocator_traits<_Alvbase>;
	using _Vectype = vector<_Vbase, _Alvbase>;
	using _Alvbase_wrapped = _Wrap_alloc<_Alvbase>;
	using size_type = typename _Alvbase_traits::size_type;

	_Vb_val() _NOEXCEPT_COND(is_nothrow_default_constructible_v<_Vectype>)
		: _Myvec()
	{	// construct empty vector
		_Alloc_proxy();
		_Mysize = 0;
	}

	_Vb_val(const _Alloc& _Al) _NOEXCEPT_COND(is_nothrow_constructible_v<_Vectype, _Alvbase>)
		: _Myvec(static_cast<_Alvbase>(_Al))
	{	// construct empty vector, allocator
		_Alloc_proxy();
		_Mysize = 0;
	}

	_Vb_val(size_type _Count, const bool& _Val)
		: _Myvec(_Nw(_Count), static_cast<_Vbase>(_Val ? -1 : 0))
	{	// construct _Count * _Val elements
		_Alloc_proxy();
		_Mysize = 0;
	}

	_Vb_val(size_type _Count, const bool& _Val, const _Alloc& _Al)
		: _Myvec(_Nw(_Count), static_cast<_Vbase>(_Val ? -1 : 0), static_cast<_Alvbase>(_Al))
	{	// construct _Count * _Val elements with allocator _Al
		_Alloc_proxy();
		_Mysize = 0;
	}

	_Vb_val(const _Vb_val& _Right)
		: _Myvec(_Right._Myvec),
		_Mysize(_Right._Mysize)
	{	// copy construct
		_Alloc_proxy();
	}

	_Vb_val(const _Vb_val& _Right, const _Alloc& _Al)
		: _Myvec(_Right._Myvec, static_cast<_Alvbase>(_Al)),
		_Mysize(_Right._Mysize)
	{	// copy construct, allocator
		_Alloc_proxy();
	}

	_Vb_val(_Vb_val&& _Right) _NOEXCEPT_COND(is_nothrow_move_constructible_v<_Vectype>)
		: _Myvec(_STD move(_Right._Myvec)),
		_Mysize(_Right._Mysize)
	{	// move construct
		_Right._Mysize = 0;
		_Alloc_proxy();
	}

	_Vb_val(_Vb_val&& _Right, const _Alloc& _Al)
		_NOEXCEPT_COND(is_nothrow_constructible_v<_Vectype, _Vectype, _Alvbase>)
		: _Myvec(_STD move(_Right._Myvec), static_cast<_Alvbase>(_Al)),
		_Mysize(_Right._Mysize)
	{	// move construct, allocator
		_Right._Mysize = 0;
		_Alloc_proxy();
	}

	~_Vb_val() noexcept
	{	// destroy proxy
		_Free_proxy();
	}

#if _ITERATOR_DEBUG_LEVEL == 0
	void _Alloc_proxy()
	{	// do nothing
	}

	void _Free_proxy()
	{	// do nothing
	}

#else /* _ITERATOR_DEBUG_LEVEL == 0 */
	void _Alloc_proxy()
	{	// allocate a proxy
		_Alproxy _Proxy_allocator(_Myvec._Getal());
		this->_Myproxy = _Unfancy(_Proxy_allocator.allocate(1));
		_Alproxy_traits::construct(_Proxy_allocator, this->_Myproxy, _Container_proxy());
		this->_Myproxy->_Mycont = this;
	}

	void _Free_proxy()
	{	// destroy proxy
		_Alproxy _Proxy_allocator(_Myvec._Getal());
		this->_Orphan_all();
		_Alproxy_traits::destroy(_Proxy_allocator, this->_Myproxy);
		_Deallocate_plain(_Proxy_allocator, this->_Myproxy);
		this->_Myproxy = nullptr;
	}
#endif /* _ITERATOR_DEBUG_LEVEL == 0 */

	static size_type _Nw(size_type _Count)
	{	// return number of base words from number of bits
		return ((_Count + _VBITS - 1) / _VBITS);
	}

	_Vectype _Myvec;	// base vector of words
	size_type _Mysize;	// current length of sequence
};

// CLASS vector<bool>
template<class _Alloc>
class vector<bool, _Alloc>
	: public _Vb_val<_Alloc>
{	// varying size array of bits
public:
	static_assert(!_ENFORCE_MATCHING_ALLOCATORS || is_same_v<bool, typename _Alloc::value_type>,
		_MISMATCHED_ALLOCATOR_MESSAGE("vector<bool, Allocator>", "bool"));

	using _Mybase = _Vb_val<_Alloc>;
	using _Alvbase_wrapped = typename _Mybase::_Alvbase_wrapped;
	using _Alvbase = typename _Mybase::_Alvbase;
	using _Alvbase_traits = typename _Mybase::_Alvbase_traits;

	using size_type = typename _Alvbase_traits::size_type;
	using difference_type = typename _Alvbase_traits::difference_type;
	using allocator_type = _Alloc;

	using reference = _Vb_reference<_Alvbase_wrapped>;
	using const_reference = bool;
	using value_type = bool;

	using _Reft = reference;
	using iterator = _Vb_iterator<_Alvbase_wrapped>;
	using const_iterator = _Vb_const_iterator<_Alvbase_wrapped>;

	using pointer = iterator;
	using const_pointer = const_iterator;
	using reverse_iterator = _STD reverse_iterator<iterator>;
	using const_reverse_iterator = _STD reverse_iterator<const_iterator>;

	static const int _VBITS = _STD _VBITS;
	enum { _EEN_VBITS = _VBITS };	// helper for expression evaluator

	vector() _NOEXCEPT_COND(is_nothrow_default_constructible_v<_Mybase>) // strengthened
		: _Mybase()
	{	// construct empty vector
	}

	explicit vector(const _Alloc& _Al)
		_NOEXCEPT_COND(is_nothrow_constructible_v<_Mybase, const _Alloc&>) // strengthened
		: _Mybase(_Al)
	{	// construct empty vector, allocator
	}

	explicit vector(_CRT_GUARDOVERFLOW size_type _Count, const _Alloc& _Al = _Alloc())
		: _Mybase(_Count, false, _Al)
	{	// construct from _Count * false, optional allocator
		_Trim(_Count);
	}

	vector(_CRT_GUARDOVERFLOW size_type _Count, const bool& _Val, const _Alloc& _Al = _Alloc())
		: _Mybase(_Count, _Val, _Al)
	{	// construct from _Count * _Val, optional allocator
		_Trim(_Count);
	}

	vector(const vector& _Right)
		: _Mybase(_Right)
	{	// construct by copying _Right
	}

	vector(const vector& _Right, const _Alloc& _Al)
		: _Mybase(_Right, _Al)
	{	// construct by copying _Right, allocator
	}

	template<class _Iter,
		class = enable_if_t<_Is_iterator_v<_Iter>>>
		vector(_Iter _First, _Iter _Last, const _Alloc& _Al = _Alloc())
		: _Mybase(_Al)
	{	// construct from [_First, _Last), optional allocator
		_BConstruct(_First, _Last);
	}

	template<class _Iter>
	void _BConstruct(_Iter _First, _Iter _Last)
	{	// initialize from [_First, _Last), input iterators
		insert(begin(), _First, _Last);
	}

	vector(vector&& _Right)
		_NOEXCEPT_COND(is_nothrow_move_constructible_v<_Mybase>) // strengthened
		: _Mybase(_STD move(_Right))
	{	// move construct by moving _Right
		this->_Swap_all(_Right);
	}

	vector(vector&& _Right, const _Alloc& _Al)
		_NOEXCEPT_COND(is_nothrow_constructible_v<_Mybase, _Mybase, const _Alloc&>)
		: _Mybase(_STD move(_Right), _Al)
	{	// move construct by moving _Right, allocator
		const bool _Swap_iterators = _Alvbase_traits::is_always_equal::value
			|| this->_Myvec._Getal() == _Right._Myvec._Getal();
		if (_Swap_iterators)
		{
			this->_Swap_all(_Right);
		}
	}

	vector& operator=(vector&& _Right)
		_NOEXCEPT_COND(is_nothrow_move_assignable_v<_Mybase>) // strengthened
	{	// assign by moving _Right
		if (this != _STD addressof(_Right))
		{	// different, assign it
			clear();

			const bool _Reload = _Alvbase_traits::propagate_on_container_move_assignment::value
				&& !_Alvbase_traits::is_always_equal::value
				&& this->_Myvec._Getal() != _Right._Myvec._Getal();

			if (_Reload)
			{
				this->_Free_proxy();
			}

			this->_Myvec = _STD move(_Right._Myvec);

			if (_Reload)
			{
				this->_Alloc_proxy();
			}

			this->_Mysize = _Right._Mysize;
			_Right._Mysize = 0;

			const bool _Swap_iterators = _Alvbase_traits::is_always_equal::value
				|| this->_Myvec._Getal() == _Right._Myvec._Getal();
			if (_Swap_iterators)
			{
				this->_Swap_all(_Right);
			}
		}
		return (*this);
	}

	template<class... _Valty>
	decltype(auto) emplace_back(_Valty&&... _Val)
	{	// insert bool at end
		bool _Tmp(_STD forward<_Valty>(_Val)...);
		push_back(_Tmp);

#if _HAS_CXX17
		return (back());
#endif /* _HAS_CXX17 */
	}

	template<class... _Valty>
	iterator emplace(const_iterator _Where, _Valty&&... _Val)
	{	// insert bool at _Where
		bool _Tmp(_STD forward<_Valty>(_Val)...);
		return (insert(_Where, _Tmp));
	}

	vector(initializer_list<bool> _Ilist,
		const _Alloc& _Al = allocator_type())
		: _Mybase(0, false, _Al)
	{	// construct from initializer_list
		insert(begin(), _Ilist.begin(), _Ilist.end());
	}

	vector& operator=(initializer_list<bool> _Ilist)
	{	// assign initializer_list
		assign(_Ilist.begin(), _Ilist.end());
		return (*this);
	}

	void assign(initializer_list<bool> _Ilist)
	{	// assign initializer_list
		assign(_Ilist.begin(), _Ilist.end());
	}

	iterator insert(const_iterator _Where,
		initializer_list<bool> _Ilist)
	{	// insert initializer_list
		return (insert(_Where, _Ilist.begin(), _Ilist.end()));
	}

	~vector() noexcept
	{	// destroy the object
	}

	vector& operator=(const vector& _Right)
	{	// assign from _Right
		if (this != _STD addressof(_Right))
		{	// different, assign it
			this->_Orphan_all();

			const bool _Reload = _Alvbase_traits::propagate_on_container_copy_assignment::value
				&& this->_Myvec._Getal() != _Right._Myvec._Getal();

			if (_Reload)
			{
				this->_Free_proxy();
			}

			this->_Myvec = _Right._Myvec;

			if (_Reload)
			{
				this->_Alloc_proxy();
			}

			this->_Mysize = _Right._Mysize;
		}

		return (*this);
	}

	void reserve(_CRT_GUARDOVERFLOW size_type _Count)
	{	// determine new minimum length of allocated storage
		this->_Myvec.reserve(this->_Nw(_Count));
	}

	_NODISCARD size_type capacity() const noexcept
	{	// return current length of allocated storage
		return (this->_Myvec.capacity() * _VBITS);
	}

	_NODISCARD iterator begin() noexcept
	{	// return iterator for beginning of mutable sequence
		return (iterator(this->_Myvec.data(), this));
	}

	_NODISCARD const_iterator begin() const noexcept
	{	// return iterator for beginning of nonmutable sequence
		return (const_iterator(this->_Myvec.data(), this));
	}

	_NODISCARD iterator end() noexcept
	{	// return iterator for end of mutable sequence
		return (begin() + static_cast<difference_type>(this->_Mysize));
	}

	_NODISCARD const_iterator end() const noexcept
	{	// return iterator for end of nonmutable sequence
		return (begin() + static_cast<difference_type>(this->_Mysize));
	}

	_NODISCARD const_iterator cbegin() const noexcept
	{	// return iterator for beginning of nonmutable sequence
		return (begin());
	}

	_NODISCARD const_iterator cend() const noexcept
	{	// return iterator for end of nonmutable sequence
		return (end());
	}

	_NODISCARD const_reverse_iterator crbegin() const noexcept
	{	// return iterator for beginning of reversed nonmutable sequence
		return (rbegin());
	}

	_NODISCARD const_reverse_iterator crend() const noexcept
	{	// return iterator for end of reversed nonmutable sequence
		return (rend());
	}

	void shrink_to_fit()
	{	// reduce capacity
		if (this->_Myvec.capacity() != this->_Myvec.size())
		{	// worth shrinking, do it
			this->_Orphan_all();
			this->_Myvec.shrink_to_fit();
		}
	}

	iterator _Make_iter(const_iterator _Where)
	{	// make iterator from const_iterator
		iterator _Tmp = begin();
		if (0 < this->_Mysize)
			_Tmp += _Where - begin();
		return (_Tmp);
	}

	_NODISCARD reverse_iterator rbegin() noexcept
	{	// return iterator for beginning of reversed mutable sequence
		return (reverse_iterator(end()));
	}

	_NODISCARD const_reverse_iterator rbegin() const noexcept
	{	// return iterator for beginning of reversed nonmutable sequence
		return (const_reverse_iterator(end()));
	}

	_NODISCARD reverse_iterator rend() noexcept
	{	// return iterator for end of reversed mutable sequence
		return (reverse_iterator(begin()));
	}

	_NODISCARD const_reverse_iterator rend() const noexcept
	{	// return iterator for end of reversed nonmutable sequence
		return (const_reverse_iterator(begin()));
	}

	void resize(_CRT_GUARDOVERFLOW size_type _Newsize, bool _Val = false)
	{	// determine new length, padding with _Val elements as needed
		if (size() < _Newsize)
			_Insert_n(end(), _Newsize - size(), _Val);
		else if (_Newsize < size())
			erase(begin() + static_cast<difference_type>(_Newsize), end());
	}

	_NODISCARD size_type size() const noexcept
	{	// return length of sequence
		return (this->_Mysize);
	}

	_NODISCARD size_type max_size() const noexcept
	{	// return maximum possible length of sequence
		constexpr size_type _Diff_max = static_cast<size_type>((numeric_limits<difference_type>::max)());
		const size_type _Ints_max = this->_Myvec.max_size();
		if (_Ints_max > _Diff_max / _VBITS)
		{	// max_size bound by difference_type limits
			return (_Diff_max);
		}

		// max_size bound by underlying storage limits
		return (_Ints_max * _VBITS);
	}

	_NODISCARD bool empty() const noexcept
	{	// test if sequence is empty
		return (size() == 0);
	}

	_NODISCARD allocator_type get_allocator() const noexcept
	{	// return allocator object for values
		return (static_cast<allocator_type>(this->_Myvec.get_allocator()));
	}

	_NODISCARD const_reference at(size_type _Off) const
	{	// subscript nonmutable sequence with checking
		if (size() <= _Off)
			_Xran();
		return ((*this)[_Off]);
	}

	_NODISCARD reference at(size_type _Off)
	{	// subscript mutable sequence with checking
		if (size() <= _Off)
			_Xran();
		return ((*this)[_Off]);
	}

	_NODISCARD const_reference operator[](size_type _Off) const
	{	// subscript nonmutable sequence
		const_iterator _It = begin();
		_It._Advance(_Off);
		return (*_It);
	}

	_NODISCARD reference operator[](size_type _Off)
	{	// subscript mutable sequence
		iterator _It = begin();
		_It._Advance(_Off);
		return (*_It);
	}

	_NODISCARD reference front()
	{	// return first element of mutable sequence
		return (*begin());
	}

	_NODISCARD const_reference front() const
	{	// return first element of nonmutable sequence
		return (*begin());
	}

	_NODISCARD reference back()
	{	// return last element of mutable sequence
		return (*(end() - 1));
	}

	_NODISCARD const_reference back() const
	{	// return last element of nonmutable sequence
		return (*(end() - 1));
	}

	void push_back(const bool& _Val)
	{	// insert element at end
		insert(end(), _Val);
	}

	void pop_back()
	{	// erase element at end
		erase(end() - 1);
	}

	template<class _Iter,
		class = enable_if_t<_Is_iterator_v<_Iter>>>
		void assign(_Iter _First, _Iter _Last)
	{	// assign [_First, _Last), input iterators
		erase(begin(), end());
		insert(begin(), _First, _Last);
	}

	void assign(_CRT_GUARDOVERFLOW size_type _Count, const bool& _Val)
	{	// assign _Count * _Val
		erase(begin(), end());
		_Insert_n(begin(), _Count, _Val);
	}

	iterator insert(const_iterator _Where, const bool& _Val)
	{	// insert _Val at _Where
		return (_Insert_n(_Where, static_cast<size_type>(1), _Val));
	}

	iterator insert(const_iterator _Where, _CRT_GUARDOVERFLOW size_type _Count,
		const bool& _Val)
	{	// insert _Count * _Val at _Where
		return (_Insert_n(_Where, _Count, _Val));
	}

	template<class _Iter,
		class = enable_if_t<_Is_iterator_v<_Iter>>>
		iterator insert(const_iterator _Where, _Iter _First, _Iter _Last)
	{	// insert [_First, _Last) at _Where
		difference_type _Off = _Where - begin();
		_Insert(_Where, _First, _Last, _Iter_cat_t<_Iter>());
		return (begin() + _Off);
	}

	template<class _Iter>
	void _Insert(const_iterator _Where,
		_Iter _First, _Iter _Last,
		input_iterator_tag)
	{	// insert [_First, _Last) at _Where, input iterators
		difference_type _Off = _Where - begin();

		for (; _First != _Last; ++_First, (void)++_Off)
			insert(begin() + _Off, *_First);
	}

	template<class _Iter>
	void _Insert(const_iterator _Where,
		_Iter _First, _Iter _Last,
		forward_iterator_tag)
	{	// insert [_First, _Last) at _Where, forward iterators
		_Adl_verify_range(_First, _Last);
		size_type _Count = _Convert_size<size_type>(static_cast<size_t>(_STD distance(_First, _Last)));
		size_type _Off = _Insert_x(_Where, _Count);
		_Copy_unchecked(_Get_unwrapped(_First), _Get_unwrapped(_Last),
			begin() + static_cast<difference_type>(_Off));
	}

	iterator erase(const_iterator _Where_arg)
	{	// erase element at _Where
		iterator _Where = _Make_iter(_Where_arg);
		difference_type _Off = _Where - begin();

#if _ITERATOR_DEBUG_LEVEL == 2
		_STL_VERIFY(end() > _Where, "vector<bool> erase iterator outside range");
		_STD copy(_Where + 1, end(), _Where);
		_Orphan_range(static_cast<size_type>(_Off), this->_Mysize);

#else /* _ITERATOR_DEBUG_LEVEL == 2 */
		_STD copy(_Where + 1, end(), _Where);
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */

		_Trim(this->_Mysize - 1);
		return (begin() + _Off);
	}

	iterator erase(const_iterator _First_arg,
		const_iterator _Last_arg)
	{	// erase [_First, _Last)
		iterator _First = _Make_iter(_First_arg);
		iterator _Last = _Make_iter(_Last_arg);
		difference_type _Off = _First - begin();

		if (_First != _Last)
		{	// worth doing, copy down over hole
#if _ITERATOR_DEBUG_LEVEL == 2
			_STL_VERIFY(_Last >= _First && end() >= _Last, "vector<bool> erase iterator outside range");
			iterator _Next = _STD copy(_Last, end(), _First);
			const auto _Newsize = static_cast<size_type>(_Next - begin());
			_Orphan_range(_Newsize, this->_Mysize);
			_Trim(_Newsize);

#else /* _ITERATOR_DEBUG_LEVEL == 2 */
			iterator _Next = _STD copy(_Last, end(), _First);
			_Trim(static_cast<size_type>(_Next - begin()));
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */
		}
		return (begin() + _Off);
	}

	void clear() noexcept
	{	// erase all elements
		erase(begin(), end());
	}

	void flip() noexcept
	{	// toggle all elements
		for (auto& _Elem : this->_Myvec)
		{
			_Elem = ~_Elem;
		}

		_Trim(this->_Mysize);
	}

	void swap(vector& _Right) noexcept // strengthened
	{	// exchange contents with _Right
		if (this != _STD addressof(_Right))
		{	// (maybe) swap allocators, swap control information
			this->_Swap_all(_Right);
			this->_Myvec.swap(_Right._Myvec);
			_STD swap(this->_Mysize, _Right._Mysize);
		}
	}

	static void swap(reference _Left, reference _Right) noexcept
	{	// swap _Left and _Right vector<bool> elements
		bool _Val = _Left;	// NOT _STD swap
		_Left = _Right;
		_Right = _Val;
	}

	friend hash<vector<bool, _Alloc>>;

	iterator _Insert_n(const_iterator _Where,
		size_type _Count, const bool& _Val)
	{	// insert _Count * _Val at _Where
		size_type _Off = _Insert_x(_Where, _Count);
		const auto _Result = begin() + static_cast<difference_type>(_Off);
		_STD fill(_Result, _Result + static_cast<difference_type>(_Count), _Val);
		return (_Result);
	}

	size_type _Insert_x(const_iterator _Where, size_type _Count)
	{	// make room to insert _Count elements at _Where
		difference_type _Off = _Where - begin();

#if _ITERATOR_DEBUG_LEVEL == 2
		_STL_VERIFY(end() >= _Where, "vector<bool> insert iterator outside range");
		bool _Realloc = capacity() - size() < _Count;
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */

		if (_Count != 0)
		{
			if (max_size() - size() < _Count)
			{
				_Xlen();	// result too long
			}

			// worth doing
			this->_Myvec.resize(this->_Nw(size() + _Count), 0);
			if (empty())
			{
				this->_Mysize += _Count;
			}
			else
			{	// make room and copy down suffix
				iterator _Oldend = end();
				this->_Mysize += _Count;
				_STD copy_backward(begin() + _Off, _Oldend, end());
			}

#if _ITERATOR_DEBUG_LEVEL == 2
			_Orphan_range(static_cast<size_type>(_Realloc ? 0 : _Off), this->_Mysize);
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */
		}

		return (static_cast<size_type>(_Off));
	}

#if _ITERATOR_DEBUG_LEVEL == 2
	void _Orphan_range(size_type _Offlo, size_type _Offhi) const
	{	// orphan iterators within specified (inclusive) range
		_Lockit _Lock(_LOCK_DEBUG);
		auto _Base = const_cast<_Vbase *>(this->_Myvec.data());

		const_iterator **_Pnext = (const_iterator **)this->_Getpfirst();
		if (_Pnext != nullptr)
			while (*_Pnext != nullptr)
			{	// test offset from beginning of vector
				size_type _Off = static_cast<size_type>(_VBITS * ((*_Pnext)->_Myptr - _Base))
					+ (*_Pnext)->_Myoff;
				if (_Off < _Offlo || _Offhi < _Off)
					_Pnext = (const_iterator **)(*_Pnext)->_Getpnext();
				else
				{	// orphan the iterator
					(*_Pnext)->_Clrcont();
					*_Pnext = *(const_iterator **)(*_Pnext)->_Getpnext();
				}
			}
	}

#else /* _ITERATOR_DEBUG_LEVEL == 2 */
	void _Orphan_range(size_type, size_type) const
	{	// orphan iterators within specified (inclusive) range
	}
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */

	void _Trim(size_type _Size)
	{	// trim base vector to exact length in bits
		if (max_size() < _Size)
			_Xlen();	// result too long
		const size_type _Words = this->_Nw(_Size);

		if (_Words < this->_Myvec.size())
			this->_Myvec.erase(this->_Myvec.begin() + static_cast<difference_type>(_Words),
				this->_Myvec.end());
		this->_Mysize = _Size;
		_Size %= _VBITS;
		if (0 < _Size)
			this->_Myvec[_Words - 1] &= (static_cast<_Vbase>(1) << _Size) - 1;
	}

	[[noreturn]] void _Xlen() const
	{	// report a length_error
		_Xlength_error("vector<bool> too long");
	}

	[[noreturn]] void _Xran() const
	{	// report an out_of_range error
		_Xout_of_range("invalid vector<bool> subscript");
	}
};

template<class _Alloc>
_NODISCARD inline bool operator==(const vector<bool, _Alloc>& _Left,
	const vector<bool, _Alloc>& _Right)
{	// test for vector equality
	return (_Left.size() == _Right.size()
		&& _Left._Myvec == _Right._Myvec);
}

template<class _Alloc>
_NODISCARD inline bool operator!=(const vector<bool, _Alloc>& _Left,
	const vector<bool, _Alloc>& _Right)
{	// test for vector inequality
	return (!(_Left == _Right));
}

// STRUCT TEMPLATE SPECIALIZATION hash
template<class _Alloc>
struct hash<vector<bool, _Alloc>>
{	// hash functor
	_CXX17_DEPRECATE_ADAPTOR_TYPEDEFS typedef vector<bool, _Alloc> argument_type;
	_CXX17_DEPRECATE_ADAPTOR_TYPEDEFS typedef size_t result_type;

	_NODISCARD size_t operator()(const vector<bool, _Alloc>& _Keyval) const noexcept
	{	// hash _Keyval to size_t value by pseudorandomizing transform
		return (_Hash_array_representation(_Keyval._Myvec.data(), _Keyval._Myvec.size()));
	}
};

#if _HAS_CXX17
namespace pmr {
	template<class _Ty>
	using vector = _STD vector<_Ty, polymorphic_allocator<_Ty>>;
} // namespace pmr
#endif /* _HAS_CXX17 */
_STD_END

#pragma pop_macro("new")
_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)
#endif /* RC_INVOKED */
#endif /* _VECTOR_ */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0009 */
