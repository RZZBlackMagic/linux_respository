// new standard header
#pragma once
#ifndef _NEW_
#define _NEW_
#ifndef RC_INVOKED
#include <exception>
#include <vcruntime_new.h>

#pragma pack(push,_CRT_PACKING)
#pragma warning(push,_STL_WARNING_LEVEL)
#pragma warning(disable: _STL_DISABLED_WARNINGS)
_STL_DISABLE_CLANG_WARNINGS
#pragma push_macro("new")
#undef new

_STD_BEGIN
#if _HAS_CXX17
// FUNCTION TEMPLATE launder
template<class _Ty>
_NODISCARD constexpr _Ty * launder(_Ty * _Ptr) noexcept
{
	static_assert(!is_function_v<_Ty> && !is_void_v<_Ty>,
		"N4727 21.6.4 [ptr.launder]/3: The program is ill-formed if T is a function type or cv void.");
#if _HAS_LAUNDER
	return (__builtin_launder(_Ptr));
#else /* _HAS_LAUNDER */
	return (_Ptr);
#endif /* _HAS_LAUNDER */
}

#if defined(_M_IX86) || defined(_M_X64) || defined(_M_ARM) || defined(_M_ARM64)
_INLINE_VAR constexpr size_t hardware_constructive_interference_size = 64;
_INLINE_VAR constexpr size_t hardware_destructive_interference_size = 64;
#else /* ^^^ supported hardware ^^^ // vvv unsupported hardware vvv */
#error Unsupported architecture
#endif /* hardware */

#endif /* _HAS_CXX17 */

// SUPPORT TYPES
#if !defined(_INC_NEW) || !defined(_MSC_EXTENSIONS)
// handler for operator new failures
typedef void (__CLRCALL_PURE_OR_CDECL * new_handler) ();
#endif /* !defined(_INC_NEW) || !defined(_MSC_EXTENSIONS) */

// FUNCTION AND OBJECT DECLARATIONS
_CRTIMP2 new_handler __cdecl set_new_handler(_In_opt_ new_handler) noexcept;
_NODISCARD _CRTIMP2 new_handler __cdecl get_new_handler() noexcept;
_STD_END

#pragma pop_macro("new")
_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)

#endif /* RC_INVOKED */
#endif /* _NEW_ */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0009 */
