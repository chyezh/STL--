#ifndef _SPLIT_BUFFER_H__
#define _SPLIT_BUFFER_H__

#include "Def/stldef.h"

STL_BEGIN

// __split_buffer is used as a swap buffer for the container
// type Allocator might be reference
template <class T, class Allocator = allocator<T>>
struct __split_buffer {
 public:
  // typdefs
  typedef T value_type;
  typedef Allocator allocator_type;
  typedef typename std::remove_reference<allocator_type>::type
      allocator_remove_reference_type_;
  typedef allocator_traits<allocator_remove_reference_type_> alloc_traits_;
  typedef value_type &reference;
  typedef const value_type &const_reference;
  typedef typename alloc_traits_::pointer iterator;
  typedef typename alloc_traits_::const_pointer const_iterator;
  typedef typename alloc_traits_::size_type size_type;
  typedef typename alloc_traits_::difference_type difference_type;
  typedef typename alloc_traits_::pointer pointer;
  typedef typename alloc_traits_::const_pointer const_pointer;

  // >>> constructor
  // default constructor
  __split_buffer() noexcept(
      std::is_nothrow_default_constructible<allocator_type>::value);

  // copy constructor disabled
  __split_buffer(const __split_buffer &) = delete;

  explicit __split_buffer(allocator_remove_reference_type_ &alloc);

  explicit __split_buffer(size_type cap, size_type start,
                          allocator_remove_reference_type_ &alloc);

  __split_buffer(__split_buffer &&x) noexcept(
      std::is_nothrow_move_constructible<allocator_type>::value);

  __split_buffer(__split_buffer &&x, allocator_remove_reference_type_ &alloc);

  // >>> destructor
  ~__split_buffer();

  // >>> assignment
  // copy assignment disabled
  __split_buffer &operator=(const __split_buffer &) = delete;

  __split_buffer operator=(__split_buffer &&x);

  // >>> iterator
  iterator begin() noexcept { return begin_; }

  const_iterator begin() const noexcept { return begin_; }

  iterator end() noexcept { return end_; }

  const_iterator end() const noexcept { return end_; }

  // >>> capacity
  size_type size() const noexcept {
    return static_cast<size_type>(end_ - begin_);
  }

  bool empty() const noexcept { return end_ == begin_; }

  size_type capacity() const noexcept {
    return static_cast<size_type>(cap_ - storage_);
  }

  size_type front_spare_() const noexcept {
    return static_cast<size_type>(begin_ - storage_);
  }

  size_type back_spare_() const noexcept {
    return static_cast<size_type>(cap_ - end_);
  }

  void reserve(size_type n);

  void shrink_to_fit() noexcept;

  // >>> access
  reference front() { return *begin_; }

  const_reference front() const { return *begin_; }

  reference back() { return *(end_ - 1); }

  const_reference back() const { return *(end_ - 1); }

  // >>> modifier
  void push_front(const value_type &value);

  void push_front(value_type &&value);

  void push_back(const value_type &value);

  void push_back(value_type &&value);

  template <class... Args>
  void emplace_back(Args... args);

  void pop_front() { destruct_at_begin_(begin_ + 1); }

  void pop_back() { destruct_at_end_(end_ - 1); }

  void clear() noexcept { destruct_at_end_(begin_); }

  void swap(__split_buffer &x) noexcept(!alloc_traits_::propagate_on_container_swap::value
                                        /* || std::is_nothrow_swappable<allocator_remove_reference_type_>::value */);

  // >>> auxiliary function

  // construct element at end without capacity check
  void default_construct_at_end_(size_type n);

  void copy_construct_at_end_(size_type n, const value_type &value);

  template <class ForwardIterator>
  typename enable_if<__is_forward_iterator<ForwardIterator>::value, void>::type
  copy_construct_at_end_(ForwardIterator first, ForwardIterator last);

  // destruct element at
  void destruct_at_begin_(pointer new_begin) noexcept;

  void destruct_at_end_(pointer new_end) noexcept;

 private:
  void move_assign_alloc_(__split_buffer &x, true_type) noexcept(
      std::is_nothrow_move_assignable<
          allocator_remove_reference_type_>::value) {
    alloc_ = std::move(x.alloc_);
  }

  void move_assign_alloc_(__split_buffer &x, false_type) noexcept {}

 public:
  pointer storage_;
  pointer begin_;
  pointer end_;
  pointer cap_;
  allocator_type alloc_;
};

// default constructs n elements at end()
// throws if constructor of element throws
// precondition: n > 0; n <= back_spare_()
// postcondition: size() == size() + n
template <class T, class Allocator>
void __split_buffer<T, Allocator>::default_construct_at_end_(size_type n) {
  do {
    // default constructor may throw
    alloc_traits_::construct(this->alloc_, __to_raw_pointer(this->end_));
    // if construct success, add end_
    ++this->end_;
  } while (--n > 0);
}

// copy constructs n element at end() with value
// throws if constructor of element throws
// precondition: n > 0; n <= back_spare_()
// postcondition: size() == size() + n
template <class T, class Allocator>
void __split_buffer<T, Allocator>::copy_construct_at_end_(
    size_type n, const value_type &value) {
  do {
    // copy constructor may throw
    alloc_traits_::construct(this->alloc_, __to_raw_pointer(this->end_), value);
    // if construct success, add end_
    ++this->end_;
  } while (--n > 0);
}

// copy constructs elements at end() with value of range first to last
// throws if copy constructor of element throws
// iterator must satisfy ForwardIterator
// precondition: (d = distance(first, last)) > 0; d <= back_spare_()
// postcondition:  size() == size() + d
template <class T, class Allocator>
template <class ForwardIterator>
typename enable_if<__is_forward_iterator<ForwardIterator>::value, void>::type
__split_buffer<T, Allocator>::copy_construct_at_end_(ForwardIterator first,
                                                     ForwardIterator last) {
  do {
    // copy constructor may throw
    alloc_traits_::construct(this->alloc_, __to_raw_pointer(this->end_),
                             *first);
    // if construct success, add end_
    ++this->end_;
  } while (++first != last);
}

// destruct from begin() to new_begin
// precondition: distance(begin(), new_begin) >= 0
// postcondition: begin() == new_begin
template <class T, class Allocator>
void __split_buffer<T, Allocator>::destruct_at_begin_(
    pointer new_begin) noexcept {
  while (this->begin_ != new_begin)
    alloc_traits_::destroy(this->alloc_, this->begin_++);
}

// destruct from end() - 1 to new_end - 1
// precondition: distance(new_end, end()) >= 0
// postcondition: end() == new_end
template <class T, class Allocator>
void __split_buffer<T, Allocator>::destruct_at_end_(pointer new_end) noexcept {
  while (this->end_ != new_end)
    alloc_traits_::destroy(this->alloc_, --this->end_);
}

// default constructor
// compile error if Allocator is reference type
template <class T, class Allocator>
__split_buffer<T, Allocator>::__split_buffer() noexcept(
    std::is_nothrow_default_constructible<allocator_type>::value)
    : storage_(nullptr),
      begin_(nullptr),
      end_(nullptr),
      cap_(nullptr),
      alloc_(allocator_type()) {}

template <class T, class Allocator>
__split_buffer<T, Allocator>::__split_buffer(
    size_type cap, size_type start, allocator_remove_reference_type_ &alloc)
    : alloc_(alloc) {
  storage_ = cap != 0 ? alloc_traits_::allocate(alloc_, cap) : nullptr;
  begin_ = end_ = storage_ + start;
  cap_ = storage_ + cap;
}

template <class T, class Allocator>
__split_buffer<T, Allocator>::__split_buffer(
    allocator_remove_reference_type_ &alloc)
    : storage_(nullptr),
      begin_(nullptr),
      end_(nullptr),
      cap_(nullptr),
      alloc_(alloc) {}

template <class T, class Allocator>
__split_buffer<T, Allocator>::__split_buffer(__split_buffer &&x) noexcept(
    std::is_nothrow_move_constructible<allocator_type>::value)
    : storage_(std::move(x.storage_)),
      begin_(std::move(x.begin_)),
      end_(std::move(x.end_)),
      cap_(std::move(x.cap_)),
      alloc_(std::move(x.alloc_)) {
  x.storage_ = x.begin_ = x.end_ = x.cap_ = nullptr;
}

template <class T, class Allocator>
__split_buffer<T, Allocator>::__split_buffer(
    __split_buffer &&x, allocator_remove_reference_type_ &alloc)
    : storage_(nullptr),
      begin_(nullptr),
      end_(nullptr),
      cap_(nullptr),
      alloc_(alloc) {
  if (x.alloc == alloc_) {
    // handover content block pointer if x.alloc == alloc_
    this->storage_ = x.storage_;
    this->begin_ = x.begin_;
    this->end_ = x.end_;
    this->cap_ = x.cap_;
    x.storage_ = x.begin_ = x.end_ = x.cap_ = nullptr;
  } else {
    // move every element if x.alloc != alloc_
    size_type new_capacity = x.capacity();
    // allocate new space and set pointer
    this->storage_ = alloc_traits_::allocate(this->alloc_, new_capacity);
    this->cap_ = this->storage_ + new_capacity;
    this->begin_ = this->end_ = this->storage_ + x.front_spare_();
    copy_construct_at_end_(std::move_iterator<iterator>(x.begin_),
                           std::move_iterator<iterator>(x.end_));
  }
}

template <class T, class Allocator>
__split_buffer<T, Allocator>::~__split_buffer() {
  clear();
  if (this->storage_ != nullptr)
    alloc_traits_::deallocate(this->alloc_, this->storage_, capacity());
}

template <class T, class Allocator>
void __split_buffer<T, Allocator>::swap(__split_buffer &x) noexcept(!alloc_traits_::propagate_on_container_swap::value
                                                                    /* || std::is_nothrow_swappable<allocator_remove_reference_type_>::value */)
{
  std::swap(this->storage_, x.storage_);
  std::swap(this->begin_, x.begin_);
  std::swap(this->end_, x.end_);
  std::swap(this->cap_, x.cap_);
  __swap_allocator(this->alloc_, x.alloc_);
}

template <class T, class Allocator>
void __split_buffer<T, Allocator>::reserve(size_type n) {
  // allocate new space if n > capacity(), do nothing else
  if (n > capacity()) {
    __split_buffer<value_type, allocator_remove_reference_type_ &> temp_buffer(
        n, front_spare_(), this->alloc_);
    iterator first = this->begin_;
    iterator last = this->end_;
    while (first != last)
      temp_buffer.copy_construct_at_end_(1, std::move_if_noexcept(*first++));
    swap(temp_buffer);
  }
}

template <class T, class Allocator>
void __split_buffer<T, Allocator>::shrink_to_fit() noexcept {
  if (capacity() > size()) {
    // noexcept
    try {
      __split_buffer<value_type, allocator_remove_reference_type_ &>
          temp_buffer(size(), 0, this->alloc_);
      iterator first = this->begin_;
      iterator last = this->end_;
      while (first != last)
        temp_buffer.copy_construct_at_end_(1, std::move_if_noexcept(*first++));
      swap(temp_buffer);
    } catch (...) {
    }
  }
}

template <class T, class Allocator>
void __split_buffer<T, Allocator>::push_front(const value_type &x) {
  if (this->begin_ == this->storage_) {
    if (this->end_ > this->cap_) {
      // adjust buffer if back_spare_() != 0
      difference_type move_step = back_spare_();
      // at lease move 1 element
      move_step = (move_step + 1) / 2;
      this->begin_ = std::move_backward(this->begin_, this->end_, this->end_ + move_step);
      this->end_ += move_step;
    } else {
      size_type new_cap = std::max<size_type>(2 * static_cast<size_type>(capacity()),1);
      // front_sparse() of new temp at least 1
      __split_buffer<value_type, allocator_remove_reference_type_ &> temp_buffer(new_cap, (new_cap + 3) / 4, this->alloc_);
      iterator first = this->begin_;
      iterator last = this->end_;
      while(first != last)
        temp_buffer.copy_construct_at_end_(1, std::move_if_noexcept(*first++));
      swap(temp_buffer);
    }
  }
  // push new element
  alloc_traits_::construct(this->alloc_, __to_raw_pointer(this->begin_ - 1), x);
  --this->begin_;
}

template <class T, class Allocator>
void __split_buffer<T, Allocator>::push_front(value_type &&x) {
  if (this->begin_ == this->storage_) {
    if (this->end_ > this->cap_) {
      // adjust buffer if back_spare_() != 0
      difference_type move_step = back_spare_();
      // at lease move 1 element
      move_step = (move_step + 1) / 2;
      this->begin_ = std::move_backward(this->begin_, this->end_, this->end_ + move_step);
      this->end_ += move_step;
    } else {
      size_type new_cap = std::max<size_type>(2 * static_cast<size_type>(capacity()),1);
      // front_sparse() of new temp at least 1
      __split_buffer<value_type, allocator_remove_reference_type_ &> temp_buffer(new_cap, (new_cap + 3) / 4, this->alloc_);
      iterator first = this->begin_;
      iterator last = this->end_;
      while(first != last)
        temp_buffer.copy_construct_at_end_(1, std::move_if_noexcept(*first++));
      swap(temp_buffer);
    }
  }
  // push new element
  alloc_traits_::construct(this->alloc_, __to_raw_pointer(this->begin_ - 1), std::move(x));
  --this->begin_;
}

template <class T, class Allocator>
void __split_buffer<T, Allocator>::push_back(const value_type &x) {
  
}

template <class T, class Allocator>
void __split_buffer<T, Allocator>::push_back(value_type &&x) {

}

STL_END

#endif  // !_SPLIT_BUFFER_H__
