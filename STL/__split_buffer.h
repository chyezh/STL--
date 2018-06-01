#ifndef _SPLIT_BUFFER_H__
#define _SPLIT_BUFFER_H__

#include "Def/stldef.h"

STL_BEGIN

template <class T, class Allocator = allocator<T>>
struct __split_buffer {
 public:
  // typdefs
  typedef T value_type;
  typedef Allocator allocator_type;
  typedef allocator_traits<allocator_type> alloc_traits_;
  typedef typename alloc_traits_::reference reference;
  typedef typename alloc_traits_::const_reference const_reference;
  typedef typename alloc_traits_::pointer iterator;
  typedef typename alloc_traits_::const_pointer const_iterator;
  typedef typename alloc_traits_::size_type size_type;
  typedef typename alloc_traits_::difference_type difference_type;
  typedef typename alloc_traits_::pointer pointer;
  typedef typename alloc_traits_::const_pointer const_pointer;
  
  // default constructor.
  __split_buffer() noexcept(std::is_nothrow_default_constructible<allocator_type>::value);

  explicit __split_buffer(const allocator_type& alloc);

  explicit __split_buffer(size_type cap_, size_type start_, const allocator_type& alloc);

  __split_buffer(__split_buffer&& x) noexcept(std::is_nothrow_move_constructible<allocator_type>::value);

  ~__split_buffer();

  // iterator
  iterator begin() noexcept { return begin_; }
  const_iterator begin() const noexcept { return begin_; }
  iterator end() noexcept { return end_; }
  const_iterator end() const noexcept { return end_; }

  void clear() noexcept {
    destruct_at_end_(begin_);
  }
  
  void destruct_at_end_(pointer new_begin) {
    
  }
  
  pointer first_;
  pointer begin_;
  pointer end_;
  pointer cap_;
  allocator_type alloc_;
};

STL_END

#endif  // !_SPLIT_BUFFER_H__
