#ifndef _SPLIT_BUFFER_H__
#define _SPLIT_BUFFER_H__

#include "Def/stldef.h"

STL_BEGIN

// __split_buffer is used as a swap buffer for the container.
// type Allocator might be reference.
template <class T, class Allocator = allocator<T>>
struct __split_buffer {
 public:
  // typdefs
  typedef T value_type;
  typedef Allocator allocator_type;
  typedef typename std::remove_reference<allocator_type>::type allocator_remove_reference_type_;
  typedef allocator_traits<allocator_remove_reference_type> alloc_traits_;
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

  __split_buffer(cosnt __split_buffer) = delete;

  explicit __split_buffer(const allocator_remove_reference_type_& alloc);

  explicit __split_buffer(size_type cap, size_type start, const allocator_remove_reference_type_& alloc);

  __split_buffer(__split_buffer&& x) noexcept(std::is_nothrow_move_constructible<allocator_type>::value);

  ~__split_buffer();

  __split_buffer& operator=(const __split_buffer&) = delete;

  // iterator
  iterator begin() noexcept { return begin_; }
  const_iterator begin() const noexcept { return begin_; }
  iterator end() noexcept { return end_; }
  const_iterator end() const noexcept { return end_; }

  // size
  size_type size() const noexcept { return static_cast<size_type>(end_ - begin_); }

  bool empty() const noexcept { return end_ == begin_; }
  
  size_type capacity() const noexcept { return static_cast<size_type>(cap_ - storage_); }

  size_type front_spare_() const noexcept { return static_cast<size_type>(begin_ - storage_); }

  size_type back_spare_() const noexcept { return static_cast<size_type>(cap_ - end_); }

  // access
  reference front() { return *begin_; }
  
  const_reference front() const { return *begin_; }

  reference back() { return *(end_ - 1); }

  const_reference back() const { return *(end_ - 1); }

  void clear() noexcept {
    destruct_at_end_(begin_);
  }
  
  void destruct_at_end_(pointer new_begin) {
        
  }
  
  pointer storage_;
  pointer begin_;
  pointer end_;
  pointer cap_;
  allocator_type alloc_;
};

template <class T, class Allocator>
__split_buffer<T, Allocator>::__split_buffer(size_type cap, 
  size_type start, 
  const allocator_remove_reference_type& alloc) 
  : alloc_(alloc) {
  storage_ = cap != 0 ? alloc_traits_::allocate(alloc_, cap) : nullptr;
  begin_ = end_ = storage_ + start;
  cap_ = storage + cap;
}

STL_END

#endif  // !_SPLIT_BUFFER_H__
