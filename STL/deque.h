#ifndef _DEQUE_H__
#define _DEQUE_H__

#include "Def/stldef.h"
#include "__split_buffer.h"

STL_BEGIN

template <class T, class VoidPtr>
class __deque_const_iterator;
template <class T, class Allocator>
class __deque_base;
template <class T, class Allocator>
class deque;

template <class T, class DifferenceType>
struct __deque_block_size {
  // from libcxx
  static const DifferenceType value = sizeof(T) < 256 ? 4096 / sizeof(T) : 16;
};

// deque iterator type
template <class T, class VoidPtr>
class __deque_iterator {
  // >>> member types
 public:
  // type_traits
  typedef random_access_iterator_tag iterator_category;
  typedef T value_type;
  typedef value_type &reference;
  typedef typename pointer_traits<VoidPtr>::template rebind<value_type> pointer;
  typedef typename pointer_traits<pointer>::difference_type difference_type;

 private:
  typedef typename pointer_traits<VoidPtr>::template rebind<pointer>
      map_iterator_;
  typedef __deque_block_size<T, difference_type> deque_block_size_;

 private:
  // friend class
  template <class, class>
  friend class __deque_const_iterator;
  template <class, class>
  friend class __deque_base;
  template <class, class>
  friend class deque;

 public:
  // constructor
  __deque_iterator() : map_iter_(nullptr), ptr_(nullptr) {}

  __deque_iterator(const __deque_iterator &iter)
      : map_iter_(iter.map_iter_), ptr_(iter.ptr_) {}

 private:
  // used for container
  __deque_iterator(const map_iterator_ &m, const pointer &p)
      : map_iter_(m), ptr_(p) {}

 public:
  // iterator interface
  reference operator*() const { return *ptr_; }

  pointer operator->() const { return ptr_; }

  __deque_iterator &operator++() {
    if (++ptr_ - *map_iter_ == block_size_()) {
      ++map_iter_;
      ptr_ = *map_iter_;
    }
    return *this;
  }

  __deque_iterator operator++(int) {
    __deque_iterator old_iter(*this);
    ++(*this);
    return old_iter;
  }

  // bidirectional iterator interface
  __deque_iterator &operator--() {
    if (ptr_ == *map_iter_) {
      --map_iter_;
      ptr_ = *map_iter_ + block_size_();
    }
    --ptr_;
    return *this;
  }

  __deque_iterator operator--(int) {
    __deque_iterator old_iter(*this);
    --(*this);
    return old_iter;
  }

  // random-access iterator interface
  __deque_iterator &operator+=(difference_type step) {
    if (step != 0) {
      // calculate total offset
      step += ptr_ - *map_iter_;
      if (step > 0) {
        // if offset > 0, increse map_iter
        map_iter_ += step / block_size_();
        ptr_ = *map_iter_ + step % block_size_();
      } else {
        // if offset < 0, decrease map_iter
        step = block_size_() - 1 - step;
        map_iter_ -= step / block_size_();
        ptr_ = *map_iter_ + (block_size_() - 1 - step % block_size_());
      }
    }
    return *this;
  }

  __deque_iterator &operator-=(difference_type step) {
    return *(this += -step);
  }

  __deque_iterator operator+(difference_type step) {
    __deque_iterator iter(*this);
    iter += step;
    return iter;
  }

  friend __deque_iterator operator+(difference_type step,
                                    __deque_iterator iter) {
    return iter + step;
  }

  __deque_iterator operator-(difference_type step) {
    __deque_iterator iter(*this);
    iter -= step;
    return iter;
  }

  friend difference_type operator-(const __deque_iterator &lhs,
                                   const __deque_iterator &rhs) {
    return block_size_() * (lhs.map_iter_ - rhs.map_iter_) +
           (lhs.ptr_ - *lhs.map_iter_) - (rhs.ptr_ - *rhs.map_iter_);
  }

  reference operator[](difference_type n) const { return *(*this + n); }

  // iterator comparation
  friend bool operator==(const __deque_iterator &lhs,
                         const __deque_iterator &rhs) {
    return lhs.map_iter_ == rhs.map_iter_ && lhs.ptr_ == rhs.ptr_;
  }

  friend bool operator!=(const __deque_iterator &lhs,
                         const __deque_iterator &rhs) {
    return !(lhs == rhs);
  }

  friend bool operator<(const __deque_iterator &lhs,
                        const __deque_iterator &rhs) {
    return (lhs.map_iter_ < rhs.map_iter_) ||
           (lhs.map_iter_ == rhs.map_iter_ && lhs.ptr_ < rhs.ptr_);
  }

  friend bool operator>(const __deque_iterator &lhs,
                        const __deque_iterator &rhs) {
    return rhs < lhs;
  }

  friend bool operator<=(const __deque_iterator &lhs,
                         const __deque_iterator &rhs) {
    return !(rhs < lhs);
  }

  friend bool operator>=(const __deque_iterator &lhs,
                         const __deque_iterator &rhs) {
    return !(lhs < rhs);
  }

  // >>> data member
 private:
  static inline difference_type block_size_() {
    return deque_block_size_::value;
  }

  map_iterator_ map_iter_;
  pointer ptr_;
};

// deque const iterator type
template <class T, class VoidPtr>
class __deque_const_iterator {
  // >>> member types
 public:
  // type_traits
  typedef random_access_iterator_tag iterator_category;
  typedef T value_type;
  typedef const value_type &reference;
  typedef typename pointer_traits<VoidPtr>::template rebind<const value_type>
      pointer;
  typedef typename pointer_traits<pointer>::difference_type difference_type;

 private:
  typedef typename pointer_traits<VoidPtr>::template rebind<pointer>
      map_iterator_;
  typedef __deque_block_size<T, difference_type> deque_block_size_;

 private:
  // friend class
  template <class, class>
  friend class __deque_base;
  template <class, class>
  friend class deque;

 public:
  // constructor
  __deque_const_iterator() : map_iter_(nullptr), ptr_(nullptr) {}

  __deque_const_iterator(const __deque_const_iterator &iter)
      : map_iter_(iter.map_iter_), ptr_(iter.ptr_) {}

  // convert from __deque_iterator
  __deque_const_iterator(const __deque_iterator<T, VoidPtr> &iter)
      : map_iter_(iter.map_iter_), ptr_(iter.ptr_) {}

 private:
  // used for container
  __deque_const_iterator(const map_iterator_ &m, const pointer &p)
      : map_iter_(m), ptr_(p) {}

 public:
  // iterator interface
  reference operator*() const { return *ptr_; }

  pointer operator->() const { return ptr_; }

  __deque_const_iterator &operator++() {
    if (++ptr_ - *map_iter_ == block_size_()) {
      ++map_iter_;
      ptr_ = *map_iter_;
    }
    return *this;
  }

  __deque_const_iterator operator++(int) {
    __deque_const_iterator old_iter(*this);
    ++(*this);
    return old_iter;
  }

  // bidirectional iterator interface
  __deque_const_iterator &operator--() {
    if (ptr_ == *map_iter_) {
      --map_iter_;
      ptr_ = *map_iter_ + block_size_();
    }
    --ptr_;
    return *this;
  }

  __deque_const_iterator operator--(int) {
    __deque_const_iterator old_iter(*this);
    --(*this);
    return old_iter;
  }

  // random-access iterator interface
  __deque_const_iterator &operator+=(difference_type step) {
    if (step != 0) {
      // calculate total offset
      step += ptr_ - *map_iter_;
      if (step > 0) {
        // if offset > 0, increse map_iter
        map_iter_ += step / block_size_();
        ptr_ = *map_iter_ + step % block_size_();
      } else {
        // if offset < 0, decrease map_iter
        step = block_size_() - 1 - step;
        map_iter_ -= step / block_size_();
        ptr_ = *map_iter_ + (block_size_() - 1 - step % block_size_());
      }
    }
    return *this;
  }

  __deque_const_iterator &operator-=(difference_type step) {
    return *(this += -step);
  }

  __deque_const_iterator operator+(difference_type step) {
    __deque_const_iterator iter(*this);
    iter += step;
    return iter;
  }

  friend __deque_const_iterator operator+(difference_type step,
                                          __deque_const_iterator iter) {
    return iter + step;
  }

  __deque_const_iterator operator-(difference_type step) {
    __deque_const_iterator iter(*this);
    iter -= step;
    return iter;
  }

  friend difference_type operator-(const __deque_const_iterator &lhs,
                                   const __deque_const_iterator &rhs) {
    return block_size_() * (lhs.map_iter_ - rhs.map_iter_) +
           (lhs.ptr_ - *lhs.map_iter_) - (rhs.ptr_ - *rhs.map_iter_);
  }

  reference operator[](difference_type n) const { return *(*this + n); }

  // iterator comparation
  friend bool operator==(const __deque_const_iterator &lhs,
                         const __deque_const_iterator &rhs) {
    return lhs.map_iter_ == rhs.map_iter_ && lhs.ptr_ == rhs.ptr_;
  }

  friend bool operator!=(const __deque_const_iterator &lhs,
                         const __deque_const_iterator &rhs) {
    return !(lhs == rhs);
  }

  friend bool operator<(const __deque_const_iterator &lhs,
                        const __deque_const_iterator &rhs) {
    return (lhs.map_iter_ < rhs.map_iter_) ||
           (lhs.map_iter_ == rhs.map_iter_ && lhs.ptr_ < rhs.ptr_);
  }

  friend bool operator>(const __deque_const_iterator &lhs,
                        const __deque_const_iterator &rhs) {
    return rhs < lhs;
  }

  friend bool operator<=(const __deque_const_iterator &lhs,
                         const __deque_const_iterator &rhs) {
    return !(rhs < lhs);
  }

  friend bool operator>=(const __deque_const_iterator &lhs,
                         const __deque_const_iterator &rhs) {
    return !(lhs < rhs);
  }

  // >>> data member
 private:
  static inline difference_type block_size_() {
    return deque_block_size_::value;
  }

  map_iterator_ map_iter_;
  pointer ptr_;
};

template <class T, class Allocator>
class __deque_base {
 protected:
  // >>> member type
  typedef T value_type;
  typedef Allocator allocator_type;
  typedef allocator_traits<allocator_type> alloc_traits_;
  typedef value_type &reference;
  typedef const value_type &const_reference;

  typedef typename alloc_traits_::void_pointer void_pointer_;
  typedef __deque_iterator<value_type, void_pointer_> iterator;
  typedef __deque_const_iterator<value_type, void_pointer_> const_iterator;

  typedef typename alloc_traits_::size_type size_type;
  typedef typename alloc_traits_::difference_type difference_type;
  typedef typename alloc_traits_::pointer pointer;
  typedef typename alloc_traits_::const_pointer const_pointer;

  typedef typename alloc_traits_::template rebind<pointer>
      pointer_allocator_type;
  typedef allocator_traits<pointer_allocator_type> pointer_alloc_traits_;

  typedef __split_buffer<pointer, pointer_allocator_type> map_type_;
  typedef typename map_type_::iterator map_iter_;
  typedef __deque_block_size<T, difference_type> deque_block_size_;

 protected:
  // >>> constructor
  // delete copy operation
  __deque_base(const __deque_base &) = delete;
  __deque_base &operator=(const __deque_base &) = delete;

  // default constructor
  __deque_base() noexcept(
      is_nothrow_default_constructible<allocator_type>::value);

  // constructor with given alloc
  explicit __deque_base(const allocator_type &alloc);

  // move constructor
  __deque_base(__deque_base &&) noexcept(
      is_nothrow_move_constructible<allocator_type>::value);

  __deque_base(__deque_base &&, const allocator_type &);

  // >>> destructor
  ~__deque_base();

  // swap
  void swap(__deque_base &) noexcept;

  // >>> access
  iterator begin() noexcept;

  const_iterator begin() const noexcept { return begin(); }

  iterator end() noexcept;

  const_iterator end() const noexcept { return end(); }

  // >>> capacity
  size_type size() const noexcept { return size_; }

  // >>> modifier
  void clear() noexcept;

  void move_assign_alloc_(__deque_base &x) noexcept(
      !alloc_traits_::propagate_on_container_move_assignment::value ||
      ::std::is_nothrow_move_assignable<allocator_type>::value) {
    move_assign_alloc_(
        x, integral_constant<
               bool,
               alloc_traits_::propagate_on_container_move_assignment::value>{});
  }

 private:
  // propagate_on_container_move_assignment
  // allocator need to be moved when container is move-assigned
  // noexcept if allocator is nothrow move assignable
  void move_assign_alloc_(__deque_base &x, true_type) noexcept(
      ::std::is_nothrow_move_assignable<allocator_type>::value) {
    clear();
    alloc_ = ::std::move(x.alloc_);
  }

  // move-trival for allocator noexcept
  void move_assign_alloc_(__deque_base &x, false_type) noexcept {}

 protected:
  // get block_size of this type
  difference_type block_size_() { return deque_block_size_::size; }

  // data member
  map_type_ map_;
  size_type start_;
  size_type size_;
  allocator_type alloc_;
};

// constructor
template <class T, class Allocator>
__deque_base<T, Allocator>::__deque_base() noexcept(
    is_nothrow_default_constructible<allocator_type>::value)
    : start_(0), size_(0) {}

template <class T, class Allocator>
__deque_base<T, Allocator>::__deque_base(const allocator_type &alloc)
    : map_(pointer_allocator_type(alloc)), start_(0), size_(0), alloc_(alloc) {}

template <class T, class Allocator>
__deque_base<T, Allocator>::__deque_base(__deque_base &&x) noexcept(
    is_nothrow_move_constructible<allocator_type>::value)
    : map_(::std::move(x.map_)),
      start_(::std::move(x.start_)),
      size_(::std::move(x.size_)),
      alloc_(::std::move(x.alloc_)) {
  x.start_ = 0;
  x.size_ = 0;
}

template <class T, class Allocator>
__deque_base<T, Allocator>::__deque_base(__deque_base &&x,
                                         const allocator_type &alloc)
    : map_(::std::move(x.map_), pointer_allocator_type(alloc)),
      start_(::std::move(x.start_)),
      size_(::std::move(x.size_)),
      alloc_(::std::move(x.alloc_)) {
  if (alloc == x.alloc_) {
    x.start_ = 0;
    x.size_ = 0;
  } else {
    map_.clear();
    start_ = 0;
    size_ = 0;
  }
}

// destructor
template <class T, class Allocator>
__deque_base<T, Allocator>::~__deque_base() {
  clear();
  typename map_type_::iterator first = map_.begin();
  typename map_type_::iterator last = map_.end();
  for (; first != last; ++first)
    alloc_traits_::deallocate(alloc_, *first, block_size_());
}

// swap
template <class T, class Allocator>
void __deque_base<T, Allocator>::swap(__deque_base &x) noexcept {
  map_.swap(x.map_);
  ::std::swap(start_, x.start_);
  ::std::swap(size_, x.size_);
  __swap_allocator(alloc_, x.alloc_);
}

template <class T, class Allocator>
typename __deque_base<T, Allocator>::iterator
__deque_base<T, Allocator>::begin() noexcept {
  typename map_type_::iterator mp = map_.begin() + start_ / block_size_();
  return iterator(mp, map_.empty() ? nullptr : *mp + start_ % block_size_());
}

template <class T, class Allocator>
typename __deque_base<T, Allocator>::iterator
__deque_base<T, Allocator>::end() noexcept {
  size_type end_offset = start_ + size_;
  typename map_type_::iterator mp = map_.begin() + end_offset / block_size_();
  return iterator(mp,
                  map_.empty() ? nullptr : *mp + end_offset % block_size_());
}

template <class T, class Allocator>
void __deque_base<T, Allocator>::clear() noexcept {
  // destroy all element
  for (iterator first = begin(), last = end(); first != last; ++first)
    alloc_traits_::destroy(alloc_, first.ptr_);
  size_ = 0;
  // release additional block
  for (; map_.size() > 1;) {
    alloc_traits_::deallocate(alloc_, map_.front(), block_size_());
    map_.pop_front();
  }
  // set start_ at half of block if map_ is not empty.
  start_ = map_.empty() ? 0 : block_size_() / 2;
}

// deque class
template <class T, class Allocator = allocator<T>>
class deque : private __deque_base<T, Allocator> {
 private:
  typedef __deque_base<T, Allocator> base_;
  typedef typename base_::alloc_traits_ alloc_traits_;
  typedef typename base_::pointer_allocator_type pointer_allocator_type;
  typedef typename base_::pointer_alloc_traits_ pointer_alloc_traits_;
  typedef typename base_::map_type_ map_type_;
  typedef typename base_::deque_block_size_ deque_block_size_;
  typedef typename base_::map_iter_ map_iter_;

 public:
  // >>> member types
  typedef typename base_::value_type value_type;
  typedef typename base_::allocator_type allocator_type;
  typedef typename base_::reference reference;
  typedef typename base_::const_reference const_reference;
  typedef typename base_::iterator iterator;
  typedef typename base_::const_iterator const_iterator;
  typedef typename base_::size_type size_type;
  typedef typename base_::difference_type difference_type;
  typedef typename base_::pointer pointer;
  typedef typename base_::const_pointer const_pointer;
  typedef ::std::reverse_iterator<iterator> reverse_iterator;
  typedef ::std::reverse_iterator<const_iterator> const_reverse_iterator;

 private:
  // type for RAII to keep exception safe after new block is allocated
  typedef __allocator_destructor<allocator_type> block_destructor_;
  typedef unique_ptr<pointer, block_destructor_> block_hold_pointer_;

 public:
  // >>> constructor
  deque() noexcept(is_nothrow_default_constructible<allocator_type>::value) {}

  // constructor with given allocator
  explicit deque(const allocator_type &alloc) : base_(alloc) {}

  // constructor with given size and value
  explicit deque(size_type n);

  explicit deque(size_type n, const allocator_type &alloc);

  deque(size_type n, const value_type &val);

  deque(size_type n, const value_type &val, const allocator_type &alloc);

  // constructor with given range
  // if const iterator is input iterator
  template <class InputIterator>
  deque(InputIterator first,
        typename enable_if<
            __is_input_iterator<InputIterator>::value &&
                !__is_forward_iterator<InputIterator>::value &&
                is_constructible<
                    value_type,
                    typename iterator_traits<InputIterator>::reference>::value,
            InputIterator>::type last);

  template <class InputIterator>
  deque(InputIterator first,
        typename enable_if<
            __is_input_iterator<InputIterator>::value &&
                !__is_forward_iterator<InputIterator>::value &&
                is_constructible<
                    value_type,
                    typename iterator_traits<InputIterator>::reference>::value,
            InputIterator>::type last,
        const allocator_type &alloc);

  template <class ForwardIterator>
  deque(
      ForwardIterator first,
      typename enable_if<
          __is_forward_iterator<ForwardIterator>::value &&
              is_constructible<
                  value_type,
                  typename iterator_traits<ForwardIterator>::reference>::value,
          ForwardIterator>::type last);

  template <class ForwardIterator>
  deque(
      ForwardIterator first,
      typename enable_if<
          __is_forward_iterator<ForwardIterator>::value &&
              is_constructible<
                  value_type,
                  typename iterator_traits<ForwardIterator>::reference>::value,
          ForwardIterator>::type last,
      const allocator_type &alloc);

  deque(const deque &x);

  deque(deque &&x) noexcept(
      is_nothrow_move_constructible<allocator_type>::value);

  deque(const deque &x, const allocator_type &alloc);

  deque(deque &&x, const allocator_type &alloc);

  deque(initializer_list<value_type> init);

  deque(initializer_list<value_type> init, const allocator_type &alloc);

  // >>> destructor
  ~deque() {}

  // >>> assignment operation
  deque &operator=(const deque &x);

  deque &operator=(deque &&x) noexcept(
      allocator_type::propagate_on_container_move_assignment::value
          &&is_nothrow_move_assignable<allocator_type>::value);

  deque &operator=(initializer_list<value_type> init) {
    assign(init.begin(), init.end());
  }

  void assign(size_type n, const value_type &val);

  // assign with given range
  template <class InputIterator>
  void assign(
      InputIterator first,
      typename enable_if<
          __is_input_iterator<InputIterator>::value &&
              !__is_forward_iterator<InputIterator>::value &&
              is_constructible<
                  value_type,
                  typename iterator_traits<InputIterator>::reference>::value,
          InputIterator>::type last);

  template <class ForwardIterator>
  void assign(
      ForwardIterator first,
      typename enable_if<
          __is_forward_iterator<ForwardIterator>::value &&
              is_constructible<
                  value_type,
                  typename iterator_traits<ForwardIterator>::reference>::value,
          ForwardIterator>::type last);

  void assign(initializer_list<value_type> init) {
    assign(init.begin(), init.end());
  }

  // >>> allocator
  allocator_type get_allocator() const noexcept { return this->alloc_; }

  // >>> iterator

  // plain iterator
  iterator begin() noexcept { return base_::begin(); }

  const_iterator begin() const noexcept { return base_::begin(); }

  iterator end() noexcept { return base_::end(); }

  const_iterator end() const noexcept { return base_::end(); }

  // reverse iterator
  reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }

  const_reverse_iterator rbegin() const noexcept {
    return reverse_iterator(end());
  }

  reverse_iterator rend() noexcept { return reverse_iterator(begin()); }

  const_reverse_iterator rend() const noexcept {
    return reverse_iterator(begin());
  }

  // const iterator
  const_iterator cbegin() const noexcept { return begin(); }

  const_iterator cend() const noexcept { return end(); }

  const_reverse_iterator crbegin() const noexcept { return rbegin(); }

  const_reverse_iterator crend() const noexcept { return rend(); }

  // >>> capacity
  size_type size() const noexcept { return base_::size(); }

  size_type max_size() const noexcept {
    return alloc_traits_::max_size(this->alloc_);
  }

  bool empty() const noexcept { return this->size_ == 0; }

  void resize(size_type n);

  void resize(size_type n, const value_type &val);

  void shrink_to_fit();

  // >>> element access
  reference operator[](size_type n);

  const_reference operator[](size_type n) const;

  reference at(size_type n);

  const_reference at(size_type n) const;

  reference front() {
    assert(!empty());
    return *begin();
  }

  const_reference front() const {
    assert(!empty());
    return *begin();
  }

  reference back() {
    assert(!empty());
    return *(end() - 1);
  }

  const_reference back() const {
    assert(!empty());
    return *(end() - 1);
  }

  // >>> modifier
  // insert
  void push_front(const value_type &val);

  void push_front(value_type &&val);

  void push_back(const value_type &val);

  void push_back(value_type &&val);

  template <class... Args>
  reference emplace_front(Args &&... args);

  template <class... Args>
  reference emplace_back(Args &&... args);

  template <class... Args>
  iterator emplace(const_iterator pos, Args &&... args);

  iterator insert(const_iterator pos, const value_type &val);

  iterator insert(const_iterator pos, value_type &&val);

  iterator insert(const_iterator pos, size_type n, const value_type &val);

  template <class InputIterator>
  typename enable_if<
      __is_input_iterator<InputIterator>::value &&
          !__is_forward_iterator<InputIterator>::value &&
          is_constructible<value_type, typename iterator_traits<
                                           InputIterator>::reference>::value,
      iterator>::type
  insert(const_iterator pos, InputIterator first, InputIterator last);

  template <class ForwardIterator>
  typename enable_if<
      __is_forward_iterator<ForwardIterator>::value &&
          is_constructible<value_type, typename iterator_traits<
                                           ForwardIterator>::reference>::value,
      iterator>::type
  insert(const_iterator pos, ForwardIterator first, ForwardIterator last);

  iterator insert(const_iterator pos, initializer_list<value_type> init);

  // remove
  void pop_front();

  void pop_back();

  iterator erase(const_iterator pos);

  iterator erase(const_iterator first, const_iterator last);

  void clear() noexcept;

  void swap(deque &x) noexcept(alloc_traits_::is_always_equal::value);

 private:
  // >>> private auxiliary function
  // throw length error
  void throw_length_error_() const { throw ::std::length_error("vector"); }

  // throw out of range error
  void throw_out_of_range_() const { throw ::std::out_of_range("vector"); }

  // calculate how many blocks can contain n element
  size_type block_alloc_strategy_(size_type n) const {
    // if map_ is empty(), n += 1
    // for keeping map_iter_ of end() vaild, keep a position in the last slot.
    n += static_cast<size_type>(this->map_.empty());
    return n / this->block_size_() +
           static_cast<size_type>(n % this->block_size_() != 0);
  }

  // query rest space
  // return the rest front space of deque
  size_type front_spare_() const { return this->start_; }

  // return the rest back space of deque
  size_type back_spare_() const {
    // for keeping map_iter_ of end() vaild, keep a position in the last slot.
    return this->block_size_() * this->map_.size() - 1 - front_spare_() -
           size();
  }

  // alloc additional space, strong guarentee exception safe
  void alloc_front_();

  void alloc_front_(size_type n);

  void alloc_back_();

  void alloc_back_(size_type n);

  // append operation
  void append_(size_type n);

  void append_(size_type n, const value_type &val);

  template <class InputIterator>
  void append_(
      InputIterator first,
      typename enable_if<
          __is_input_iterator<InputIterator>::value &&
              !__is_forward_iterator<InputIterator>::value &&
              is_constructible<
                  value_type,
                  typename iterator_traits<InputIterator>::reference>::value,
          InputIterator>::type last);

  template <class ForwardIterator>
  void append_(
      ForwardIterator first,
      typename enable_if<
          __is_forward_iterator<ForwardIterator>::value &&
              is_constructible<
                  value_type,
                  typename iterator_traits<ForwardIterator>::reference>::value,
          ForwardIterator>::type);
};

// >>> private auxiliary function

// >>> alloc additional space, strong guarentee exception safe
// add empty block at front of map
template <class T, class Allocator>
void deque<T, Allocator>::alloc_front_() {
  // if back_spare_() > block_size, move the back free block to front
  if (back_spare_() >= this->block_size_()) {
    pointer p = this->map_.back();
    this->map_.pop_back();
    this->map_.push_front(p);
    // if map_ do not reserve
  } else if (this->map_.size() < this->map_.capacity()) {
    // do allocate to the front
    if (this->map_.front_spare_() > 0)
      this->map_.push_front(
          alloc_traits_::allocate(this->alloc_, this->block_size_()));
    else {
      this->map_.push_back(
          alloc_traits_::allocate(this->alloc_, this->block_size_()));
      pointer p = this->map_.back();
      this->map_.pop_back();
      this->map_.push_front(p);
    }
    // alloc new map
  } else {
    // alloc a new buffer at front
    __split_buffer<pointer, pointer_allocator_type &> swap_buffer(
        ::std::max<size_type>(2 * this->map_.capacity(), 1), 0,
        this->map_.alloc_);
    block_hold_pointer_ hold_ptr(
        alloc_traits_::allocate(this->alloc_, this->block_size_()));
    swap_buffer.push_back(hold_ptr.get());
    hold_ptr.release();
    for (map_iter_ iter = this->map_.begin(); iter != this->map_.end(); ++iter)
      swap_buffer.push_back(*iter);
    this->map_.swap(swap_buffer);
  }
  // if original map_ is empty(), set start_ at mid of block
  if (this->map_.size() == 1)
    this->start_ = this->block_size_() / 2;
  else
    this->start_ += this->block_size_();
}

// add empty block at front of map
template <class T, class Allocator>
void deque<T, Allocator>::alloc_front_(size_type n) {
  size_type new_block = block_alloc_strategy_(n);
  size_type move_block = back_spare_() / this->block_size_();
  move_block = ::std::min<size_type>(new_block, move_block);
  new_block -= move_block;
  // if new block is 0, no allocate happen
  if (new_block == 0) {
    this->start_ += move_block * this->block_size_();
    for (; move_block > 0; --move_block) {
      pointer p = this->map_.back();
      this->map_.pop_back();
      this->map_.push_front(p);
    }
    // if map_ do not reserve
  } else if (new_block <= this->map_.capacity() - this->map_.size()) {
    // fill front first
    for (; new_block > 0 && this->map_.front_spare_() != 0; --new_block) {
      this->map_.push_front(
          alloc_traits_::allocate(this->alloc_, this->block_size_()));
      this->start_ +=
          this->block_size_() - static_cast<size_type>(this->map_.size() == 1);
    }
    // fill rest at back
    for (; new_block > 0; --new_block, ++move_block)
      this->map_.push_back(
          alloc_traits_::allocate(this->alloc_, this->block_size_()));
    // move back to front
    this->start_ += move_block * this->block_size_();
    for (; move_block > 0; --move_block) {
      pointer p = this->map_.back();
      this->map_.pop_back();
      this->map_.push_front(p);
    }
    // alloc new map
  } else {
    size_type diff_start = (new_block + move_block) * this->block_size_() -
                           static_cast<size_type>(this->map_.empty());
    // alloc a new buffer at front
    __split_buffer<pointer, pointer_allocator_type &> swap_buffer(
        ::std::max<size_type>(2 * this->map_.capacity(),
                              new_block + this->map_.capacity()),
        0, this->map_.alloc_);
    // alloc new block in buffer
    try {
      for (; new_block > 0; --new_block)
        swap_buffer.push_back(
            alloc_traits_::allocate(this->alloc_, this->block_size_()));
    } catch (...) {
      // deallocate all block if throw
      for (map_iter_ iter = swap_buffer.begin(); iter != swap_buffer.end();
           ++iter)
        alloc_traits_::deallocate(this->alloc_, *iter, this->block_size_());
      throw;
    }
    for (; move_block > 0; --move_block) {
      pointer p = this->map_.back();
      this->map_.pop_back();
      swap_buffer.push_back(p);
    }
    for (map_iter_ iter = this->map_.begin(); iter != this->map_.end(); ++iter)
      swap_buffer.push_back(*iter);
    this->map_.swap(swap_buffer);
    this->start_ += diff_start;
  }
}

template <class T, class Allocator>
void deque<T, Allocator>::alloc_back_() {
  // if back_spare_() > block_size, move the back free block to front
  if (front_spare_() >= this->block_size_()) {
    pointer p = this->map_.front();
    this->map_.pop_front();
    this->map_.push_back(p);
    // if map_ do not reserve
  } else if (this->map_.size() < this->map_.capacity()) {
    // do allocate to the front
    if (this->map_.back_spare_() > 0)
      this->map_.push_back(
          alloc_traits_::allocate(this->alloc_, this->block_size_()));
    else {
      this->map_.push_front(
          alloc_traits_::allocate(this->alloc_, this->block_size_()));
      pointer p = this->map_.front();
      this->map_.pop_front();
      this->map_.push_back(p);
    }
  } else {
    __split_buffer<pointer, pointer_allocator_type &> swap_buffer(
        ::std::max<size_type>(2 * this->map_.capacity(), 1), this->map_.size(),
        this->map_.alloc_);
    block_hold_pointer_ hold_ptr(
        alloc_traits_::allocate(this->alloc_, this->block_size_()));
    swap_buffer.push_back(hold_ptr.get());
    hold_ptr.release();
    for (map_iter_ iter = this->map_.end(); iter != this->map_.begin();)
      swap_buffer.push_front(*--iter);
    this->map_.swap(swap_buffer);
  }
}

template <class T, class Allocator>
void deque<T, Allocator>::alloc_back_(size_type n) {
  size_type new_block = block_alloc_strategy_(n);
  size_type move_block = front_spare_() / this->block_size_();
  move_block = ::std::min<size_type>(new_block, move_block);
  new_block -= move_block;
  // if new block is 0, no allocate happen
  if (new_block == 0) {
    this->start_ -= move_block * this->block_size_();
    for (; move_block > 0; --move_block) {
      pointer p = this->map_.front();
      this->map_.pop_front();
      this->map_.push_back(p);
    }
    // if map_ do not reserve
  } else if (new_block <= this->map_.capacity() - this->map_.size()) {
    // fill back first
    for (; new_block > 0 && this->map_.back_spare_() != 0; --new_block)
      this->map_.push_back(
          alloc_traits_::allocate(this->alloc_, this->block_size_()));
    // fill rest at front
    for (; new_block > 0; --new_block, ++move_block) {
      this->map_.push_front(
          alloc_traits_::allocate(this->alloc_, this->block_size_()));
      // change start_ to keep exception safe
      this->start_ +=
          this->block_size_() - static_cast<size_type>(this->map_.size() == 1);
    }
    // move front to start
    this->start_ -= move_block * this->block_size_();
    for (; move_block > 0; --move_block) {
      pointer p = this->map_.front();
      // pop first to guarantee map_ do shift not realloc
      this->map_.pop_front();
      this->map_.push_back(p);
    }
    // alloc new map
  } else {
    size_type diff_start = move_block * this->block_size_();
    // alloc a new buffer at front
    __split_buffer<pointer, pointer_allocator_type &> swap_buffer(
        ::std::max<size_type>(2 * this->map_.capacity(),
                              new_block + this->map_.capacity()),
        this->map_size() - move_block, this->map_.alloc_);
    // alloc new block in buffer
    try {
      for (; new_block > 0; --new_block)
        swap_buffer.push_back(
            alloc_traits_::allocate(this->alloc_, this->block_size_()));
    } catch (...) {
      // deallocate all block if throw
      for (map_iter_ iter = swap_buffer.begin(); iter != swap_buffer.end();
           ++iter)
        alloc_traits_::deallocate(this->alloc_, *iter, this->block_size_());
      throw;
    }
    for (; move_block > 0; --move_block) {
      pointer p = this->map_.front();
      this->map_.pop_front();
      swap_buffer.push_back(p);
    }
    for (map_iter_ iter = this->map_.end(); iter != this->map_.begin();)
      swap_buffer.push_front(*--iter);
    this->map_.swap(swap_buffer);
    this->start_ -= diff_start;
  }
}

// append operation
template <class T, class Allocator>
void deque<T, Allocator>::append_(size_type n) {
  if (back_spare_() < n) alloc_back_(n - back_spare_());
  for (iterator iter = end(); n > 0; ++iter, --n, ++this->size_)
    alloc_traits_::construct(this->alloc_, iter.ptr_);
}

template <class T, class Allocator>
void deque<T, Allocator>::append_(size_type n, const value_type &val) {
  if (back_spare_() < n) alloc_back_(n - back_spare_());
  for (iterator iter = end(); n > 0; ++iter, --n, ++this->size_)
    alloc_traits_::construct(this->alloc_, iter.ptr_, val);
}

template <class T, class Allocator>
template <class InputIterator>
void deque<T, Allocator>::append_(
    InputIterator first,
    typename enable_if<
        __is_input_iterator<InputIterator>::value &&
            !__is_forward_iterator<InputIterator>::value &&
            is_constructible<value_type, typename iterator_traits<
                                             InputIterator>::reference>::value,
        InputIterator>::type last) {
  for (; first != last; ++first) emplace_back(*first);
}

template <class T, class Allocator>
template <class ForwardIterator>
void deque<T, Allocator>::append_(
    ForwardIterator first,
    typename enable_if<
        __is_forward_iterator<ForwardIterator>::value &&
            is_constructible<
                value_type,
                typename iterator_traits<ForwardIterator>::reference>::value,
        ForwardIterator>::type last) {
  typename iterator_traits<ForwardIterator>::difference_type n =
      ::std::distance(first, last);
  if (back_spare_() < n) alloc_back_(n - back_spare_());
  for (iterator iter = end(); first != last; ++iter, ++first, ++this->size_)
    alloc_traits_::construct(this->alloc_, iter.ptr_, *first);
}

// >>> constructor
// constructor with given size and value
template <class T, class Allocator>
deque<T, Allocator>::deque(size_type n) {
  append_(n);
}

template <class T, class Allocator>
deque<T, Allocator>::deque(size_type n, const allocator_type &alloc)
    : base_(alloc) {
  append_(n);
}

template <class T, class Allocator>
deque<T, Allocator>::deque(size_type n, const value_type &val) {
  append_(n, val);
}

template <class T, class Allocator>
deque<T, Allocator>::deque(size_type n, const value_type &val,
                           const allocator_type &alloc)
    : base_(alloc) {
  append_(n, val);
}

// constructor with given range
// if const iterator is input iterator
template <class T, class Allocator>
template <class InputIterator>
deque<T, Allocator>::deque(
    InputIterator first,
    typename enable_if<
        __is_input_iterator<InputIterator>::value &&
            !__is_forward_iterator<InputIterator>::value &&
            is_constructible<value_type, typename iterator_traits<
                                             InputIterator>::reference>::value,
        InputIterator>::type last) {
  append_(first, last);
}

template <class T, class Allocator>
template <class InputIterator>
deque<T, Allocator>::deque(
    InputIterator first,
    typename enable_if<
        __is_input_iterator<InputIterator>::value &&
            !__is_forward_iterator<InputIterator>::value &&
            is_constructible<value_type, typename iterator_traits<
                                             InputIterator>::reference>::value,
        InputIterator>::type last,
    const allocator_type &alloc)
    : base_(alloc) {
  append_(first, last);
}

template <class T, class Allocator>
template <class ForwardIterator>
deque<T, Allocator>::deque(
    ForwardIterator first,
    typename enable_if<
        __is_forward_iterator<ForwardIterator>::value &&
            is_constructible<
                value_type,
                typename iterator_traits<ForwardIterator>::reference>::value,
        ForwardIterator>::type last) {
  append_(first, last);
}

template <class T, class Allocator>
template <class ForwardIterator>
deque<T, Allocator>::deque(
    ForwardIterator first,
    typename enable_if<
        __is_forward_iterator<ForwardIterator>::value &&
            is_constructible<
                value_type,
                typename iterator_traits<ForwardIterator>::reference>::value,
        ForwardIterator>::type last,
    const allocator_type &alloc)
    : base_(alloc) {
  append_(first, last);
}

template <class T, class Allocator>
deque<T, Allocator>::deque(const deque &x)
    : base_(alloc_traits_::select_on_container_copy_construction(x.alloc_)) {
  append_(x.begin(), x.end());
}

template <class T, class Allocator>
deque<T, Allocator>::deque(deque &&x) noexcept(
    is_nothrow_move_constructible<allocator_type>::value)
    : base_(::std::move(x)) {}

template <class T, class Allocator>
deque<T, Allocator>::deque(const deque &x, const allocator_type &alloc)
    : base_(alloc) {
  append_(x.begin(), x.end());
}

template <class T, class Allocator>
deque<T, Allocator>::deque(deque &&x, const allocator_type &alloc)
    : base_(::std::move(x), alloc) {
  if (this->alloc_ != x.alloc) {
    append_(::std::move_iterator<iterator>(x.begin()),
            ::std::move_iterator<iterator>(x.begin()));
  }
}

template <class T, class Allocator>
deque<T, Allocator>::deque(initializer_list<value_type> init) {
  append_(init.begin(), init.end());
}

template <class T, class Allocator>
deque<T, Allocator>::deque(initializer_list<value_type> init,
                           const allocator_type &alloc)
    : base_(alloc) {
  append_(init.begin(), init.end());
}

// >>> assignment operation 
template <class T, class Allocator>
deque<T, Allocator> &deque<T, Allocator>::operator=(const deque &x);

template <class T, class Allocator>
deque<T, Allocator> &deque<T, Allocator>::operator=(deque &&x) noexcept(
    allocator_type::propagate_on_container_move_assignment::value
        &&is_nothrow_move_assignable<allocator_type>::value);

template <class T, class Allocator>
deque<T, Allocator> &deque<T, Allocator>::operator=(
    initializer_list<value_type> init) {
}

template <class T, class Allocator>
void deque<T, Allocator>::assign(size_type n, const value_type &val);

// assign with given range
template <class T, class Allocator>
template <class InputIterator>
void deque<T, Allocator>::assign(
    InputIterator first,
    typename enable_if<
        __is_input_iterator<InputIterator>::value &&
            !__is_forward_iterator<InputIterator>::value &&
            is_constructible<value_type, typename iterator_traits<
                                             InputIterator>::reference>::value,
        InputIterator>::type last);

template <class T, class Allocator>
template <class ForwardIterator>
void deque<T, Allocator>::assign(
    ForwardIterator first,
    typename enable_if<
        __is_forward_iterator<ForwardIterator>::value &&
            is_constructible<
                value_type,
                typename iterator_traits<ForwardIterator>::reference>::value,
        ForwardIterator>::type last);

// >>> capacity

template <class T, class Allocator>
void deque<T, Allocator>::resize(size_type n);

template <class T, class Allocator>
void deque<T, Allocator>::resize(size_type n, const value_type &val);

template <class T, class Allocator>
void deque<T, Allocator>::shrink_to_fit();

// >>> element access
template <class T, class Allocator>
deque<T, Allocator>::reference deque<T, Allocator>::operator[](size_type n);

template <class T, class Allocator>
deque<T, Allocator>::const_reference deque<T, Allocator>::operator[](
    size_type n) const;

template <class T, class Allocator>
deque<T, Allocator>::reference deque<T, Allocator>::at(size_type n);

template <class T, class Allocator>
deque<T, Allocator>::const_reference deque<T, Allocator>::at(size_type n) const;

// >>> modifier
// insert
template <class T, class Allocator>
void deque<T, Allocator>::push_front(const value_type &val);

template <class T, class Allocator>
void deque<T, Allocator>::push_front(value_type &&val);

template <class T, class Allocator>
void deque<T, Allocator>::push_back(const value_type &val);

template <class T, class Allocator>
void deque<T, Allocator>::push_back(value_type &&val);

template <class T, class Allocator>
template <class... Args>
deque<T, Allocator>::reference deque<T, Allocator>::emplace_front(
    Args &&... args);

template <class T, class Allocator>
template <class... Args>
deque<T, Allocator>::reference deque<T, Allocator>::emplace_back(
    Args &&... args);

template <class T, class Allocator>
template <class... Args>
deque<T, Allocator>::iterator deque<T, Allocator>::emplace(const_iterator pos,
                                                           Args &&... args);

template <class T, class Allocator>
deque<T, Allocator>::iterator deque<T, Allocator>::insert(
    const_iterator pos, const value_type &val);

template <class T, class Allocator>
deque<T, Allocator>::iterator deque<T, Allocator>::insert(const_iterator pos,
                                                          value_type &&val);

template <class T, class Allocator>
deque<T, Allocator>::iterator deque<T, Allocator>::insert(
    const_iterator pos, size_type n, const value_type &val);

template <class T, class Allocator>
template <class InputIterator>
typename enable_if<
    __is_input_iterator<InputIterator>::value &&
        !__is_forward_iterator<InputIterator>::value &&
        is_constructible<value_type, typename iterator_traits<
                                         InputIterator>::reference>::value,
    deque<T, Allocator>::iterator>::type
deque<T, Allocator>::insert(const_iterator pos, InputIterator first,
                            InputIterator last);

template <class T, class Allocator>
template <class ForwardIterator>
typename enable_if<
    __is_forward_iterator<ForwardIterator>::value &&
        is_constructible<value_type, typename iterator_traits<
                                         ForwardIterator>::reference>::value,
    deque<T, Allocator>::iterator>::type
deque<T, Allocator>::insert(const_iterator pos, ForwardIterator first,
                            ForwardIterator last);

template <class T, class Allocator>
deque<T, Allocator>::deque<T, Allocator>::iterator deque<T, Allocator>::insert(
    const_iterator pos, initializer_list<value_type> init);

// remove
template <class T, class Allocator>
void deque<T, Allocator>::pop_front();

template <class T, class Allocator>
void deque<T, Allocator>::pop_back();

template <class T, class Allocator>
deque<T, Allocator>::iterator deque<T, Allocator>::erase(const_iterator p);

template <class T, class Allocator>
deque<T, Allocator>::iterator deque<T, Allocator>::erase(const_iterator f,
                                                         const_iterator l);

template <class T, class Allocator>
void deque<T, Allocator>::clear() noexcept;

template <class T, class Allocator>
void deque<T, Allocator>::swap(deque &c) noexcept(
    alloc_traits_::is_always_equal::value);

STL_END

#endif  // _DEQUE_H__
