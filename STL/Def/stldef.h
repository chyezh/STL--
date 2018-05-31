#ifndef _STLDEF_H__
#define _STLDEF_H__

#include <memory>
#include <type_traits>

#define STL_BEGIN namespace stl {
#define STL_END }

STL_BEGIN

// default allocator
template <class T>
using allocator = std::allocator<T>;

// change pointer type to raw pointer.
template <class T>
inline T* __to_raw_pointer(T* pointer) noexcept { return pointer; }

template <class Pointer>
inline typename std::pointer_traits<Pointer>::element_type* __to_raw_pointer(Pointer pointer) noexcept {
    return __to_raw_pointer(pointer.operator->());
}

STL_END

#endif // !DEF_H__
