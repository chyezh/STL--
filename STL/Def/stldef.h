#ifndef _STLDEF_H__
#define _STLDEF_H__

#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <type_traits>

#define STL_BEGIN namespace stl {
#define STL_END }
#define STL_NAME stl


STL_BEGIN

// >>> iterator category tag.
using ::std::bidirectional_iterator_tag;
using ::std::forward_iterator_tag;
using ::std::input_iterator_tag;
using ::std::output_iterator_tag;
using ::std::random_access_iterator_tag;

// >>> type traits
template <class T, T v>
using integral_constant = ::std::integral_constant<T, v>;

using ::std::true_type;

using ::std::false_type;

template <class...>
using void_t = void;

template <bool B, class T>
using enable_if = ::std::enable_if<B, T>;

template <class T>
using pointer_traits = ::std::pointer_traits<T>;

template <class T>
using iterator_traits = ::std::iterator_traits<T>;

template <class T, class U>
using is_convertible = ::std::is_convertible<T, U>;

template <class T, class U>
using is_assignable = ::std::is_assignable<T, U>;

template <class T, class... Args>
using is_constructible = ::std::is_constructible<T, Args...>;

template <class T>
using is_nothrow_default_constructible = ::std::is_nothrow_default_constructible<T>;

template <class T>
using is_nothrow_constructible = ::std::is_nothrow_constructible<T>;

template <class T>
using is_nothrow_move_constructible = ::std::is_nothrow_move_constructible<T>;

template <class T>
using is_nothrow_move_assignable = ::std::is_nothrow_move_assignable<T>;

/* using some utilities of namespace std */
// default allocator
template <class T>
using allocator = ::std::allocator<T>;

template <class Allocator>
using allocator_traits = ::std::allocator_traits<Allocator>;

// swap allocator
template <class Allocator>
void __swap_allocator(Allocator alloc1, Allocator alloc2, true_type) noexcept {
  ::std::swap(alloc1, alloc2);
}

template <class Allocator>
void __swap_allocator(Allocator alloc1, Allocator alloc2, false_type) noexcept {
}

template <class Allocator>
void __swap_allocator(Allocator alloc1, Allocator alloc2) {
  __swap_allocator(
      alloc1, alloc2,
      integral_constant<
          bool,
          allocator_traits<Allocator>::propagate_on_container_swap::value>());
}

// allocator destructor for RAII
template <class Allocator>
class __allocator_destructor {
  typedef allocator_traits<Allocator> alloc_traits_;
 public:
  typedef typename alloc_traits_::pointer pointer;
  typedef typename alloc_traits_::size_type size_type;

  __allocator_destructor(Allocator& a, size_type n) : alloc_(a), size_(n) {}

 void operator()(pointer pos) {
   alloc_traits_::deallocate(alloc_, pos, size_);
 }

 private:
  Allocator& alloc_;
  size_type size_;
};

// smart pointer
template <class T, class Deleter>
using unique_ptr = ::std::unique_ptr<T, Deleter>;

// change like pointer type to raw pointer
template <class T>
inline T *__to_raw_pointer(T *pointer) noexcept {
  return pointer;
}

template <class Pointer>
inline typename pointer_traits<Pointer>::element_type *__to_raw_pointer(
    Pointer pointer) noexcept {
  return __to_raw_pointer(pointer.operator->());
}

/* iterator category judgement */
// whether type T has type member iterator_category
template <class T, class = void_t<>>
struct __has_iterator_category_tag : false_type {};

template <class T>
struct __has_iterator_category_tag<T, void_t<typename T::iterator_category>>
    : true_type {};

// whether type T is convertible to type U
template <class T, class U,
          bool = __has_iterator_category_tag<iterator_traits<T>>::value>
struct __has_iterator_category_convertible_to
    : public integral_constant<
          bool, is_convertible<typename iterator_traits<T>::iterator_category,
                               U>::value> {};

template <class T, class U>
struct __has_iterator_category_convertible_to<T, U, false> : public false_type {
};

// judge T's iterator_category.
template <class T>
struct __is_input_iterator
    : public __has_iterator_category_convertible_to<T, input_iterator_tag> {};

template <class T>
struct __is_output_iterator
    : public __has_iterator_category_convertible_to<T, output_iterator_tag> {};

template <class T>
struct __is_forward_iterator
    : public __has_iterator_category_convertible_to<T, forward_iterator_tag> {};

template <class T>
struct __is_bidirectional_iterator
    : public __has_iterator_category_convertible_to<
          T, bidirectional_iterator_tag> {};

template <class T>
struct __is_random_access_iterator
    : public __has_iterator_category_convertible_to<
          T, random_access_iterator_tag> {};

template <class T1, class T2>
using pair = ::std::pair<T1, T2>;

template <class T>
using initializer_list = ::std::initializer_list<T>;

STL_END

#endif  // !_STLDEF_H__
