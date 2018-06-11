#ifndef _STL_VECTOR__
#define _STL_VECTOR__

#include <initializer_list>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include "Def/stldef.h"
#include "__split_buffer.h"

STL_BEGIN

// vector base type for data section
template <class T, class Allocator>
class __vector_base {
 protected:
  // >>> member type
  typedef T value_type;
  typedef Allocator allocator_type;
  typedef allocator_traits<allocator_type> alloc_traits_;
  typedef value_type &reference;
  typedef const value_type &const_reference;
  typedef typename alloc_traits_::pointer iterator;
  typedef typename alloc_traits_::const_pointer const_iterator;
  typedef typename alloc_traits_::size_type size_type;
  typedef typename alloc_traits_::difference_type difference_type;
  typedef typename alloc_traits_::pointer pointer;
  typedef typename alloc_traits_::const_pointer const_pointer;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

 protected:
  // >>> constructor
  // default constructor
  __vector_base() noexcept(
      std::is_nothrow_default_constructible<allocator_type>::value)
      : begin_(nullptr),
        end_(nullptr),
        cap_(nullptr),
        alloc_(allocator_type()) {}

  // constructor with given allocator
  __vector_base(const allocator_type &alloc) noexcept
      : begin_(nullptr), end_(nullptr), cap_(nullptr), alloc_(alloc) {}

  // >>> destructor.
  ~__vector_base() {
    // free the memory using allocator
    if (begin_ != nullptr) {
      clear();
      alloc_traits_::deallocate(alloc_, begin_, capacity());
    }
  }

  // destroy all elements.
  void clear() noexcept { destroy_at_end_(begin_); }

  // acquire the capacity of container
  size_type capacity() const noexcept {
    return static_cast<size_type>(cap_ - begin_);
  }

  // >>> auxiliary function for derived type
  // destroy every element from end to loc
  // noexcept because of allocator::destroy is noexcept
  void destroy_at_end_(pointer loc) noexcept {
    pointer end_now = end_;
    while (loc != end_now)
      alloc_traits_::destroy(alloc_, __to_raw_pointer(--end_now));
    end_ = loc;
  }

  // copy assignment of allocator
  void copy_assign_alloc_(const __vector_base &x) {
    copy_assign_alloc_(
        x, std::integral_constant<
               bool,
               alloc_traits_::propagate_on_container_copy_assignment::value>());
  }

  // move assignment for allocator
  void move_assign_alloc_(const __vector_base &x) noexcept(
      !alloc_traits_::propagate_on_container_move_assignment::value ||
      std::is_nothrow_move_assignable<allocator_type>::value) {
    move_assign_alloc_(
        x, integral_constant<
               bool,
               alloc_traits_::propagate_on_container_move_assignment::value>());
  }

 private:
  // >>> private auxiliary function
  // propagate_on_container_copy_assignment
  // allocator need to be copied when container is copy-assigned
  void copy_assign_alloc_(const __vector_base &x, true_type) {
    // deallocate when allocators do not compare equal.
    if (alloc_ != x.alloc_) {
      clear();
      alloc_traits_::deallocate(alloc_, begin_, capacity());
      begin_ = end_ = cap_ = nullptr;
    }
    alloc_ = x.alloc_;
  }

  // copy-trival for allocator.
  void copy_assign_alloc_(const __vector_base &x, false_type) {}

  // propagate_on_container_move_assignment
  // allocator need to be moved when container is move-assigned
  // noexcept if allocator is nothrow move assignable
  void move_assign_alloc_(const __vector_base &x, true_type) noexcept(
      std::is_nothrow_move_assignable<allocator_type>::value) {
    clear();
    alloc_ = std::move(x.alloc_);
  }

  // move-trival for allocator. noexcept
  void move_assign_alloc_(const __vector_base &x, false_type) noexcept {
    clear();
  }

 protected:
  // >>> data member
  allocator_type alloc_;
  pointer begin_;
  pointer end_;
  pointer cap_;
};

template <class T, class Allocator = allocator<T>>
class vector : private __vector_base<T, Allocator> {
 private:
  typedef __vector_base<T, Allocator> base_;
  typedef typename base_::alloc_traits_ alloc_traits_;

 public:
  // >>> member type
  typedef typename base_::value_type value_type;
  typedef typename base_::allocator_type allocator_type;
  typedef typename base_::reference reference;
  typedef typename base_::const_reference const_reference;
  typedef typename base_::pointer iterator;
  typedef typename base_::const_pointer const_iterator;
  typedef typename base_::size_type size_type;
  typedef typename base_::difference_type difference_type;
  typedef typename base_::pointer pointer;
  typedef typename base_::const_pointer const_pointer;
  typedef typename base_::reverse_iterator reverse_iterator;
  typedef typename base_::const_reverse_iterator const_reverse_iterator;

 public:
  // >>> constructor
  // default constructor
  vector() noexcept(
      std::is_nothrow_default_constructible<allocator_type>::value) {}

  // constructor with given allocator
  explicit vector(const allocator_type &alloc) noexcept : base_(alloc) {}

  // constructor with given size or value
  explicit vector(size_type n);

  explicit vector(size_type n, const allocator_type &alloc);

  vector(size_type n, const value_type &value);

  vector(size_type n, const value_type &value, const allocator_type &alloc);

  // constructor with given range
  // if range iterator is input iterator
  template <class InputIterator>
  vector(InputIterator first,
         typename enable_if<
             __is_input_iterator<InputIterator>::value &&
                 !__is_forward_iterator<InputIterator>::value &&
                 is_constructible<
                     value_type,
                     typename iterator_traits<InputIterator>::reference>::value,
             InputIterator>::type last);

  template <class InputIterator>
  vector(InputIterator first,
         typename enable_if<
             __is_input_iterator<InputIterator>::value &&
                 !__is_forward_iterator<InputIterator>::value &&
                 is_constructible<
                     value_type,
                     typename iterator_traits<InputIterator>::reference>::value,
             InputIterator>::type last,
         const allocator_type &alloc);

  // if range iterator is forward iterator
  template <class ForwardIterator>
  vector(
      ForwardIterator first,
      typename enable_if<
          __is_forward_iterator<ForwardIterator>::value &&
              is_constructible<
                  value_type,
                  typename iterator_traits<ForwardIterator>::reference>::value,
          ForwardIterator>::type last);

  template <class ForwardIterator>
  vector(
      ForwardIterator first,
      typename enable_if<
          __is_forward_iterator<ForwardIterator>::value &&
              is_constructible<
                  value_type,
                  typename iterator_traits<ForwardIterator>::reference>::value,
          ForwardIterator>::type last,
      const allocator_type &alloc);

  // copy constructor
  vector(const vector &x);

  vector(const vector &x, const allocator_type &alloc);

  // move constructor
  // noexcept if allocator is nothrow move constructible
  vector(vector &&x) noexcept(
      std::is_nothrow_move_constructible<allocator_type>::value);

  vector(vector &&x, const allocator_type &alloc);

  // constructor with initial list
  vector(std::initializer_list<value_type> init);

  vector(std::initializer_list<value_type> init, const allocator_type &alloc);

  // >>> deconstructor
  ~vector() {}

  // >>> assignment operator
  // operator=
  vector &operator=(const vector &x);
  vector &operator=(vector &&x) noexcept(
      alloc_traits_::propagate_on_container_move_assignment::value ||
      alloc_traits_::is_always_equal::value);
  vector &operator=(std::initializer_list<value_type> init) {
    assign(init.begin(), init.end());
  }

  // assign
  void assign(size_type n, const value_type &value);

  template <class InputIterator>
  void assign(
      InputIterator first,
      typename enableif<
          __is_input_iterator<InputIterator>::value &&
              !__is_forward_iterator<InputIterator>::value &&
              is_constructible<
                  value_type,
                  typename iterator_traits<InputIterator>::reference>::value,
          InputIterator>::value_type last);

  template <class ForwardIterator>
  void assign(
      ForwardIterator first,
      typename enable_if<
          __is_forward_iterator<ForwardIterator>::value &&
              is_constructible<
                  value_type,
                  typename iterator_traits<ForwardIterator>::reference>::value,
          ForwardIterator>::value_type last);

  void assign(std::initializer_list<value_type> init) {
    assign(init.begin(), init.end());
  }

  // >>> allocator
  allocator_type get_allocator() const noexcept { return this->alloc_; }

  // >>> iterator
  // plain iterator
  iterator begin() noexcept { return this->begin_; }

  const_iterator begin() const noexcept { return this->begin_; }

  iterator end() noexcept { return this->end_; }

  const_iterator end() const noexcept { return this->end_; }

  // reverse iterator
  reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }

  const_reverse_iterator rbegin() const noexcept {
    return const_reverse_iterator(end());
  }

  reverse_iterator rend() noexcept { return reverse_iterator(begin()); }

  const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator(begin());
  }

  // const iterator
  const_iterator cbegin() const noexcept { return begin(); }

  const_iterator cend() const noexcept { return end(); }

  const_reverse_iterator crbegin() const noexcept { return rbegin(); }

  const_reverse_iterator crend() const noexcept { return rend(); }

  // >>> capacity
  size_type size() const noexcept {
    return static_cast<size_type>(this->end_ - this->begin_);
  }

  size_type max_size() const noexcept {
    return alloc_traits_::max_size(this->alloc_);
  }

  bool empty() const noexcept { return this->begin_ == this->end_; }

  void resize(size_type n);

  void resize(size_type n, const value_type &value);

  size_type capacity() const noexcept { return base_::capacity(); }

  void reserve(size_type n);

  void shrink_to_fit() noexcept;

  // >>> element access
  reference operator[](size_type n);

  const_reference operator[](size_type n) const;

  reference at(size_type n);

  const_reference at(size_type n) const;

  reference front() {
    assert(!empty(), "vector: front() called for empty vector");
    return *this->begin_;
  }

  const_reference front() const {
    assert(!empty(), "vector: front() called for empty vector");
    return *this->begin_;
  }

  reference back() {
    assert(!empty(), "vector: back() called for empty vector");
    return *(this->end_ - 1);
  }

  const_reference back() const {
    assert(!empty(), "vector: back() called for empty vector");
    return *(this->end_ - 1);
  }

  value_type *data() noexcept { return __to_raw_pointer(this->begin_); }

  const value_type *data() const noexcept {
    return __to_raw_pointer(this->begin_);
  }

  // >>> modifier
  // insert
  void push_back(const value_type &value);

  void push_back(value_type &&value);

  iterator insert(const_iterator position, const value_type &value);
  iterator insert(const_iterator position, value_type &&value);
  iterator insert(const_iterator position, size_type n,
                  const value_type &value);
  template <class InputIterator>
  iterator insert(const_iterator position, InputIterator first,
                  InputIterator last);
  iterator insert(const_iterator position,
                  std::initializer_list<value_type> init);

  // emplace
  template <class... Args>
  reference emplace_back(Args &&... args);

  template <class... Args>
  iterator emplace(const_iterator position, Args &&... args);

  // remove
  iterator erase(const_iterator position);
  iterator erase(const_iterator first, const_iterator last);
  void pop_back();

  void clear() noexcept { base_::clear(); }

  void swap(vector &) noexcept(
      alloc_traits_::propagate_on_container_swap::value ||
      alloc_traits_::is_always_equal::value);

 private:
  // >>> private auxiliary function
  // throw length error
  void throw_length_error_() { throw std::length_error("vector"); }

  // throw out of range error
  void throw_out_of_range_() { throw std::out_of_range("vector"); }

  // allocate n elements
  void allocate_(size_type n);

  // destroy all elements and deallocate the space
  void deallocate_() noexcept;

  /* construct at end */
  // default construct n elements at end
  void default_construct_at_end_(size_type n);

  // copy construct n elements at end with value
  void copy_construct_at_end_(size_type n, const_reference value);

  // copy construct n elements at end with element in the range
  template <class ForwardIterator>
  typename enable_if<__is_forward_iterator<ForwardIterator>::value, void>::type
  copy_construct_at_end_(ForwardIterator first, ForwardIterator last);

  // default append n element, may reallocate new space
  void default_append_(size_type n);

  // copy append n element with value, may reallocate new space
  void copy_append_(size_type n, const_reference value);

  // move range with bound check
  void move_range_(pointer src_first, pointer src_last, pointer dst_first);

  // move-assignment when alloc_traits_::propagate_on_container_move_assignment
  // is true
  void move_assign_(vector &x, true_type) noexcept(
      std::is_nothrow_move_assignable<allocator_type>::value);

  // move-assignment when alloc_traits_::propagate_on_container_move_assignment
  // is false
  void move_assign_(vector &x, false_type);

  // construct at end when capacity() == size().
  template <class... Args>
  void emplace_back_when_capacity_is_full_(Args &&... args);

  // construct at end with constructor argument arg
  template <class Arg>
  void emplace_back_with_single_value_(Arg &&arg) {
    // perfect forward
    emplace_back(std::forward<Arg>(arg));
  }

  // move if noexcept or copy old element to swap buffer and then swap the
  // buffer
  void swap_out_buffer_(
      __split_buffer<value_type, allocator_type &> &swap_buffer);

  // slice old element into two part by loc
  // move if noexcept or copy old element to swap buffer and then swap the
  // buffer
  pointer swap_out_buffer_(
      __split_buffer<value_type, allocator_type &> &swap_buffer, pointer loc);

  size_type default_realloc_strategy_(size_type new_size) const;
};

// >>> private auxiliary function

// allocates n sapce for element
// throws length error if n > max_size()
// throws bad_alloc if memory run out
// precondition: capacity() == 0; n > 0
// postconditon: capacity() = n; size() = 0
template <class T, class Allocator>
void vector<T, Allocator>::allocate_(size_type n) {
  if (n > max_size()) this->throw_length_error_();
  this->begin_ = this->end_ = alloc_traits_::allocate(this->alloc_, n);
  this->cap_ = this->begin_ + n;
}

// deallocates sapce. noexcept
// precondition: capacity() != 0
// postcondition： capacity() == 0
template <class T, class Allocator>
void vector<T, Allocator>::deallocate_() noexcept {
  if (this->begin_ != nullptr) {
    // destroy all elements.
    clear();
    alloc_traits_::deallocate(this->alloc_, this->begin_, capacity());
    this->begin_ = this->end_ = this->cap_ = nullptr;
  }
}

// default constructs n element at end()
// throws if default constructor of element throws
// precondition: n > 0; size() + n <= capacity()
// postcondition: size() == size() + n
template <class T, class Allocator>
void vector<T, Allocator>::default_construct_at_end_(size_type n) {
  do {
    // default constructor may throw.
    alloc_traits_::construct(this->alloc_, __to_raw_pointer(this->end_));
    // updates end_ after construct success.
    ++this->end_;
  } while (--n > 0);
}

// copy constructs n element at end() with value
// throws if copy constructor of element throws
// preconditon: n > 0; size() + n <= capacity()
// postcondition: size() == size() + n
template <class T, class Allocator>
void vector<T, Allocator>::copy_construct_at_end_(size_type n,
                                                  const_reference value) {
  do {
    // copy constructor may throw.
    alloc_traits_::construct(this->alloc_, __to_raw_pointer(this->end_), value);
    // updates end_ after construct success.
    ++this->end_;
  } while (--n > 0);
}

// copy constructs elements at end() with value of range first to last
// throws if copy constructor of element throws
// iterator must satisfy ForwardIterator
// precondition: (d = distance(first, last)) > 0; size() + d <= capacity()
// postcondition:  size() == size() + d
template <class T, class Allocator>
template <class ForwardIterator>
typename enable_if<__is_forward_iterator<ForwardIterator>::value, void>::type
vector<T, Allocator>::copy_construct_at_end_(ForwardIterator first,
                                             ForwardIterator last) {
  do {
    // copy constructor may throw.
    alloc_traits_::construct(this->alloc_, __to_raw_pointer(this->end_),
                             *first);
    // updates end_ after construct success.
    ++this->end_;
  } while (++first != last);
}

// default append n element, may reallocate new space
template <class T, class Allocator>
void vector<T, Allocator>::default_append_(size_type n) {
  if (n >= static_cast<size_type>(capacity() - size_type())) {
    __split_buffer<value_type, allocator_type &> swap_buffer(
        default_realloc_strategy_(capacity() + 1), size(), this->alloc_);
    swap_out_buffer_(swap_buffer);
  }
  default_construct_at_end_(n);
}

// copy append n element with value, may reallocate new space
template <class T, class Allocator>
void vector<T, Allocator>::copy_append_(size_type n, const_reference value) {
  if (n >= static_cast<size_type>(capacity() - size_type())) {
    __split_buffer<value_type, allocator_type &> swap_buffer(
        default_realloc_strategy_(capacity() + 1), size(), this->alloc_);
    swap_out_buffer_(swap_buffer);
  }
  copy_construct_at_end_(n, value);
}

// move range [src_first, src_last) to the [dst_first,)
// precondition: (this->cap_ - dst_first) > (src_last - src_first);
// src_first < dst_first && dst_first <= this->end_
// weak exception guarantee
template <class T, class Allocator>
void vector<T, Allocator>::move_range_(pointer src_first, pointer src_last,
                                       pointer dst_first) {
  pointer old_end = this->end_;
  difference_type n = old_end - dst_first;
  // move construct the additional element
  for (pointer p = src_first + n; p < src_last; ++p, ++this->end_)
    alloc_traits_::construct(this->alloc_, __to_raw_pointer(this->end_),
                             std::move(p));
  // move the rest element
  std::move_backward(src_first, src_first + n, old_end);
}

// move-assignment when alloc_traits_::propagate_on_container_move_assignment is
// true
template <class T, class Allocator>
void vector<T, Allocator>::move_assign_(vector &x, true_type) noexcept(
    std::is_nothrow_move_assignable<allocator_type>::value) {
  base_::move_assign_alloc_(x);
  this->begin_ = x.begin_;
  this->end_ = x.end_;
  this->cap_ = x.cap_;
  x.begin_ = x.end_ = x.cap_ = nullptr;
}

// move-assignment when alloc_traits_::propagate_on_container_move_assignment is
// false
template <class T, class Allocator>
void vector<T, Allocator>::move_assign_(vector &x, false_type) noexcept(
    alloc_traits_::is_always_equal::value) {
  if (this->alloc_ != x.alloc_)
    assign(std::move_iterator<iterator>(x.begin_),
           std::move_iterator<iterator>(x.end_));
  else
    move_assign_(x, true_type());
}

// a emplace path when capacity is full
template <class T, class Allocator>
template <class... Args>
void vector<T, Allocator>::emplace_back_when_capacity_is_full_(
    Args &&... args) {
  // make a swap buffer to allocate new space
  __split_buffer<value_type, allocator_type &> swap_buffer(
      default_realloc_strategy_(size() + 1), size(), this->alloc_);
  // emplace new element at swap_buffer end()
  alloc_traits_::construct(this->alloc_, __to_raw_pointer(swap_buffer.end_),
                           std::forward<Args>(args)...);
  ++swap_buffer.end_;
  // move old element to the swap buffer and swap
  swap_out_buffer_(swap_buffer);
}

// move if noexcept or copy old element to swap buffer and then swap the buffer
template <class T, class Allocator>
void vector<T, Allocator>::swap_out_buffer_(
    __split_buffer<value_type, allocator_type &> &swap_buffer) {
  while (this->end_ != this->begin_) {
    alloc_traits_::construct(this->alloc_, __to_raw_pointer(swap_buffer.begin_ - 1), std::move_if_noexcept(*(this->end_ - 1));
    --this->end_;
    --swap_buffer.begin_;
  }
  swap(this->begin_, swap_buffer.begin_);
  swap(this->end_, swap_buffer.end_);
  swap(this->cap_, swap_buffer.cap_);
  swap_buffer.storage_ = swap_buffer.begin_;
}

// slice old element into two part by loc
// move if noexcept or copy old element to swap buffer and then swap the buffer
template <class T, class Allocator>
void vector<T, Allocator>::swap_out_buffer_(
    __split_buffer<value_type, allocator_type &> &swap_buffer, pointer loc) {
  pointer temp_loc = loc;
  while (temp_loc != this->begin_) {
    alloc_traits_::construct(this->alloc_,
                             __to_raw_pointer(swap_buffer.begin_ - 1),
                             std::move_if_noexcept(*(temp_loc - 1)));
    --temp_loc;
    --swap_buffer.begin_;
  }
  while (loc != this->end_) {
    alloc_traits_::construct(this->alloc_, __to_raw_pointer(swap_buffer.end_),
                             std::move_if_noexcept(*loc));
    ++loc;
    ++swap_buffer.end_;
  }
  swap(this->begin_, swap_buffer.begin_);
  swap(this->end_, swap_buffer.end_);
  swap(this->cap_, swap_buffer.cap_);
  swap_buffer.storage_ = swap_buffer.begin_;
}

template <class T, class Allocator>
typename vector<T, Allocator>::size_type
vector<T, Allocator>::default_realloc_strategy_(size_type new_size) const {
  const size_type ms = max_size();
  if (new_size > max_size()) this->throw_length_error_();
  const size_type cap_now = capacity();
  if (cap_now >= ms / 2) return ms;
  return std::max<size_type>(2 * cap_now, new_size);
}

// >>> vector constructor

template <class T, class Allocator>
vector<T, Allocator>::vector(size_type n) {
  if (n > 0) {
    allocate_(n);
    default_construct_at_end_(n);
  }
}

template <class T, class Allocator>
vector<T, Allocator>::vector(size_type n, const allocator_type &alloc)
    : base_(alloc) {
  if (n > 0) {
    allocate_(n);
    default_construct_at_end_(n);
  }
}

template <class T, class Allocator>
vector<T, Allocator>::vector(size_type n, const value_type &value) {
  if (n > 0) {
    allocate_(n);
    copy_construct_at_end_(n, value);
  }
}

template <class T, class Allocator>
vector<T, Allocator>::vector(size_type n, const value_type &value,
                             const allocator_type &alloc)
    : base_(alloc) {
  if (n > 0) {
    allocate_(n);
    copy_construct_at_end_(n, value);
  }
}

// constructor with given range
// if range iterator is input iterator
template <class T, class Allocator>
template <class InputIterator>
vector<T, Allocator>::vector(
    InputIterator first,
    typename enable_if<
        __is_input_iterator<InputIterator>::value &&
            !__is_forward_iterator<InputIterator>::value &&
            is_constructible<value_type, typename iterator_traits<
                                             InputIterator>::reference>::value,
        InputIterator>::type last) {
  while (first != last) emplace_back_with_single_value_(*first++);
}

template <class T, class Allocator>
template <class InputIterator>
vector<T, Allocator>::vector(
    InputIterator first,
    typename enable_if<
        __is_input_iterator<InputIterator>::value &&
            !__is_forward_iterator<InputIterator>::value &&
            is_constructible<value_type, typename iterator_traits<
                                             InputIterator>::reference>::value,
        InputIterator>::type last,
    const allocator_type &alloc)
    : base_(alloc) {
  while (first != last) emplace_back_with_single_value_(*first++);
}

// if range iterator is forward iterator
template <class T, class Allocator>
template <class ForwardIterator>
vector<T, Allocator>::vector(
    ForwardIterator first,
    typename enable_if<
        __is_forward_iterator<ForwardIterator>::value &&
            is_constructible<
                value_type,
                typename iterator_traits<ForwardIterator>::reference>::value,
        ForwardIterator>::type last) {
  // gets the element number for allocating enough space
  size_type new_size = static_cast<size_type>(std::distance(last, first));
  if (new_size > 0) {
    allocate_(new_size);
    copy_construct_at_end_(first, last);
  }
}

template <class T, class Allocator>
template <class ForwardIterator>
vector<T, Allocator>::vector(
    ForwardIterator first,
    typename enable_if<
        __is_forward_iterator<ForwardIterator>::value &&
            is_constructible<
                value_type,
                typename iterator_traits<ForwardIterator>::reference>::value,
        ForwardIterator>::type last,
    const allocator_type &alloc)
    : base_(alloc) {
  // gets the element number for allocating enough space
  size_type new_size = static_cast<size_type>(std::distance(first, last));
  if (new_size > 0) {
    allocate_(new_size);
    copy_construct_at_end_(first, last);
  }
}

template <class T, class Allocator>
vector<T, Allocator>::vector(const vector &x)
    : base_(alloc_traits_::select_on_container_copy_constrcution(x.alloc_)) {
  size_type new_size = x.size();
  if (new_size > 0) {
    allocate_(new_size);
    copy_construct_at_end_(x.begin(), x.end());
  }
}

template <class T, class Allocator>
vector<T, Allocator>::vector(const vector &x, const allocator_type &alloc)
    : base_(alloc) {
  size_type new_size = x.size();
  if (new_size > 0) {
    allocate_(new_size);
    copy_construct_at_end_(x.begin(), x.end());
  }
}

template <class T, class Allocator>
vector<T, Allocator>::vector(vector &&x) noexcept(
    std::is_nothrow_move_constructible<allocator_type>::value)
    : base_(std::move(x.alloc_)) {
  this->begin_ = x.begin_;
  this->end_ = x.end_;
  this->cap_ = x.cap_;
  x.begin_ = x.end_ = x.cap_ = nullptr;
}

template <class T, class Allocator>
vector<T, Allocator>::vector(vector &&x, const allocator_type &alloc)
    : base_(alloc) {
  if (alloc == x.alloc_) {
    this->begin_ = x.begin_;
    this->end_ = x.end_;
    this->cap_ = x.cap_;
  } else
    assign(std::move_iterator<iterator>(x.begin()),
           std::move_iterator<iterator>(x.end()));
}

template <class T, class Allocator>
vector<T, Allocator>::vector(std::initializer_list<value_type> init) {
  size_type new_size = init.size();
  if (new_size > 0) {
    allocate_(new_size);
    copy_construct_at_end_(init.begin(), init.end());
  }
}

template <class T, class Allocator>
vector<T, Allocator>::vector(std::initializer_list<value_type> init,
                             const allocator_type &alloc)
    : base_(alloc) {
  size_type new_size = init.size();
  if (new_size > 0) {
    allocate_(new_size);
    copy_construct_at_end_(init.begin(), init.end());
  }
}

// >>> assignment operator

template <class T, class Allocator>
vector<T, Allocator> &vector<T, Allocator>::operator=(const vector &x) {
  // self assignment check
  if (this != &x) {
    base::copy_assign_alloc_(x.alloc_);
    assgin(x.begin_, x.end_));
  }
  return *this;
}

// do no self assignment check
template <class T, class Allocator>
vector<T, Allocator> &vector<T, Allocator>::operator=(vector &&x) noexcept(
    alloc_traits_::propagate_on_container_move_assignment::value ||
    alloc_traits_::is_always_equal::value) {
  move_assign_(
      x, integral_constant<
             bool,
             alloc_traits_::propagate_on_container_move_assignment::value>());
  return *this;
}

template <class T, class Allocator>
void vector<T, Allocator>::assign(size_type n, const value_type &value) {
  if (n <= capacity()) {
    size_type old_size = size();
    // do assignment on the used space
    std::fill_n(this->begin_, std::min(old_size, n), value);
    if (n > old_size)
      // do constrution of rest element
      copy_construct_at_end_(n - old_size, value);
    else
      // destroy excess element
      destroy_at_end_(this->begin_ + n);
  } else {
    // realloc when capacity is too small
    deallocate_();
    allocate_(default_realloc_strategy_(n));
    copy_construct_at_end_(n, value);
  }
}

template <class T, class Allocator>
template <class InputIterator>
void vector<T, Allocator>::assign(
    InputIterator first,
    typename enableif<
        __is_input_iterator<InputIterator>::value &&
            !__is_forward_iterator<InputIterator>::value &&
            is_constructible<value_type, typename iterator_traits<
                                             InputIterator>::reference>::value,
        InputIterator>::value_type last) {
  clear();
  while (first != last) emplace_back_with_single_value_(*first++);
}

template <class T, class Allocator>
template <class ForwardIterator>
void vector<T, Allocator>::assign(
    ForwardIterator first,
    typename enable_if<
        __is_forward_iterator<ForwardIterator>::value &&
            is_constructible<
                value_type,
                typename iterator_traits<ForwardIterator>::reference>::value,
        ForwardIterator>::value_type last) {
  size_type new_size = static_cast<size_type>(std::distance(frist, last));
  if (new_size <= capacity()) {
    size_type old_size = size();
    iterator beg = begin();
    iterator end = beg + std::min(new_size, old_size);
    while (beg != end) *beg++ = *first++;
    if (new_size > old_size)
      copy_construct_at_end_(first, last);
    else
      destroy_at_end_(this->begin_ + new_size);
  } else {
    deallocate_();
    allocate_(default_realloc_strategy_(new_size));
    copy_construct_at_end_(first, last);
  }
}

// >>> capacity operation

template <class T, class Allocator>
void vector<T, Allocator>::resize(size_type n) {
  if (n > size())
    default_append_(n - size());
  else
    destroy_at_end_(this->begin_ + n);
}

template <class T, class Allocator>
void vector<T, Allocator>::resize(size_type n, const value_type &value) {
  if (n > size())
    copy_append_(n - size(), value);
  else
    destroy_at_end_(this->begin_ + n);
}

template <class T, class Allocator>
void vector<T, Allocator>::reserve(size_type n) {
  if (n > capacity()) {
    __split_buffer<value_type, allocator_type &> swap_buffer(n, size(),
                                                             this->alloc_);
    swap_out_buffer_(swap_buffer);
  }
}

template <class T, class Allocator>
void vector<T, Allocator>::shrink_to_fit() noexcept {
  if (capacity() > size()) {
    // noexcept
    try {
      __split_buffer<value_type, allocator_type &> swap_buffer(size(), size(),
                                                               this->alloc_);
      swap_out_buffer_(swap_buffer);
    } catch (...) {
    }
  }
}

// >>> access operation

// access element of subscript n without bound check
template <class T, class Allocator>
typename vector<T, Allocator>::reference vector<T, Allocator>::operator[](
    size_type n) {
  assert(n < size(), "vector: operator[] index out of bounds");
  return this->begin_[n];
}

template <class T, class Allocator>
typename vector<T, Allocator>::const_reference vector<T, Allocator>::operator[](
    size_type n) const {
  assert(n < size(), "vector: operator[] index out of bounds");
  return this->begin_[n];
}

// access element of subscript n without bound check
template <class T, class Allocator>
typename vector<T, Allocator>::reference vector<T, Allocator>::at(size_type n) {
  if (n >= size()) this->throw_out_of_range_();
  return this->begin_[n];
}

template <class T, class Allocator>
typename vector<T, Allocator>::const_reference vector<T, Allcator>::at(
    size_type n) const {
  if (n >= size()) this->throw_out_of_range_();
  return this->begin_[n];
}

// >>> insert operation

template <class T, class Allocator>
void vector<T, Allocator>::push_back(const value_type &value) {
  if (this->end_ < this->cap_) {
    alloc_traits_::construct(this->alloc_, __to_raw_pointer(this->end_), value);
    ++this->end_;
  } else
    emplace_back_when_capacity_is_full_(value);
}

template <class T, class Allocator>
void vector<T, Allocator>::push_back(value_type &&value) {
  if (this->end_ < this->cap_) {
    alloc_traits_::construct(this->alloc_, __to_raw_pointer(this->end_),
                             std::move(value));
    ++this->end_
  } else
    emplace_back_when_capacity_is_full_(std::move(value));
}

template <class T, class Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(
    const_iterator position, const value_type &value) {}

template <class T, class Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(
    const_iterator position, value_type &&value) {}

template <class T, class Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(
    const_iterator position, size_type n, const value_type &value) {}
template <class T, class Allocator>
template <class InputIterator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(
    const_iterator position, InputIterator first, InputIterator last) {}

template <class T, class Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(
    const_iterator position, std::initializer_list<value_type> init) {}

template <class T, class Allocator>
template <class... Args>
inline typename vector<T, Allocator>::reference
vector<T, Allocator>::emplace_back(Args &&... args) {
  if (this->end_ < this->cap_) {
    alloc_traits_::construct(this->alloc_, __to_raw_pointer(this->end_),
                             std::forward(args...));
    ++this->end_;
  } else
    emplace_back_when_capacity_is_full_(std::forward<Args>(args)...);
  return this->back();
}

template <class T, class Allocator>
template <class... Args>
typename vector<T, Allocator>::iterator vector<T, Allocator>::emplace(
    const_iterator position, Args &&... args) {}

// remove

template <class T, class Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::erase(
    const_iterator position) {}

template <class T, class Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::erase(
    const_iterator first, const_iterator last) {}

template <class T, class Allocator>
void vector<T, Allocator>::pop_back() {}

template <class T, class Allocator>
void vector<T, Allocator>::swap(vector &) noexcept(
    alloc_traits_::propagate_on_container_swap::value ||
    alloc_traits_::is_always_equal::value) {}

STL_END

#endif  // !STL_VECTOR__
