// xtree internal header
#pragma once
#ifndef _XTREE_
#define _XTREE_
#ifndef RC_INVOKED
#include <xmemory>
#include <stdexcept>

#if _HAS_CXX17
#include <xnode_handle.h>
#endif /* _HAS_CXX17 */

#pragma pack(push,_CRT_PACKING)
#pragma warning(push,_STL_WARNING_LEVEL)
#pragma warning(disable: _STL_DISABLED_WARNINGS)
_STL_DISABLE_CLANG_WARNINGS
#pragma push_macro("new")
#undef new

_STD_BEGIN
// CLASS TEMPLATE _Tree_unchecked_const_iterator
template<class _Mytree,
	class _Base = _Iterator_base0>
	class _Tree_unchecked_const_iterator
	: public _Base
{	// unchecked iterator for nonmutable tree
public:
	using iterator_category = bidirectional_iterator_tag;

	using _Nodeptr = typename _Mytree::_Nodeptr;
	using value_type = typename _Mytree::value_type;
	using difference_type = typename _Mytree::difference_type;
	using pointer = typename _Mytree::const_pointer;
	using reference = const value_type&;

	_Tree_unchecked_const_iterator()
		: _Ptr()
	{	// construct with null node pointer
	}

	_Tree_unchecked_const_iterator(_Nodeptr _Pnode, const _Mytree *_Plist)
		: _Ptr(_Pnode)
	{	// construct with node pointer _Pnode
		this->_Adopt(_Plist);
	}

	_NODISCARD reference operator*() const
	{	// return designated value
		return (_Ptr->_Myval);
	}

	_NODISCARD pointer operator->() const
	{	// return pointer to class object
		return (pointer_traits<pointer>::pointer_to(**this));
	}

	_Tree_unchecked_const_iterator& operator++()
	{	// preincrement
		if (_Ptr->_Right->_Isnil)
		{	// climb looking for right subtree
			_Nodeptr _Pnode;
			while (!(_Pnode = _Ptr->_Parent)->_Isnil && _Ptr == _Pnode->_Right)
			{
				_Ptr = _Pnode;	// ==> parent while right subtree
			}

			_Ptr = _Pnode;	// ==> parent (head if end())
		}
		else
		{
			_Ptr = _Mytree::_Min(_Ptr->_Right);	// ==> smallest of right subtree
		}

		return (*this);
	}

	_Tree_unchecked_const_iterator operator++(int)
	{	// postincrement
		_Tree_unchecked_const_iterator _Tmp = *this;
		++*this;
		return (_Tmp);
	}

	_Tree_unchecked_const_iterator& operator--()
	{	// predecrement
		if (_Ptr->_Isnil)
		{
			_Ptr = _Ptr->_Right;	// end() ==> rightmost
		}
		else if (_Ptr->_Left->_Isnil)
		{	// climb looking for left subtree
			_Nodeptr _Pnode;
			while (!(_Pnode = _Ptr->_Parent)->_Isnil && _Ptr == _Pnode->_Left)
			{
				_Ptr = _Pnode;	// ==> parent while left subtree
			}

			if (!_Ptr->_Isnil)
			{	// decrement non-begin()
				_Ptr = _Pnode;	// ==> parent if not head
			}
		}
		else
		{
			_Ptr = _Mytree::_Max(_Ptr->_Left);	// ==> largest of left subtree
		}

		return (*this);
	}

	_Tree_unchecked_const_iterator operator--(int)
	{	// postdecrement
		_Tree_unchecked_const_iterator _Tmp = *this;
		--*this;
		return (_Tmp);
	}

	_NODISCARD bool operator==(const _Tree_unchecked_const_iterator& _Right) const
	{	// test for iterator equality
		return (_Ptr == _Right._Ptr);
	}

	_NODISCARD bool operator!=(const _Tree_unchecked_const_iterator& _Right) const
	{	// test for iterator inequality
		return (!(*this == _Right));
	}

	_Nodeptr _Ptr;	// pointer to node
};

// CLASS TEMPLATE _Tree_unchecked_iterator
template<class _Mytree>
class _Tree_unchecked_iterator
	: public _Tree_unchecked_const_iterator<_Mytree>
{	// unchecked iterator for mutable tree
public:
	using _Mybase = _Tree_unchecked_const_iterator<_Mytree>;
	using iterator_category = bidirectional_iterator_tag;

	using _Nodeptr = typename _Mytree::_Nodeptr;
	using value_type = typename _Mytree::value_type;
	using difference_type = typename _Mytree::difference_type;
	using pointer = typename _Mytree::pointer;
	using reference = value_type & ;

	_Tree_unchecked_iterator()
	{	// construct with null node
	}

	_Tree_unchecked_iterator(_Nodeptr _Pnode, const _Mytree *_Plist)
		: _Mybase(_Pnode, _Plist)
	{	// construct with node pointer _Pnode
	}

	_NODISCARD reference operator*() const
	{	// return designated value
		return ((reference)**(_Mybase *)this);
	}

	_NODISCARD pointer operator->() const
	{	// return pointer to class object
		return (pointer_traits<pointer>::pointer_to(**this));
	}

	_Tree_unchecked_iterator& operator++()
	{	// preincrement
		++static_cast<_Mybase&>(*this);
		return (*this);
	}

	_Tree_unchecked_iterator operator++(int)
	{	// postincrement
		_Tree_unchecked_iterator _Tmp = *this;
		++*this;
		return (_Tmp);
	}

	_Tree_unchecked_iterator& operator--()
	{	// predecrement
		--static_cast<_Mybase&>(*this);
		return (*this);
	}

	_Tree_unchecked_iterator operator--(int)
	{	// postdecrement
		_Tree_unchecked_iterator _Tmp = *this;
		--*this;
		return (_Tmp);
	}
};

// CLASS TEMPLATE _Tree_const_iterator
template<class _Mytree>
class _Tree_const_iterator
	: public _Tree_unchecked_const_iterator<_Mytree, _Iterator_base>
{	// iterator for nonmutable tree
public:
	using _Mybase = _Tree_unchecked_const_iterator<_Mytree, _Iterator_base>;
	using iterator_category = bidirectional_iterator_tag;

	using _Nodeptr = typename _Mytree::_Nodeptr;
	using value_type = typename _Mytree::value_type;
	using difference_type = typename _Mytree::difference_type;
	using pointer = typename _Mytree::const_pointer;
	using reference = const value_type&;

	_Tree_const_iterator()
		: _Mybase()
	{	// construct with null node pointer
	}

	_Tree_const_iterator(_Nodeptr _Pnode, const _Mytree *_Plist)
		: _Mybase(_Pnode, _Plist)
	{	// construct with node pointer _Pnode
	}

	_NODISCARD reference operator*() const
	{	// return designated value
#if _ITERATOR_DEBUG_LEVEL != 0
		const auto _Mycont = static_cast<const _Mytree *>(this->_Getcont());
		_STL_ASSERT(_Mycont, "cannot dereference value-initialized map/set iterator");
		_STL_VERIFY(this->_Ptr != _Mycont->_Myhead, "cannot dereference end map/set iterator");
#endif /* _ITERATOR_DEBUG_LEVEL != 0 */

		return (this->_Ptr->_Myval);
	}

	_NODISCARD pointer operator->() const
	{	// return pointer to class object
		return (pointer_traits<pointer>::pointer_to(**this));
	}

	_Tree_const_iterator& operator++()
	{	// preincrement
#if _ITERATOR_DEBUG_LEVEL != 0
		_STL_VERIFY(this->_Getcont(), "cannot increment value-initialized map/set iterator");
		_STL_VERIFY(!this->_Ptr->_Isnil, "cannot increment end map/set iterator");
#endif /* _ITERATOR_DEBUG_LEVEL != 0 */

		++static_cast<_Mybase&>(*this);
		return (*this);
	}

	_Tree_const_iterator operator++(int)
	{	// postincrement
		_Tree_const_iterator _Tmp = *this;
		++*this;
		return (_Tmp);
	}

	_Tree_const_iterator& operator--()
	{	// predecrement
#if _ITERATOR_DEBUG_LEVEL == 0
		--static_cast<_Mybase&>(*this);
#else /* ^^^ _ITERATOR_DEBUG_LEVEL == 0 ^^^ // vvv _ITERATOR_DEBUG_LEVEL != 0 vvv */
		_STL_ASSERT(this->_Getcont(), "cannot decrement value-initialized map/set iterator");
		_Nodeptr _Ptrsav = this->_Ptr;
		--static_cast<_Mybase&>(*this);
		_STL_VERIFY(_Ptrsav != this->_Ptr, "cannot decrement begin map/set iterator");
#endif /* _ITERATOR_DEBUG_LEVEL == 0 */

		return (*this);
	}

	_Tree_const_iterator operator--(int)
	{	// postdecrement
		_Tree_const_iterator _Tmp = *this;
		--*this;
		return (_Tmp);
	}

	_NODISCARD bool operator==(const _Tree_const_iterator& _Right) const
	{	// test for iterator equality
#if _ITERATOR_DEBUG_LEVEL != 0
		_STL_VERIFY(this->_Getcont() == _Right._Getcont(), "map/set iterators incompatible");
#endif /* _ITERATOR_DEBUG_LEVEL != 0 */

		return (this->_Ptr == _Right._Ptr);
	}

	_NODISCARD bool operator!=(const _Tree_const_iterator& _Right) const
	{	// test for iterator inequality
		return (!(*this == _Right));
	}

#if _ITERATOR_DEBUG_LEVEL != 0
	friend void _Verify_range(const _Tree_const_iterator& _First, const _Tree_const_iterator& _Last)
	{
		_STL_VERIFY(_First._Getcont() == _Last._Getcont(),
			"map/set iterators in range are from different containers");
	}
#endif /* _ITERATOR_DEBUG_LEVEL != 0 */

	_NODISCARD _Tree_unchecked_const_iterator<_Mytree> _Unwrapped() const
	{
		return (_Tree_unchecked_const_iterator<_Mytree>(this->_Ptr, static_cast<const _Mytree *>(this->_Getcont())));
	}

	void _Seek_to(const _Tree_unchecked_const_iterator<_Mytree> _It)
	{
		this->_Ptr = _It._Ptr;
	}
};

// CLASS TEMPLATE _Tree_iterator
template<class _Mytree>
class _Tree_iterator
	: public _Tree_const_iterator<_Mytree>
{	// iterator for mutable tree
public:
	using _Mybase = _Tree_const_iterator<_Mytree>;
	using iterator_category = bidirectional_iterator_tag;

	using _Nodeptr = typename _Mytree::_Nodeptr;
	using value_type = typename _Mytree::value_type;
	using difference_type = typename _Mytree::difference_type;

	using pointer = typename _Mytree::pointer;
	using reference = value_type & ;

	_Tree_iterator()
	{	// construct with null node
	}

	_Tree_iterator(_Nodeptr _Pnode, const _Mytree *_Plist)
		: _Mybase(_Pnode, _Plist)
	{	// construct with node pointer _Pnode
	}

	_NODISCARD reference operator*() const
	{	// return designated value
		return ((reference)**(_Mybase *)this);
	}

	_NODISCARD pointer operator->() const
	{	// return pointer to class object
		return (pointer_traits<pointer>::pointer_to(**this));
	}

	_Tree_iterator& operator++()
	{	// preincrement
		++static_cast<_Mybase&>(*this);
		return (*this);
	}

	_Tree_iterator operator++(int)
	{	// postincrement
		_Tree_iterator _Tmp = *this;
		++*this;
		return (_Tmp);
	}

	_Tree_iterator& operator--()
	{	// predecrement
		--static_cast<_Mybase&>(*this);
		return (*this);
	}

	_Tree_iterator operator--(int)
	{	// postdecrement
		_Tree_iterator _Tmp = *this;
		--*this;
		return (_Tmp);
	}

	_NODISCARD _Tree_unchecked_iterator<_Mytree> _Unwrapped() const
	{
		return (_Tree_unchecked_iterator<_Mytree>(this->_Ptr, static_cast<const _Mytree *>(this->_Getcont())));
	}
};

// tree TYPE WRAPPERS
template<class _Value_type,
	class _Size_type,
	class _Difference_type,
	class _Pointer,
	class _Const_pointer,
	class _Reference,
	class _Const_reference,
	class _Nodeptr_type>
	struct _Tree_iter_types
{	// wraps types needed by iterators
	using value_type = _Value_type;
	using size_type = _Size_type;
	using difference_type = _Difference_type;
	using pointer = _Pointer;
	using const_pointer = _Const_pointer;
	using _Nodeptr = _Nodeptr_type;
};

template<class _Value_type,
	class _Voidptr>
	struct _Tree_node
{	// tree node
	using _Nodeptr = _Rebind_pointer_t<_Voidptr, _Tree_node>;
	_Nodeptr _Left;	// left subtree, or smallest element if head
	_Nodeptr _Parent;	// parent, or root of tree if head
	_Nodeptr _Right;	// right subtree, or largest element if head
	char _Color;	// _Red or _Black, _Black if head
	char _Isnil;	// true only if head (also nil) node
	_Value_type _Myval;	// the stored value, unused if head

	_Tree_node& operator=(const _Tree_node&) = delete;

	template<class _Alloc>
	static void _Freenode0(_Alloc& _Al, _Nodeptr _Ptr) noexcept
	{
		using _Alnode = _Rebind_alloc_t<_Alloc, _Tree_node>;
		using _Alnode_traits = allocator_traits<_Alnode>;
		_Alnode _Node_alloc(_Al);
		_Alnode_traits::destroy(_Node_alloc, _STD addressof(_Ptr->_Left));
		_Alnode_traits::destroy(_Node_alloc, _STD addressof(_Ptr->_Parent));
		_Alnode_traits::destroy(_Node_alloc, _STD addressof(_Ptr->_Right));
		_Alnode_traits::deallocate(_Node_alloc, _Ptr, 1);
	}
};

template<class _Ty>
struct _Tree_simple_types
	: public _Simple_types<_Ty>
{	// wraps types needed by iterators
	using _Node = _Tree_node<_Ty, void *>;
	using _Nodeptr = _Node * ;
};

template<class _Ty,
	class _Alloc>
	struct _Tree_base_types
{	// types needed for a container base
	using _Alty = _Rebind_alloc_t<_Alloc, _Ty>;
	using _Alty_traits = allocator_traits<_Alty>;
	using _Node = _Tree_node<_Ty, typename allocator_traits<_Alloc>::void_pointer>;
	using _Alnode = _Rebind_alloc_t<_Alloc, _Node>;
	using _Alnode_traits = allocator_traits<_Alnode>;
	using _Nodeptr = typename _Alnode_traits::pointer;

	using _Val_types = conditional_t<_Is_simple_alloc_v<_Alnode>,
		_Tree_simple_types<_Ty>,
		_Tree_iter_types<_Ty,
		typename _Alty_traits::size_type,
		typename _Alty_traits::difference_type,
		typename _Alty_traits::pointer,
		typename _Alty_traits::const_pointer,
		_Ty&,
		const _Ty&,
		_Nodeptr>>;
};

// CLASS TEMPLATE _Tree_val
template<class _Val_types>
class _Tree_val
	: public _Container_base
{	// base class for tree to hold data
public:
	using _Nodeptr = typename _Val_types::_Nodeptr;

	using value_type = typename _Val_types::value_type;
	using size_type = typename _Val_types::size_type;
	using difference_type = typename _Val_types::difference_type;
	using pointer = typename _Val_types::pointer;
	using const_pointer = typename _Val_types::const_pointer;
	using reference = value_type & ;
	using const_reference = const value_type&;

	using const_iterator = _Tree_const_iterator<_Tree_val>;

	_Tree_val()
		: _Myhead(),
		_Mysize(0)
	{	// initialize data
	}

	enum _Redbl
	{	// colors for link to parent
		_Red, _Black
	};

	static _Nodeptr _Max(_Nodeptr _Pnode)
	{	// return rightmost node in subtree at _Pnode
		while (!_Pnode->_Right->_Isnil)
			_Pnode = _Pnode->_Right;
		return (_Pnode);
	}

	static _Nodeptr _Min(_Nodeptr _Pnode)
	{	// return leftmost node in subtree at _Pnode
		while (!_Pnode->_Left->_Isnil)
			_Pnode = _Pnode->_Left;
		return (_Pnode);
	}

	_Nodeptr& _Root() const
	{	// return root of nonmutable tree
		return (_Myhead->_Parent);
	}

	_Nodeptr& _Lmost() const
	{	// return leftmost node in nonmutable tree
		return (_Myhead->_Left);
	}

	_Nodeptr& _Rmost() const
	{	// return rightmost node in nonmutable tree
		return (_Myhead->_Right);
	}

	void _Lrotate(_Nodeptr _Wherenode)
	{	// promote right node to root of subtree
		_Nodeptr _Pnode = _Wherenode->_Right;
		_Wherenode->_Right = _Pnode->_Left;

		if (!_Pnode->_Left->_Isnil)
		{
			_Pnode->_Left->_Parent = _Wherenode;
		}

		_Pnode->_Parent = _Wherenode->_Parent;

		if (_Wherenode == _Myhead->_Parent)
		{
			_Myhead->_Parent = _Pnode;
		}
		else if (_Wherenode == _Wherenode->_Parent->_Left)
		{
			_Wherenode->_Parent->_Left = _Pnode;
		}
		else
		{
			_Wherenode->_Parent->_Right = _Pnode;
		}

		_Pnode->_Left = _Wherenode;
		_Wherenode->_Parent = _Pnode;
	}

	void _Rrotate(_Nodeptr _Wherenode)
	{	// promote left node to root of subtree
		_Nodeptr _Pnode = _Wherenode->_Left;
		_Wherenode->_Left = _Pnode->_Right;

		if (!_Pnode->_Right->_Isnil)
			_Pnode->_Right->_Parent = _Wherenode;
		_Pnode->_Parent = _Wherenode->_Parent;

		if (_Wherenode == _Myhead->_Parent)
		{
			_Myhead->_Parent = _Pnode;
		}
		else if (_Wherenode == _Wherenode->_Parent->_Right)
		{
			_Wherenode->_Parent->_Right = _Pnode;
		}
		else
		{
			_Wherenode->_Parent->_Left = _Pnode;
		}

		_Pnode->_Right = _Wherenode;
		_Wherenode->_Parent = _Pnode;
	}

	_Nodeptr _Extract(const_iterator _Where)
	{	// extract node at _Where
#if _ITERATOR_DEBUG_LEVEL == 2
		_STL_VERIFY(_Where._Getcont() == this
			&& !_Where._Ptr->_Isnil, "map/set erase iterator outside range");
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */

		_Nodeptr _Erasednode = _Where._Ptr;	// node to erase
		++_Where;	// save successor iterator for return

		_Nodeptr _Fixnode;	// the node to recolor as needed
		_Nodeptr _Fixnodeparent;	// parent of _Fixnode (which may be nil)
		_Nodeptr _Pnode = _Erasednode;

		if (_Pnode->_Left->_Isnil)
		{
			_Fixnode = _Pnode->_Right;	// stitch up right subtree
		}
		else if (_Pnode->_Right->_Isnil)
		{
			_Fixnode = _Pnode->_Left;	// stitch up left subtree
		}
		else
		{	// two subtrees, must lift successor node to replace erased
			_Pnode = _Where._Ptr;	// _Pnode is successor node
			_Fixnode = _Pnode->_Right;	// _Fixnode is only subtree
		}

		if (_Pnode == _Erasednode)
		{	// at most one subtree, relink it
			_Fixnodeparent = _Erasednode->_Parent;
			if (!_Fixnode->_Isnil)
				_Fixnode->_Parent = _Fixnodeparent;	// link up

			if (_Root() == _Erasednode)
			{
				_Root() = _Fixnode;	// link down from root
			}
			else if (_Fixnodeparent->_Left == _Erasednode)
			{
				_Fixnodeparent->_Left = _Fixnode;	// link down to left
			}
			else
			{
				_Fixnodeparent->_Right = _Fixnode;	// link down to right
			}

			if (_Lmost() == _Erasednode)
			{
				_Lmost() = _Fixnode->_Isnil
					? _Fixnodeparent	// smallest is parent of erased node
					: _Min(_Fixnode);	// smallest in relinked subtree
			}

			if (_Rmost() == _Erasednode)
			{
				_Rmost() = _Fixnode->_Isnil
					? _Fixnodeparent	// largest is parent of erased node
					: _Max(_Fixnode);	// largest in relinked subtree
			}
		}
		else
		{	// erased has two subtrees, _Pnode is successor to erased
			_Erasednode->_Left->_Parent = _Pnode;	// link left up
			_Pnode->_Left = _Erasednode->_Left;	// link successor down

			if (_Pnode == _Erasednode->_Right)
			{
				_Fixnodeparent = _Pnode;	// successor is next to erased
			}
			else
			{	// successor further down, link in place of erased
				_Fixnodeparent = _Pnode->_Parent;	// parent is successor's
				if (!_Fixnode->_Isnil)
				{
					_Fixnode->_Parent = _Fixnodeparent;	// link fix up
				}

				_Fixnodeparent->_Left = _Fixnode;	// link fix down
				_Pnode->_Right = _Erasednode->_Right;	// link next down
				_Erasednode->_Right->_Parent = _Pnode;	// right up
			}

			if (_Root() == _Erasednode)
			{
				_Root() = _Pnode;	// link down from root
			}
			else if (_Erasednode->_Parent->_Left == _Erasednode)
			{
				_Erasednode->_Parent->_Left = _Pnode;	// link down to left
			}
			else
			{
				_Erasednode->_Parent->_Right = _Pnode;	// link down to right
			}

			_Pnode->_Parent = _Erasednode->_Parent;	// link successor up
			_STD swap(_Pnode->_Color, _Erasednode->_Color);	// recolor it
		}

		if (_Erasednode->_Color == this->_Black)
		{	// erasing black link, must recolor/rebalance tree
			for (; _Fixnode != _Root()
				&& _Fixnode->_Color == this->_Black;
				_Fixnodeparent = _Fixnode->_Parent)
				if (_Fixnode == _Fixnodeparent->_Left)
				{	// fixup left subtree
					_Pnode = _Fixnodeparent->_Right;
					if (_Pnode->_Color == this->_Red)
					{	// rotate red up from right subtree
						_Pnode->_Color = this->_Black;
						_Fixnodeparent->_Color = this->_Red;
						_Lrotate(_Fixnodeparent);
						_Pnode = _Fixnodeparent->_Right;
					}

					if (_Pnode->_Isnil)
						_Fixnode = _Fixnodeparent;	// shouldn't happen
					else if (_Pnode->_Left->_Color == this->_Black
						&& _Pnode->_Right->_Color == this->_Black)
					{	// redden right subtree with black children
						_Pnode->_Color = this->_Red;
						_Fixnode = _Fixnodeparent;
					}
					else
					{	// must rearrange right subtree
						if (_Pnode->_Right->_Color == this->_Black)
						{	// rotate red up from left sub-subtree
							_Pnode->_Left->_Color = this->_Black;
							_Pnode->_Color = this->_Red;
							_Rrotate(_Pnode);
							_Pnode = _Fixnodeparent->_Right;
						}

						_Pnode->_Color = _Fixnodeparent->_Color;
						_Fixnodeparent->_Color = this->_Black;
						_Pnode->_Right->_Color = this->_Black;
						_Lrotate(_Fixnodeparent);
						break;	// tree now recolored/rebalanced
					}
				}
				else
				{	// fixup right subtree
					_Pnode = _Fixnodeparent->_Left;
					if (_Pnode->_Color == this->_Red)
					{	// rotate red up from left subtree
						_Pnode->_Color = this->_Black;
						_Fixnodeparent->_Color = this->_Red;
						_Rrotate(_Fixnodeparent);
						_Pnode = _Fixnodeparent->_Left;
					}

					if (_Pnode->_Isnil)
						_Fixnode = _Fixnodeparent;	// shouldn't happen
					else if (_Pnode->_Right->_Color == this->_Black
						&& _Pnode->_Left->_Color == this->_Black)
					{	// redden left subtree with black children
						_Pnode->_Color = this->_Red;
						_Fixnode = _Fixnodeparent;
					}
					else
					{	// must rearrange left subtree
						if (_Pnode->_Left->_Color == this->_Black)
						{	// rotate red up from right sub-subtree
							_Pnode->_Right->_Color = this->_Black;
							_Pnode->_Color = this->_Red;
							_Lrotate(_Pnode);
							_Pnode = _Fixnodeparent->_Left;
						}

						_Pnode->_Color = _Fixnodeparent->_Color;
						_Fixnodeparent->_Color = this->_Black;
						_Pnode->_Left->_Color = this->_Black;
						_Rrotate(_Fixnodeparent);
						break;	// tree now recolored/rebalanced
					}
				}

			_Fixnode->_Color = this->_Black;	// stopping node is black
		}

		if (0 < _Mysize)
			--_Mysize;

		return (_Erasednode);
	}

	_Nodeptr _Myhead;	// pointer to head node
	size_type _Mysize;	// number of elements
};

// CLASS TEMPLATE _Tree_comp_alloc
template<class _Traits>
class _Tree_comp_alloc
{	// base class for tree to hold ordering predicate, allocator
public:
	using allocator_type = typename _Traits::allocator_type;
	using key_compare = typename _Traits::key_compare;

	using _Alloc_types = _Tree_base_types<typename _Traits::value_type, allocator_type>;

	using _Alnode = typename _Alloc_types::_Alnode;
	using _Alnode_traits = typename _Alloc_types::_Alnode_traits;
	using _Alproxy = _Rebind_alloc_t<_Alnode, _Container_proxy>;
	using _Alproxy_traits = allocator_traits<_Alproxy>;
	using _Node = typename _Alloc_types::_Node;
	using _Nodeptr = typename _Alloc_types::_Nodeptr;
	using _Val_types = typename _Alloc_types::_Val_types;

	using value_type = typename _Val_types::value_type;
	using size_type = typename _Val_types::size_type;
	using difference_type = typename _Val_types::difference_type;
	using pointer = typename _Val_types::pointer;
	using const_pointer = typename _Val_types::const_pointer;
	using reference = value_type & ;
	using const_reference = const value_type&;

	using iterator = _Tree_iterator<_Tree_val<_Val_types>>;
	using const_iterator = _Tree_const_iterator<_Tree_val<_Val_types>>;
	using _Unchecked_iterator = _Tree_unchecked_iterator<_Tree_val<_Val_types>>;
	using _Unchecked_const_iterator = _Tree_unchecked_const_iterator<_Tree_val<_Val_types>>;

	enum _Redbl
	{	// colors for link to parent
		_Red, _Black
	};

	_Tree_comp_alloc(const key_compare& _Parg)
		: _Mypair(_One_then_variadic_args_t(), _Parg,
			_Zero_then_variadic_args_t())
	{	// construct from comparator
		_Construct();
	}

	template<class _Any_alloc,
		class = enable_if_t<!is_same_v<remove_cv_t<remove_reference_t<_Any_alloc>>, _Tree_comp_alloc>>>
		_Tree_comp_alloc(const key_compare& _Parg, _Any_alloc&& _Al)
		: _Mypair(_One_then_variadic_args_t(), _Parg,
			_One_then_variadic_args_t(),
			_STD forward<_Any_alloc>(_Al))
	{	// construct from comparator, allocator
		_Construct();
	}

#if _ITERATOR_DEBUG_LEVEL == 0
	void _Construct()
	{	// construct head node
		_Get_data()._Myhead = _Buyheadnode();
	}

	~_Tree_comp_alloc() noexcept
	{	// destroy head node
		_Freeheadnode(_Get_data()._Myhead);
	}

	void _Alloc_proxy()
	{	// do nothing
	}

	void _Free_proxy()
	{	// do nothing
	}

#else /* _ITERATOR_DEBUG_LEVEL == 0 */
	void _Construct()
	{	// construct head node, proxy
		auto& _My_data = _Get_data();
		_My_data._Myhead = _Buyheadnode();
		_TRY_BEGIN
			_Alloc_proxy();
		_CATCH_ALL
			_Freeheadnode(_My_data._Myhead);
		_RERAISE;
		_CATCH_END
	}

	~_Tree_comp_alloc() noexcept
	{	// destroy head node, proxy
		_Freeheadnode(_Get_data()._Myhead);
		_Free_proxy();
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

	void _Copy_alloc(const _Alnode& _Al)
	{	// replace old allocator
		const bool _Reload = _Alnode_traits::propagate_on_container_copy_assignment::value
			&& _Getal() != _Al;

		if (_Reload)
		{
			_Free_proxy();
			_Freeheadnode(_Get_data()._Myhead);
		}

		_Pocca(_Getal(), _Al);

		if (_Reload)
		{
			_Get_data()._Myhead = _Buyheadnode();
			_Alloc_proxy();
		}
	}

	void _Move_alloc(_Alnode& _Al)
	{	// replace old allocator
		const bool _Reload = _Alnode_traits::propagate_on_container_move_assignment::value
			&& _Getal() != _Al;

		if (_Reload)
		{
			_Free_proxy();
			_Freeheadnode(_Get_data()._Myhead);
		}

		_Pocma(_Getal(), _Al);

		if (_Reload)
		{
			_Get_data()._Myhead = _Buyheadnode();
			_Alloc_proxy();
		}
	}

	void _Orphan_all()
	{	// orphan all iterators
		_Get_data()._Orphan_all();
	}

	void _Swap_all(_Tree_comp_alloc& _Right)
	{	// swap all iterators
		_Get_data()._Swap_all(_Right._Get_data());
	}

	_Nodeptr _Buyheadnode()
	{	// get head node using current allocator
		_Alnode& _Al = _Getal();
		_Nodeptr _Pnode = _Al.allocate(1);

		_TRY_BEGIN
			_Alnode_traits::construct(_Al, _STD addressof(_Pnode->_Left), _Pnode);
		_Alnode_traits::construct(_Al, _STD addressof(_Pnode->_Parent), _Pnode);
		_Alnode_traits::construct(_Al, _STD addressof(_Pnode->_Right), _Pnode);
		_CATCH_ALL
			_Al.deallocate(_Pnode, 1);
		_RERAISE;
		_CATCH_END

			_Pnode->_Color = _Black;
		_Pnode->_Isnil = true;
		return (_Pnode);
	}

	void _Freeheadnode(_Nodeptr _Pnode)
	{	// free head node using current allocator
		_Node::_Freenode0(_Getal(), _Pnode);
	}

	_Nodeptr _Buynode0()
	{	// allocate a non-value node
		_Alnode& _Al = _Getal();
		_Nodeptr _Pnode = _Al.allocate(1);

		auto& _My_data = _Get_data();

		_TRY_BEGIN
			_Alnode_traits::construct(_Al, _STD addressof(_Pnode->_Left), _My_data._Myhead);
		_Alnode_traits::construct(_Al, _STD addressof(_Pnode->_Parent), _My_data._Myhead);
		_Alnode_traits::construct(_Al, _STD addressof(_Pnode->_Right), _My_data._Myhead);
		_CATCH_ALL
			_Al.deallocate(_Pnode, 1);
		_RERAISE;
		_CATCH_END

			return (_Pnode);
	}

	void _Freenode0(_Nodeptr _Pnode)
	{	// free non-value node using current allocator
		_Node::_Freenode0(_Getal(), _Pnode);
	}

	template<class... _Valty>
	_Nodeptr _Buynode(_Valty&&... _Val)
	{	// allocate a node with defaults and set links and value
		_Nodeptr _Pnode = _Buynode0();

		_Pnode->_Color = _Red;
		_Pnode->_Isnil = false;

		_TRY_BEGIN
			_Alnode_traits::construct(_Getal(),
				_STD addressof(_Pnode->_Myval), _STD forward<_Valty>(_Val)...);
		_CATCH_ALL
			_Freenode0(_Pnode);
		_RERAISE;
		_CATCH_END

			return (_Pnode);
	}

	key_compare& _Getcomp() noexcept
	{	// return reference to ordering predicate
		return (_Mypair._Get_first());
	}

	const key_compare& _Getcomp() const noexcept
	{	// return const reference to ordering predicate
		return (_Mypair._Get_first());
	}

	_Alnode& _Getal() noexcept
	{	// return reference to allocator
		return (_Mypair._Get_second()._Get_first());
	}

	const _Alnode& _Getal() const noexcept
	{	// return const reference to allocator
		return (_Mypair._Get_second()._Get_first());
	}

	_Tree_val<_Val_types>& _Get_data() noexcept
	{	// return reference to _Tree_val
		return (_Mypair._Get_second()._Get_second());
	}

	const _Tree_val<_Val_types>& _Get_data() const noexcept
	{	// return const reference to _Tree_val
		return (_Mypair._Get_second()._Get_second());
	}

private:
	_Compressed_pair<key_compare,
		_Compressed_pair<_Alnode, _Tree_val<_Val_types>>> _Mypair;
};

// CLASS TEMPLATE _Tree
template<class _Traits>
class _Tree
	: public _Tree_comp_alloc<_Traits>
{	// ordered red-black tree for map/multimap/set/multiset
public:
	using _Mybase = _Tree_comp_alloc<_Traits>;

	using key_type = typename _Traits::key_type;
	using value_compare = typename _Traits::value_compare;
	enum
	{	// get multi parameter
		_Multi = _Traits::_Multi
	};

	using _Node = typename _Mybase::_Node;
	using _Nodeptr = typename _Mybase::_Nodeptr;
	using _Alnode = typename _Mybase::_Alnode;
	using _Alnode_traits = typename _Mybase::_Alnode_traits;

	using key_compare = typename _Mybase::key_compare;
	using allocator_type = typename _Mybase::allocator_type;

	using value_type = typename _Mybase::value_type;
	using size_type = typename _Mybase::size_type;
	using difference_type = typename _Mybase::difference_type;
	using pointer = typename _Mybase::pointer;
	using const_pointer = typename _Mybase::const_pointer;
	using reference = value_type & ;
	using const_reference = const value_type&;

	using iterator = conditional_t<is_same_v<key_type, value_type>,
		typename _Mybase::const_iterator,
		typename _Mybase::iterator>;
	using const_iterator = typename _Mybase::const_iterator;
	using _Unchecked_iterator = conditional_t<is_same_v<key_type, value_type>,
		typename _Mybase::_Unchecked_const_iterator,
		typename _Mybase::_Unchecked_iterator>;
	using _Unchecked_const_iterator = typename _Mybase::_Unchecked_const_iterator;

	using reverse_iterator = _STD reverse_iterator<iterator>;
	using const_reverse_iterator = _STD reverse_iterator<const_iterator>;

	using _Pairib = pair<iterator, bool>;
	using _Pairii = pair<iterator, iterator>;
	using _Paircc = pair<const_iterator, const_iterator>;

	using _Mytreeval = _Tree_val<typename _Mybase::_Val_types>;

	struct _Copy_tag
	{	// signals copying
	};
	struct _Move_tag
	{	// signals moving
	};

	_Tree(const key_compare& _Parg)
		: _Mybase(_Parg)
	{	// construct empty tree from comparator
	}

	_Tree(const key_compare& _Parg,
		const allocator_type& _Al)
		: _Mybase(_Parg, _Al)
	{	// construct empty tree from comparator, allocator
	}

	template<class _Any_alloc>
	_Tree(const _Tree& _Right, _Any_alloc&& _Al)
		: _Mybase(_Right.key_comp(), _STD forward<_Any_alloc>(_Al))
	{	// construct tree by copying _Right, allocator
		_TRY_BEGIN
			_Copy(_Right, _Copy_tag());
		_CATCH_ALL
			_Tidy();
		_RERAISE;
		_CATCH_END
	}

	_Tree(_Tree&& _Right)
		: _Mybase(_Right.key_comp(), _STD move(_Right._Getal()))
	{	// construct tree by moving _Right
		_Assign_rv(_STD move(_Right), true_type());
	}

	_Tree(_Tree&& _Right, const allocator_type& _Al)
		: _Mybase(_Right.key_comp(), _Al)
	{	// construct tree by moving _Right, allocator
		_Assign_rv(_STD move(_Right), false_type());
	}

	_Tree& operator=(_Tree&& _Right)
	{	// assign by moving _Right
		if (this != _STD addressof(_Right))
		{	// different, move it
			clear();
			this->_Move_alloc(_Right._Getal());
			this->_Getcomp() = _Right._Getcomp();
			_Assign_rv(_STD move(_Right), bool_constant<_Always_equal_after_move<_Alnode>>{});
		}
		return (*this);
	}

	void _Assign_rv(_Tree&& _Right, true_type)
	{	// move from _Right, stealing its contents
		this->_Swap_all(_Right);
		_Swap_adl(this->_Getcomp(), _Right._Getcomp());
		auto& _My_data = this->_Get_data();
		auto& _Right_data = _Right._Get_data();
		_Swap_adl(_My_data._Myhead, _Right_data._Myhead);
		_STD swap(_My_data._Mysize, _Right_data._Mysize);
	}

	void _Assign_rv(_Tree&& _Right, false_type)
	{	// move from _Right, possibly moving its contents
		if (this->_Getal() == _Right._Getal())
		{
			_Assign_rv(_STD move(_Right), true_type());
		}
		else
		{
			_Copy(_Right, _Move_tag());
		}
	}

	template<class... _Valty>
	_Pairib emplace(_Valty&&... _Val)
	{	// try to insert value_type(_Val...), favoring right side
		_Nodeptr _Newnode = this->_Buynode(_STD forward<_Valty>(_Val)...);
		return (_Insert_nohint(false, _Newnode->_Myval, _Newnode));
	}

	template<class... _Valty>
	iterator emplace_hint(const_iterator _Where, _Valty&&... _Val)
	{	// insert value_type(_Val...) at _Where
		_Nodeptr _Newnode = this->_Buynode(_STD forward<_Valty>(_Val)...);
		return (_Insert_hint(_Where, _Newnode->_Myval, _Newnode));
	}

	~_Tree() noexcept
	{	// destroy tree
		_Tidy();
	}

	_Tree& operator=(const _Tree& _Right)
	{	// replace contents from _Right
		if (this != _STD addressof(_Right))
		{	// different, assign it
			clear();
			this->_Copy_alloc(_Right._Getal());
			this->_Getcomp() = _Right._Getcomp();
			_Copy(_Right, _Copy_tag());
		}

		return (*this);
	}

	_NODISCARD iterator begin() noexcept
	{	// return iterator for beginning of mutable sequence
		auto& _My_data = this->_Get_data();
		return (iterator(_My_data._Lmost(), _STD addressof(_My_data)));
	}

	_NODISCARD const_iterator begin() const noexcept
	{	// return iterator for beginning of nonmutable sequence
		auto& _My_data = this->_Get_data();
		return (const_iterator(_My_data._Lmost(), _STD addressof(_My_data)));
	}

	_NODISCARD iterator end() noexcept
	{	// return iterator for end of mutable sequence
		auto& _My_data = this->_Get_data();
		return (iterator(_My_data._Myhead, _STD addressof(_My_data)));
	}

	_NODISCARD const_iterator end() const noexcept
	{	// return iterator for end of nonmutable sequence
		auto& _My_data = this->_Get_data();
		return (const_iterator(_My_data._Myhead, _STD addressof(_My_data)));
	}

	_Unchecked_iterator _Unchecked_begin() noexcept
	{	// return unchecked iterator for beginning of mutable sequence
		return (_Unchecked_iterator(this->_Get_data()._Lmost(), nullptr));
	}

	_Unchecked_const_iterator _Unchecked_begin() const noexcept
	{	// return unchecked iterator for beginning of nonmutable sequence
		return (_Unchecked_const_iterator(this->_Get_data()._Lmost(), nullptr));
	}

	_Unchecked_iterator _Unchecked_end() noexcept
	{	// return unchecked iterator for end of mutable sequence
		return (_Unchecked_iterator(this->_Get_data()._Myhead, nullptr));
	}

	_Unchecked_const_iterator _Unchecked_end() const noexcept
	{	// return unchecked iterator for end of nonmutable sequence
		return (_Unchecked_const_iterator(this->_Get_data()._Myhead, nullptr));
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

	_NODISCARD size_type size() const noexcept
	{	// return length of sequence
		return (this->_Get_data()._Mysize);
	}

	_NODISCARD size_type max_size() const noexcept
	{	// return maximum possible length of sequence
		return (_Alnode_traits::max_size(this->_Getal()));
	}

	_NODISCARD bool empty() const noexcept
	{	// return true only if sequence is empty
		return (size() == 0);
	}

	_NODISCARD allocator_type get_allocator() const noexcept
	{	// return allocator object for values
		return (static_cast<allocator_type>(this->_Getal()));
	}

	_NODISCARD key_compare key_comp() const
	{	// return object for comparing keys
		return (this->_Getcomp());
	}

	_NODISCARD value_compare value_comp() const
	{	// return object for comparing values
		return (value_compare(key_comp()));
	}

	template<bool _Multi2 = _Multi,
		enable_if_t<!_Multi2, int> = 0>
		_Pairib insert(const value_type& _Val)
	{	// try to insert node with value _Val, favoring right side
		return (_Insert_nohint(false, _Val, _Not_a_node_tag()));
	}

	template<bool _Multi2 = _Multi,
		enable_if_t<_Multi2, int> = 0>
		iterator insert(const value_type& _Val)
	{	// try to insert node with value _Val, favoring right side
		return (_Insert_nohint(false, _Val, _Not_a_node_tag()).first);
	}

	template<bool _Multi2 = _Multi,
		enable_if_t<!_Multi2, int> = 0>
		_Pairib insert(value_type&& _Val)
	{	// try to insert node with value _Val, favoring right side
		return (_Insert_nohint(false, _STD move(_Val), _Not_a_node_tag()));
	}

	template<bool _Multi2 = _Multi,
		enable_if_t<_Multi2, int> = 0>
		iterator insert(value_type&& _Val)
	{	// try to insert node with value _Val, favoring right side
		return (_Insert_nohint(false, _STD move(_Val), _Not_a_node_tag()).first);
	}

	iterator insert(const_iterator _Where,
		const value_type& _Val)
	{	// try to insert node with value _Val using _Where as a hint
		return (_Insert_hint(_Where, _Val, _Not_a_node_tag()));
	}

	iterator insert(const_iterator _Where, value_type&& _Val)
	{	// try to insert node with value _Val using _Where as a hint
		return (_Insert_hint(_Where, _STD move(_Val), _Not_a_node_tag()));
	}

	template<class _Iter>
	void insert(_Iter _First, _Iter _Last)
	{	// insert [_First, _Last) one at a time
		_Adl_verify_range(_First, _Last);
		auto _UFirst = _Get_unwrapped(_First);
		const auto _ULast = _Get_unwrapped(_Last);
		for (; _UFirst != _ULast; ++_UFirst)
		{
			emplace_hint(end(), *_UFirst);
		}
	}

	void insert(initializer_list<value_type> _Ilist)
	{	// insert initializer_list
		insert(_Ilist.begin(), _Ilist.end());
	}

	template<class _Iter = iterator,
		class = enable_if_t<!is_same_v<_Iter, const_iterator>>>
		iterator erase(iterator _Where)
	{	// erase element at _Where
		return (erase(const_iterator{ _Where }));
	}

	iterator erase(const_iterator _Where)
	{	// erase element at _Where
		auto& _My_data = this->_Get_data();
#if _ITERATOR_DEBUG_LEVEL == 2
		_STL_VERIFY(_Where._Getcont() == _STD addressof(_My_data)
			&& !_Where._Ptr->_Isnil, "map/set erase iterator outside range");
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */

		const_iterator _Successor = _Where;
		++_Successor;	// save successor iterator for return
		_Nodeptr _Erasednode = _My_data._Extract(_Where);	// node to erase

#if _ITERATOR_DEBUG_LEVEL == 2
		_Orphan_ptr(_Erasednode);
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */

		_Alnode& _Al = this->_Getal();
		_Alnode_traits::destroy(_Al, _STD addressof(_Erasednode->_Myval));	// delete erased node
		_Node::_Freenode0(_Al, _Erasednode);

		return (iterator(_Successor._Ptr, _STD addressof(_My_data)));	// return successor iterator
	}

	iterator erase(const_iterator _First, const_iterator _Last)
	{	// erase [_First, _Last)
		if (_First == begin() && _Last == end())
		{	// erase all
			clear();
			return (begin());
		}
		else
		{	// partial erase, one at a time
			while (_First != _Last)
				erase(_First++);
			return (iterator(_First._Ptr, _STD addressof(this->_Get_data())));
		}
	}

	size_type erase(const key_type& _Keyval)
	{	// erase and count all that match _Keyval
		_Pairii _Where = equal_range(_Keyval);
		const auto _Num = static_cast<size_type>(_STD distance(_Where.first, _Where.second));
		erase(_Where.first, _Where.second);
		return (_Num);
	}

	void clear() noexcept
	{	// erase all
#if _ITERATOR_DEBUG_LEVEL == 2
		this->_Orphan_ptr(nullptr);
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */

		auto& _My_data = this->_Get_data();
		auto _Head = _My_data._Myhead;
		_Erase(_My_data._Root());
		_My_data._Root() = _Head;
		_My_data._Lmost() = _Head;
		_My_data._Rmost() = _Head;
		_My_data._Mysize = 0;
	}

	_NODISCARD iterator find(const key_type& _Keyval)
	{	// find an element in mutable sequence that matches _Keyval
		iterator _Where = lower_bound(_Keyval);
		return (_Where == end()
			|| _DEBUG_LT_PRED(this->_Getcomp(),
				_Keyval, this->_Key(_Where._Ptr))
			? end() : _Where);
	}

	_NODISCARD const_iterator find(const key_type& _Keyval) const
	{	// find an element in nonmutable sequence that matches _Keyval
		const_iterator _Where = lower_bound(_Keyval);
		return (_Where == end()
			|| _DEBUG_LT_PRED(this->_Getcomp(),
				_Keyval, this->_Key(_Where._Ptr))
			? end() : _Where);
	}

	template<class _Other,
		class _Mycomp = key_compare,
		class = typename _Mycomp::is_transparent>
		_NODISCARD iterator find(const _Other& _Keyval)
	{	// find an element in mutable sequence that matches _Keyval
		iterator _Where = lower_bound(_Keyval);
		return (_Where == end()
			|| _DEBUG_LT_PRED(this->_Getcomp(),
				_Keyval, this->_Key(_Where._Ptr))
			? end() : _Where);
	}

	template<class _Other,
		class _Mycomp = key_compare,
		class = typename _Mycomp::is_transparent>
		_NODISCARD const_iterator find(const _Other& _Keyval) const
	{	// find an element in nonmutable sequence that matches _Keyval
		const_iterator _Where = lower_bound(_Keyval);
		return (_Where == end()
			|| _DEBUG_LT_PRED(this->_Getcomp(),
				_Keyval, this->_Key(_Where._Ptr))
			? end() : _Where);
	}

	_NODISCARD size_type count(const key_type& _Keyval) const
	{	// count all elements that match _Keyval
		_Paircc _Ans = equal_range(_Keyval);
		return (static_cast<size_type>(_STD distance(_Ans.first, _Ans.second)));
	}

	template<class _Other,
		class _Mycomp = key_compare,
		class = typename _Mycomp::is_transparent>
		_NODISCARD size_type count(const _Other& _Keyval) const
	{	// count all elements that match _Keyval
		_Paircc _Ans = equal_range(_Keyval);
		return (static_cast<size_type>(_STD distance(_Ans.first, _Ans.second)));
	}

	_NODISCARD iterator lower_bound(const key_type& _Keyval)
	{	// find leftmost node not less than _Keyval in mutable tree
		return (iterator(_Lbound(_Keyval), _STD addressof(this->_Get_data())));
	}

	_NODISCARD const_iterator lower_bound(const key_type& _Keyval) const
	{	// find leftmost node not less than _Keyval in nonmutable tree
		return (const_iterator(_Lbound(_Keyval), _STD addressof(this->_Get_data())));
	}

	template<class _Other,
		class _Mycomp = key_compare,
		class = typename _Mycomp::is_transparent>
		_NODISCARD iterator lower_bound(const _Other& _Keyval)
	{	// find leftmost node not less than _Keyval in mutable tree
		return (iterator(_Lbound(_Keyval), _STD addressof(this->_Get_data())));
	}

	template<class _Other,
		class _Mycomp = key_compare,
		class = typename _Mycomp::is_transparent>
		_NODISCARD const_iterator lower_bound(const _Other& _Keyval) const
	{	// find leftmost node not less than _Keyval in nonmutable tree
		return (const_iterator(_Lbound(_Keyval), _STD addressof(this->_Get_data())));
	}

	_NODISCARD iterator upper_bound(const key_type& _Keyval)
	{	// find leftmost node greater than _Keyval in mutable tree
		return (iterator(_Ubound(_Keyval), _STD addressof(this->_Get_data())));
	}

	_NODISCARD const_iterator upper_bound(const key_type& _Keyval) const
	{	// find leftmost node greater than _Keyval in nonmutable tree
		return (const_iterator(_Ubound(_Keyval), _STD addressof(this->_Get_data())));
	}

	template<class _Other,
		class _Mycomp = key_compare,
		class = typename _Mycomp::is_transparent>
		_NODISCARD iterator upper_bound(const _Other& _Keyval)
	{	// find leftmost node greater than _Keyval in mutable tree
		return (iterator(_Ubound(_Keyval), _STD addressof(this->_Get_data())));
	}

	template<class _Other,
		class _Mycomp = key_compare,
		class = typename _Mycomp::is_transparent>
		_NODISCARD const_iterator upper_bound(const _Other& _Keyval) const
	{	// find leftmost node greater than _Keyval in nonmutable tree
		return (const_iterator(_Ubound(_Keyval), _STD addressof(this->_Get_data())));
	}

	_NODISCARD _Pairii equal_range(const key_type& _Keyval)
	{	// find range equivalent to _Keyval in mutable tree
		return (_Eqrange(_Keyval));
	}

	_NODISCARD _Paircc equal_range(const key_type& _Keyval) const
	{	// find range equivalent to _Keyval in nonmutable tree
		return (_Eqrange(_Keyval));
	}

	template<class _Other,
		class _Mycomp = key_compare,
		class = typename _Mycomp::is_transparent>
		_NODISCARD _Pairii equal_range(const _Other& _Keyval)
	{	// find range equivalent to _Keyval in mutable tree
		return (_Eqrange(_Keyval));
	}

	template<class _Other,
		class _Mycomp = key_compare,
		class = typename _Mycomp::is_transparent>
		_NODISCARD _Paircc equal_range(const _Other& _Keyval) const
	{	// find range equivalent to _Keyval in nonmutable tree
		return (_Eqrange(_Keyval));
	}

	void swap(_Tree& _Right) _NOEXCEPT_COND(_Is_nothrow_swappable<key_compare>::value) // strengthened
	{	// exchange contents with _Right
		if (this != _STD addressof(_Right))
		{	// (maybe) swap allocators, swap control information
			_Swap_adl(this->_Getcomp(), _Right._Getcomp());
			_Pocs(this->_Getal(), _Right._Getal());
			this->_Swap_all(_Right);
			auto& _My_data = this->_Get_data();
			auto& _Right_data = _Right._Get_data();
			_Swap_adl(_My_data._Myhead, _Right_data._Myhead);
			_STD swap(_My_data._Mysize, _Right_data._Mysize);
		}
	}

protected:
	template<class _Valty>
	_Nodeptr _Buy_if_not_node(_Nodeptr _Node, _Valty&&)
	{	// node exists, just return it
		return (_Node);
	}

	template<class _Valty>
	_Nodeptr _Buy_if_not_node(_Not_a_node_tag, _Valty&& _Val)
	{	// node doesn't exist, make it
		return (this->_Buynode(_STD forward<_Valty>(_Val)));
	}

	void _Destroy_if_node(_Nodeptr _Newnode)
	{	// node exists, destroy it
		_Alnode& _Al = this->_Getal();
		_Alnode_traits::destroy(_Al, _STD addressof(_Newnode->_Myval));
		_Node::_Freenode0(_Al, _Newnode);
	}

	void _Destroy_if_node(_Not_a_node_tag)
	{	// node doesn't exist, do nothing
	}

	template<class _Valty,
		class _Nodety>
		iterator _Insert_hint(const_iterator _Where,
			_Valty&& _Val, _Nodety _Newnode)
	{	// try to insert node using _Where as a hint
		const_iterator _Next;
		bool _Leftish = false;	// assume nearest point is end of sequence
		auto& _My_data = this->_Get_data();

		_TRY_BEGIN
#if _ITERATOR_DEBUG_LEVEL == 2
			_STL_VERIFY(_Where._Getcont() == _STD addressof(_My_data), "map/set insert iterator outside range");
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */

#pragma warning(push)
#pragma warning(disable: 4127)	// conditional expression is constant
		if (size() == 0)
		{
			return (_Insert_at(true, _My_data._Myhead,
				_STD forward<_Valty>(_Val), _Newnode));	// empty tree
		}
		else if (this->_Multi)
		{	// insert even if duplicate
			if (_Where == begin())
			{	// insert at beginning if before first element
				if (!_DEBUG_LT_PRED(this->_Getcomp(),
					this->_Key(_Where._Ptr), this->_Kfn(_Val)))
					return (_Insert_at(true, _Where._Ptr,
						_STD forward<_Valty>(_Val), _Newnode));
				_Leftish = true;	// nearest point is beginning of sequence
			}
			else if (_Where == end())
			{	// insert at end if after last element
				if (!_DEBUG_LT_PRED(this->_Getcomp(),
					this->_Kfn(_Val), this->_Key(_My_data._Rmost())))
					return (_Insert_at(false, _My_data._Rmost(),
						_STD forward<_Valty>(_Val), _Newnode));
			}
			else if (!_DEBUG_LT_PRED(this->_Getcomp(),
				this->_Key(_Where._Ptr), this->_Kfn(_Val))
				&& !_DEBUG_LT_PRED(this->_Getcomp(),
					this->_Kfn(_Val),
					this->_Key((--(_Next = _Where))._Ptr)))
			{	// insert before _Where
				if (_Next._Ptr->_Right->_Isnil)
					return (_Insert_at(false, _Next._Ptr,
						_STD forward<_Valty>(_Val), _Newnode));
				else
					return (_Insert_at(true, _Where._Ptr,
						_STD forward<_Valty>(_Val), _Newnode));
			}
			else if (!_DEBUG_LT_PRED(this->_Getcomp(),
				this->_Kfn(_Val), this->_Key(_Where._Ptr))
				&& (++(_Next = _Where) == end()
					|| !_DEBUG_LT_PRED(this->_Getcomp(),
						this->_Key(_Next._Ptr), this->_Kfn(_Val))))
			{	// insert after _Where
				if (_Where._Ptr->_Right->_Isnil)
					return (_Insert_at(false, _Where._Ptr,
						_STD forward<_Valty>(_Val), _Newnode));
				else
					return (_Insert_at(true, _Next._Ptr,
						_STD forward<_Valty>(_Val), _Newnode));
			}
			else
			{
				_Leftish = true;	// nearest point is beginning of sequence
			}
		}
		else
		{	// insert only if unique
			if (_Where == begin())
			{	// insert at beginning if before first element
				if (_DEBUG_LT_PRED(this->_Getcomp(),
					this->_Kfn(_Val), this->_Key(_Where._Ptr)))
				{
					return (_Insert_at(true, _Where._Ptr,
						_STD forward<_Valty>(_Val), _Newnode));
				}
			}
			else if (_Where == end())
			{	// insert at end if after last element
				if (_DEBUG_LT_PRED(this->_Getcomp(),
					this->_Key(_My_data._Rmost()), this->_Kfn(_Val)))
				{
					return (_Insert_at(false, _My_data._Rmost(),
						_STD forward<_Valty>(_Val), _Newnode));
				}
			}
			else if (_DEBUG_LT_PRED(this->_Getcomp(),
				this->_Kfn(_Val), this->_Key(_Where._Ptr))
				&& _DEBUG_LT_PRED(this->_Getcomp(),
					this->_Key((--(_Next = _Where))._Ptr),
					this->_Kfn(_Val)))
			{	// insert before _Where
				if (_Next._Ptr->_Right->_Isnil)
				{
					return (_Insert_at(false, _Next._Ptr,
						_STD forward<_Valty>(_Val), _Newnode));
				}
				else
				{
					return (_Insert_at(true, _Where._Ptr,
						_STD forward<_Valty>(_Val), _Newnode));
				}
			}
			else if (_DEBUG_LT_PRED(this->_Getcomp(),
				this->_Key(_Where._Ptr), this->_Kfn(_Val))
				&& (++(_Next = _Where) == end()
					|| _DEBUG_LT_PRED(this->_Getcomp(),
						this->_Kfn(_Val), this->_Key(_Next._Ptr))))
			{	// insert after _Where
				if (_Where._Ptr->_Right->_Isnil)
				{
					return (_Insert_at(false, _Where._Ptr,
						_STD forward<_Valty>(_Val), _Newnode));
				}
				else
				{
					return (_Insert_at(true, _Next._Ptr,
						_STD forward<_Valty>(_Val), _Newnode));
				}
			}
		}
#pragma warning(pop)
		_CATCH_ALL
			_Destroy_if_node(_Newnode);
		_RERAISE;
		_CATCH_END

			return (_Insert_nohint(_Leftish,
				_STD forward<_Valty>(_Val), _Newnode).first);
	}

	template<class _Valty,
		class _Nodety>
		_Pairib _Insert_nohint(bool _Leftish,
			_Valty&& _Val, _Nodety _Newnode)
	{	// try to insert node, on left if _Leftish
		auto& _My_data = this->_Get_data();
		_TRY_BEGIN
			_Nodeptr _Wherenode = _My_data._Myhead;
		_Nodeptr _Trynode = _Wherenode->_Parent;
		bool _Addleft = true;	// add to left of head if tree empty

		while (!_Trynode->_Isnil)
		{	// look for leaf to insert before (_Addleft) or after
			_Wherenode = _Trynode;
			if (_Leftish)
			{
				_Addleft = !_DEBUG_LT_PRED(this->_Getcomp(),
					this->_Key(_Trynode),
					this->_Kfn(_Val));	// favor left end
			}
			else
			{
				_Addleft = _DEBUG_LT_PRED(this->_Getcomp(),
					this->_Kfn(_Val),
					this->_Key(_Trynode));	// favor right end
			}

			_Trynode = _Addleft ? _Trynode->_Left : _Trynode->_Right;
		}

#pragma warning(push)
#pragma warning(disable: 4127)	// conditional expression is constant
		if (this->_Multi)
		{
			return (_Pairib(_Insert_at(_Addleft, _Wherenode,
				_STD forward<_Valty>(_Val), _Newnode), true));
		}
		else
		{	// insert only if unique
			iterator _Where = iterator(_Wherenode, _STD addressof(_My_data));
			if (!_Addleft)
			{	// need to test if insert after is okay
			}
			else if (_Where == begin())
			{
				return (_Pairib(_Insert_at(true, _Wherenode,
					_STD forward<_Valty>(_Val), _Newnode), true));
			}
			else
			{
				--_Where;	// need to test if insert before is okay
			}

			if (_DEBUG_LT_PRED(this->_Getcomp(),
				this->_Key(_Where._Ptr),
				this->_Kfn(_Val)))
			{
				return (_Pairib(_Insert_at(_Addleft, _Wherenode,
					_STD forward<_Valty>(_Val), _Newnode), true));
			}
			else
			{	// duplicate, don't insert
				_Destroy_if_node(_Newnode);
				return (_Pairib(_Where, false));
			}
		}
#pragma warning(pop)
		_CATCH_ALL
			_Destroy_if_node(_Newnode);
		_RERAISE;
		_CATCH_END
	}

	template<class _Valty,
		class _Nodety>
		iterator _Insert_at(bool _Addleft, _Nodeptr _Wherenode, _Valty&& _Val, _Nodety _Node)
	{	// add node with value next to _Wherenode, to left if _Addleft
		auto& _My_data = this->_Get_data();
		if (max_size() - 1 <= _My_data._Mysize)
		{	// tree would get too big, fail
			_Destroy_if_node(_Node);
			_Xlength_error("map/set<T> too long");
		}

		_Nodeptr _Newnode = _Buy_if_not_node(_Node, _STD forward<_Valty>(_Val));

		++_My_data._Mysize;
		_Newnode->_Parent = _Wherenode;

		if (_Wherenode == _My_data._Myhead)
		{	// first node in tree, just set head values
			_My_data._Root() = _Newnode;
			_My_data._Lmost() = _Newnode;
			_My_data._Rmost() = _Newnode;
		}
		else if (_Addleft)
		{	// add to left of _Wherenode
			_Wherenode->_Left = _Newnode;
			if (_Wherenode == _My_data._Lmost())
			{
				_My_data._Lmost() = _Newnode;
			}
		}
		else
		{	// add to right of _Wherenode
			_Wherenode->_Right = _Newnode;
			if (_Wherenode == _My_data._Rmost())
			{
				_My_data._Rmost() = _Newnode;
			}
		}

		for (_Nodeptr _Pnode = _Newnode; _Pnode->_Parent->_Color == this->_Red; )
		{
			if (_Pnode->_Parent == _Pnode->_Parent->_Parent->_Left)
			{	// fixup red-red in left subtree
				_Wherenode = _Pnode->_Parent->_Parent->_Right;
				if (_Wherenode->_Color == this->_Red)
				{	// parent has two red children, blacken both
					_Pnode->_Parent->_Color = this->_Black;
					_Wherenode->_Color = this->_Black;
					_Pnode->_Parent->_Parent->_Color = this->_Red;
					_Pnode = _Pnode->_Parent->_Parent;
				}
				else
				{	// parent has red and black children
					if (_Pnode == _Pnode->_Parent->_Right)
					{	// rotate right child to left
						_Pnode = _Pnode->_Parent;
						_My_data._Lrotate(_Pnode);
					}

					_Pnode->_Parent->_Color = this->_Black;	// propagate red up
					_Pnode->_Parent->_Parent->_Color = this->_Red;
					_My_data._Rrotate(_Pnode->_Parent->_Parent);
				}
			}
			else
			{	// fixup red-red in right subtree
				_Wherenode = _Pnode->_Parent->_Parent->_Left;
				if (_Wherenode->_Color == this->_Red)
				{	// parent has two red children, blacken both
					_Pnode->_Parent->_Color = this->_Black;
					_Wherenode->_Color = this->_Black;
					_Pnode->_Parent->_Parent->_Color = this->_Red;
					_Pnode = _Pnode->_Parent->_Parent;
				}
				else
				{	// parent has red and black children
					if (_Pnode == _Pnode->_Parent->_Left)
					{	// rotate left child to right
						_Pnode = _Pnode->_Parent;
						_My_data._Rrotate(_Pnode);
					}

					_Pnode->_Parent->_Color = this->_Black;	// propagate red up
					_Pnode->_Parent->_Parent->_Color = this->_Red;
					_My_data._Lrotate(_Pnode->_Parent->_Parent);
				}
			}
		}

		_My_data._Root()->_Color = this->_Black;	// root is always black
		return (iterator(_Newnode, _STD addressof(_My_data)));
	}

	template<class _Moveit>
	void _Copy(const _Tree& _Right, _Moveit _Movefl)
	{	// copy or move entire tree from _Right
		auto& _My_data = this->_Get_data();
		_My_data._Root() = _Copy_nodes(_Right._Get_data()._Root(), _My_data._Myhead, _Movefl);
		_My_data._Mysize = _Right.size();
		if (!_My_data._Root()->_Isnil)
		{	// nonempty tree, look for new smallest and largest
			_My_data._Lmost() = _Mytreeval::_Min(_My_data._Root());
			_My_data._Rmost() = _Mytreeval::_Max(_My_data._Root());
		}
		else
		{	// empty tree, just tidy head pointers
			_My_data._Lmost() = _My_data._Myhead;
			_My_data._Rmost() = _My_data._Myhead;
		}
	}

	template<class _Ty,
		class _Is_set>
		_Nodeptr _Copy_or_move(_Ty& _Val, _Copy_tag, _Is_set)
	{	// copy to new node
		return (this->_Buynode(_Val));
	}

	template<class _Ty>
	_Nodeptr _Copy_or_move(_Ty& _Val, _Move_tag, true_type)
	{	// move to new node -- set
		return (this->_Buynode(_STD move(_Val)));
	}

	template<class _Ty>
	_Nodeptr _Copy_or_move(_Ty& _Val, _Move_tag, false_type)
	{	// move to new node -- map
		return (this->_Buynode(
			_STD move(const_cast<key_type&>(_Val.first)),
			_STD move(_Val.second)));
	}

	template<class _Moveit>
	_Nodeptr _Copy_nodes(_Nodeptr _Rootnode, _Nodeptr _Wherenode,
		_Moveit _Movefl)
	{	// copy entire subtree, recursively
		_Nodeptr _Newroot = this->_Get_data()._Myhead;	// point at nil node

		if (!_Rootnode->_Isnil)
		{	// copy or move a node, then any subtrees
			typename is_same<key_type, value_type>::type _Is_set;
			_Nodeptr _Pnode = _Copy_or_move(_Rootnode->_Myval, _Movefl, _Is_set);
			_Pnode->_Parent = _Wherenode;
			_Pnode->_Color = _Rootnode->_Color;
			if (_Newroot->_Isnil)
				_Newroot = _Pnode;	// memorize new root

			_TRY_BEGIN
				_Pnode->_Left = _Copy_nodes(_Rootnode->_Left, _Pnode, _Movefl);
			_Pnode->_Right = _Copy_nodes(_Rootnode->_Right, _Pnode, _Movefl);
			_CATCH_ALL
				_Erase(_Newroot);	// subtree copy failed, bail out
			_RERAISE;
			_CATCH_END
		}

		return (_Newroot);	// return newly constructed tree
	}

	template<class _Other>
	_Paircc _Eqrange(const _Other& _Keyval) const
	{	// find leftmost node not less than _Keyval
		auto& _My_data = this->_Get_data();
		_Nodeptr _Pnode = _My_data._Root();
		_Nodeptr _Lonode = _My_data._Myhead;	// end() if search fails
		_Nodeptr _Hinode = _My_data._Myhead;	// end() if search fails

		while (!_Pnode->_Isnil)
		{
			if (_DEBUG_LT_PRED(this->_Getcomp(), this->_Key(_Pnode), _Keyval))
			{
				_Pnode = _Pnode->_Right;	// descend right subtree
			}
			else
			{	// _Pnode not less than _Keyval, remember it
				if (_Hinode->_Isnil
					&& _DEBUG_LT_PRED(this->_Getcomp(), _Keyval,
						this->_Key(_Pnode)))
				{
					_Hinode = _Pnode;	// _Pnode greater, remember it
				}

				_Lonode = _Pnode;
				_Pnode = _Pnode->_Left;	// descend left subtree
			}
		}

		_Pnode = _Hinode->_Isnil ? _My_data._Root() : _Hinode->_Left;	// continue scan for upper bound
		while (!_Pnode->_Isnil)
		{
			if (_DEBUG_LT_PRED(this->_Getcomp(), _Keyval, this->_Key(_Pnode)))
			{	// _Pnode greater than _Keyval, remember it
				_Hinode = _Pnode;
				_Pnode = _Pnode->_Left;	// descend left subtree
			}
			else
			{
				_Pnode = _Pnode->_Right;	// descend right subtree
			}
		}

		const_iterator _First = const_iterator(_Lonode, _STD addressof(_My_data));
		const_iterator _Last = const_iterator(_Hinode, _STD addressof(_My_data));
		return (_Paircc(_First, _Last));
	}

	template<class _Other>
	_Pairii _Eqrange(const _Other& _Keyval)
	{	// find leftmost node not less than _Keyval
		_Paircc _Ans(static_cast<const _Tree *>(this)->_Eqrange(_Keyval));
		const auto _My_addr = _STD addressof(this->_Get_data());
		iterator _First = iterator(_Ans.first._Ptr, _My_addr);
		iterator _Last = iterator(_Ans.second._Ptr, _My_addr);
		return (_Pairii(_First, _Last));
	}

	void _Erase(_Nodeptr _Rootnode)
	{	// free entire subtree, recursively
		for (_Nodeptr _Pnode = _Rootnode; !_Pnode->_Isnil; _Rootnode = _Pnode)
		{	// free subtrees, then node
			_Erase(_Pnode->_Right);
			_Pnode = _Pnode->_Left;
			_Alnode& _Al = this->_Getal();
			_Alnode_traits::destroy(_Al, _STD addressof(_Rootnode->_Myval));
			_Node::_Freenode0(_Al, _Rootnode);
		}
	}

	bool _Compare(const key_type& _Left, const key_type& _Right) const
	{	// compare key_type to key_type, with debug checks
		return (_DEBUG_LT_PRED(this->_Getcomp(), _Left, _Right));
	}

	template<class _Ty1,
		class _Ty2>
		bool _Compare(const _Ty1& _Left, const _Ty2& _Right) const
	{	// compare _Ty1 to _Ty2, without debug checks
		return (this->_Getcomp()(_Left, _Right));
	}

	template<class _Other>
	_Nodeptr _Lbound(const _Other& _Keyval) const
	{	// find leftmost node not less than _Keyval
		_Nodeptr _Wherenode = this->_Get_data()._Myhead;	// end() if search fails
		_Nodeptr _Pnode = _Wherenode->_Parent;

		while (!_Pnode->_Isnil)
		{
			if (_Compare(this->_Key(_Pnode), _Keyval))
			{
				_Pnode = _Pnode->_Right;	// descend right subtree
			}
			else
			{	// _Pnode not less than _Keyval, remember it
				_Wherenode = _Pnode;
				_Pnode = _Pnode->_Left;	// descend left subtree
			}
		}

		return (_Wherenode);	// return best remembered candidate
	}

	template<class _Other>
	_Nodeptr _Ubound(const _Other& _Keyval) const
	{	// find leftmost node greater than _Keyval
		auto& _My_data = this->_Get_data();
		_Nodeptr _Pnode = _My_data._Root();
		_Nodeptr _Wherenode = _My_data._Myhead;	// end() if search fails

		while (!_Pnode->_Isnil)
		{
			if (_Compare(_Keyval, this->_Key(_Pnode)))
			{	// _Pnode greater than _Keyval, remember it
				_Wherenode = _Pnode;
				_Pnode = _Pnode->_Left;	// descend left subtree
			}
			else
			{
				_Pnode = _Pnode->_Right;	// descend right subtree
			}
		}

		return (_Wherenode);	// return best remembered candidate
	}

#if _ITERATOR_DEBUG_LEVEL == 2
	void _Orphan_ptr(_Nodeptr _Ptr)
	{	// orphan iterators with specified node pointers
		_Lockit _Lock(_LOCK_DEBUG);
		const_iterator **_Pnext = (const_iterator **)this->_Getpfirst();
		if (_Pnext != nullptr)
		{
			while (*_Pnext != nullptr)
			{
				if ((*_Pnext)->_Ptr == this->_Get_data()._Myhead
					|| (_Ptr != nullptr && (*_Pnext)->_Ptr != _Ptr))
				{
					_Pnext = (const_iterator **)(*_Pnext)->_Getpnext();
				}
				else
				{	// orphan the iterator
					(*_Pnext)->_Clrcont();
					*_Pnext = *(const_iterator **)(*_Pnext)->_Getpnext();
				}
			}
		}
	}
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */

	void _Tidy()
	{	// free all storage
		erase(begin(), end());
	}

	const key_type& _Kfn(const value_type& _Val) const
	{	// get key from value
		return (_Traits::_Kfn(_Val));
	}

	const key_type& _Key(_Nodeptr _Pnode) const
	{	// return reference to key in node
		return (this->_Kfn(_Pnode->_Myval));
	}

#if _HAS_CXX17
public:
	using node_type = typename _Traits::node_type;

	node_type extract(const const_iterator _Where)
	{	// extract the node denoted by _Where
		const auto _Ptr = this->_Get_data()._Extract(_Where);
#if _ITERATOR_DEBUG_LEVEL == 2
		_Orphan_ptr(_Ptr);
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */
		return (node_type::_Make(_Ptr, this->_Getal()));
	}

	node_type extract(const key_type& _Keyval)
	{	// extract the first node whose key matches _Keyval
		const const_iterator _Where = find(_Keyval);
		if (_Where == end())
		{
			return (node_type{});
		}

		return (extract(_Where));
	}

	auto insert(node_type&& _Handle)
	{	// insert the node (if any) held in _Handle
#if 1  /* TRANSITION, if constexpr */
		return (_Insert_node(bool_constant<_Multi>{}, _STD move(_Handle)));
#else  /* TRANSITION, if constexpr */
		if (_Handle.empty())
		{
			if constexpr (_Multi)
			{
				return (end());
			}
			else
			{
				return (_Insert_return_type<iterator, node_type>{end(), false, {}});
			}
		}

		_Check_node_allocator(_Handle);

		const auto _Result = _Insert_nohint(
			false, _Handle._Getptr()->_Myval, _STD addressof(_Handle));
		if constexpr (_Multi)
		{
			return (_Result.first);
		}
		else
		{
			return _Insert_return_type<iterator, node_type>{
				_Result.first, _Result.second, _STD move(_Handle)};
		}
#endif /* TRANSITION, if constexpr */
	}

	iterator insert(const const_iterator _Hint, node_type&& _Handle)
	{	// insert the node held in _Handle (if any), with hint
		if (_Handle.empty())
		{
			return (end());
		}

		_Check_node_allocator(_Handle);
		return (_Insert_hint(_Hint, _Handle._Getptr()->_Myval, _STD addressof(_Handle)));
	}

	template<class>
	friend class _Tree;

	template<class _Other_traits>
	void merge(_Tree<_Other_traits>& _That)
	{	// transfer all nodes from _That into *this
		static_assert(is_same_v<_Nodeptr, typename _Tree<_Other_traits>::_Nodeptr>,
			"merge() requires an argument with a compatible node type.");

		static_assert(is_same_v<allocator_type, typename _Tree<_Other_traits>::allocator_type>,
			"merge() requires an argument with the same allocator type.");

#if 1  /* TRANSITION, if constexpr */
		if (_Check_self(_STD addressof(_That)))
		{
			return;
		}
#else  /* TRANSITION, if constexpr */
		if constexpr (is_same_v<_Tree, _Tree<_Other_traits>>)
		{
			if (this == _STD addressof(_That))
			{
				return;
			}
		}
#endif /* TRANSITION, if constexpr */

#if _ITERATOR_DEBUG_LEVEL == 2
		_STL_VERIFY(this->_Getal() == _That._Getal(), "allocator incompatible for merge");
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */

		_Node_merge_wrapper<_Other_traits> _Wrapper{ _That, {} };
		auto _First = _That.begin();
		const auto _Last = _That.end();
		while (_First != _Last)
		{
			_Wrapper._Where = _First;
			++_First;
			const auto _Result = _Insert_nohint(false, *_Wrapper._Where, _STD addressof(_Wrapper));
			if (_Result.second)
			{	// Reparent iterators for the merged node.
				_Reparent_ptr(_Wrapper._Where._Ptr, _That);
			}
		}
	}

protected:
	_Nodeptr _Buy_if_not_node(node_type * const _Node_handle, _Any_tag)
	{	// Extract node from node handle
		const auto _Ptr = _Node_handle->_Release();
		const auto _Head = this->_Get_data()._Myhead;
		_Ptr->_Left = _Head;
		_Ptr->_Right = _Head;
		return (_Ptr);
	}

	void _Destroy_if_node(node_type *)
	{	// Handle retains ownership of node
	}

	template<class _Other_traits>
	struct _Node_merge_wrapper
	{
		_Tree<_Other_traits>& _That;
		typename _Tree<_Other_traits>::iterator _Where;
	};

	template<class _Other_traits>
	auto _Buy_if_not_node(_Node_merge_wrapper<_Other_traits> * const _Wrapper, _Any_tag)
	{	// transition the denoted node into this container
		const auto _Ptr = _Wrapper->_That._Get_data()._Extract(_Wrapper->_Where);
		const auto _Head = this->_Get_data()._Myhead;
		_Ptr->_Left = _Head;
		_Ptr->_Right = _Head;
		return (_Ptr);
	}

	template<class _Other_traits>
	void _Destroy_if_node(_Node_merge_wrapper<_Other_traits> *)
	{	// source container retains ownership of node
	}

	template<class _Other_traits>
	void _Reparent_ptr(const _Nodeptr _Ptr, _Tree<_Other_traits>& _Old_parent)
	{	// steal iterators with specified node pointer from _Old_parent
		(void)_Ptr; (void)_Old_parent;
#if _ITERATOR_DEBUG_LEVEL == 2
		_Lockit _Lock(_LOCK_DEBUG);
		auto _Pnext = reinterpret_cast<const_iterator **>(_Old_parent._Getpfirst());
		_STL_VERIFY(_Pnext, "source container corrupted");
		if (_Ptr == nullptr || _Ptr == _Old_parent._Get_data()._Myhead)
		{
			return;
		}

		const auto _My_saved_first = this->_Getpfirst();
		const auto _My_saved_proxy = this->_Myproxy();

		while (*_Pnext)
		{
			const auto _Next = reinterpret_cast<const_iterator **>((*_Pnext)->_Getpnext());
			if ((*_Pnext)->_Ptr == _Ptr)
			{	// reparent the iterator
				const_iterator * const _Iter = *_Pnext;
				*_Pnext = *_Next;
				_Iter->_Myproxy = _My_saved_proxy;
				_Iter->_Mynextiter = *_My_saved_first;
				*_My_saved_first = _Iter;
			}
			else
			{	// skip the iterator
				_Pnext = _Next;
			}
		}
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */
	}

	void _Check_node_allocator(node_type& _Handle) const
	{	// ensure that _Handle and *this have compatible allocators
		(void)_Handle;
#if _ITERATOR_DEBUG_LEVEL == 2
		_STL_VERIFY(this->_Getal() == _Handle._Getal(), "node handle allocator incompatible for insert");
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */
	}

#if 1  /* TRANSITION, if constexpr */
private:
	auto _Insert_node(true_type, node_type&& _Handle)
	{	// try to insert _Handle, multi(set|map) case
		if (_Handle.empty())
		{
			return (end());
		}

		_Check_node_allocator(_Handle);

		const auto _Result = _Insert_nohint(
			false, _Handle._Getptr()->_Myval, _STD addressof(_Handle));
		return (_Result.first);
	}

	auto _Insert_node(false_type, node_type&& _Handle)
	{	// try to insert _Handle, (set|map) case
		if (_Handle.empty())
		{
			return (_Insert_return_type<iterator, node_type>{end(), false, {}});
		}

		_Check_node_allocator(_Handle);

		const auto _Result = _Insert_nohint(
			false, _Handle._Getptr()->_Myval, _STD addressof(_Handle));
		return _Insert_return_type<iterator, node_type>{
			_Result.first, _Result.second, _STD move(_Handle)};
	}

	bool _Check_self(const _Tree * const _That) const
	{	// determine if argument points to *this, same-type case
		return (this == _That);
	}
	bool _Check_self(void *) const
	{	// determine if argument points to *this, different-type case
		return (false);
	}
#endif /* TRANSITION, if constexpr */
#endif /* _HAS_CXX17 */
};

template<class _Traits>
_NODISCARD inline bool operator==(const _Tree<_Traits>& _Left, const _Tree<_Traits>& _Right)
{	// test for _Tree equality
	return (_Left.size() == _Right.size()
		&& _STD equal(_Left.begin(), _Left.end(), _Right.begin()));
}

template<class _Traits>
_NODISCARD inline bool operator!=(const _Tree<_Traits>& _Left, const _Tree<_Traits>& _Right)
{	// test for _Tree inequality
	return (!(_Left == _Right));
}

template<class _Traits>
_NODISCARD inline bool operator<(const _Tree<_Traits>& _Left, const _Tree<_Traits>& _Right)
{	// test if _Less < _Right for _Trees
	return (_STD lexicographical_compare(_Left.begin(), _Left.end(),
		_Right.begin(), _Right.end()));
}

template<class _Traits>
_NODISCARD inline bool operator>(const _Tree<_Traits>& _Left, const _Tree<_Traits>& _Right)
{	// test if _Less > _Right for _Trees
	return (_Right < _Left);
}

template<class _Traits>
_NODISCARD inline bool operator<=(const _Tree<_Traits>& _Left, const _Tree<_Traits>& _Right)
{	// test if _Less <= _Right for _Trees
	return (!(_Right < _Left));
}

template<class _Traits>
_NODISCARD inline bool operator>=(const _Tree<_Traits>& _Left, const _Tree<_Traits>& _Right)
{	// test if _Less >= _Right for _Trees
	return (!(_Left < _Right));
}
_STD_END

#pragma pop_macro("new")
_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)
#endif /* RC_INVOKED */
#endif /* _XTREE_ */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0009 */
