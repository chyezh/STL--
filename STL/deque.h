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
  for(iterator first = begin(), last = end(); first != last; ++first)
    alloc_traits_::destroy(alloc_, ::std::addressof(*first));
  size_ = 0;
  // release additional block
  for(;map_.size() > 1;) {
    alloc_traits_::deallocate(alloc_, map_.front(), block_size_());
    map_.pop_front();
  }
  // set start_ at half of block if map_ is not empty.
  start_ = map_.empty() ? 0 : block_size_() / 2;
}

/*
template <class T, class Pointer, class MapIterator>
class __deque_iterator {
  // >>> member types
 private:
  typedef MapIterator map_iterator_;

 public:
  typedef random_access_iterator_tag iterator_category;
  typedef T value_type;
  typedef value_type &reference;
  typedef Pointer pointer;
  typedef typename pointer_traits<pointer>::difference_type difference_type;

 private:
  // friend class
  template<class, class>
  friend class __deque_base;
  template<class, class>
  friend class deque;

 public:
  // constructor
  __deque_iterator() : map_iter_(nullptr), ptr_(nullptr) {}

  __deque_iterator(const __deque_iterator &iter)
      : map_iter_(iter.map_iter_), ptr_(iter.ptr_) {}

 public:
  // iterator interface
  reference operator*() const { return *ptr_; }

  pointer operator->() const { return ptr_; }

  __deque_iterator &operator++() {
    if (++ptr_ - *map_iter_ == block_size_) {
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
      ptr_ = *map_iter_ + block_size_;
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
        map_iter_ += step / block_size_;
        ptr_ = *map_iter_ + step % block_size_;
      } else {
        // if offset < 0, decrease map_iter
        step = block_size_ - 1 - step;
        map_iter_ -= step / block_size_;
        ptr_ = *map_iter_ + (block_size_ - 1 - step % block_size_);
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
    return __deque_iterator::block_size_ * (lhs.map_iter_ - rhs.map_iter_) +
           (lhs.ptr_ - *lhs.map_iter_) - (rhs.ptr_ - *rhs.map_iter_);
  }

  reference operator[](difference_type n) const {
      return *(*this + n);
  }

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
    return (lhs.map_iter_ < rhs.map_iter_) || (lhs.map_iter_ == rhs.map_iter_ &&
lhs.ptr_ < rhs.ptr_);
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
  map_iterator_ map_iter_;
  pointer ptr_;

  static const difference_type block_size_;
};
*/

/*

template <class T, class Allocator = allocator<T> >
class deque
{
public:
    // types:
    typedef T value_type;
    typedef Allocator allocator_type;

    typedef typename allocator_type::reference       reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef implementation-defined                   iterator;
    typedef implementation-defined                   const_iterator;
    typedef typename allocator_type::size_type       size_type;
    typedef typename allocator_type::difference_type difference_type;

    typedef typename allocator_type::pointer         pointer;
    typedef typename allocator_type::const_pointer   const_pointer;
    typedef std::reverse_iterator<iterator>          reverse_iterator;
    typedef std::reverse_iterator<const_iterator>    const_reverse_iterator;

    // construct/copy/destroy:
    deque() noexcept(is_nothrow_default_constructible<allocator_type>::value);
    explicit deque(const allocator_type& a);
    explicit deque(size_type n);
    explicit deque(size_type n, const allocator_type& a); // C++14
    deque(size_type n, const value_type& v);
    deque(size_type n, const value_type& v, const allocator_type& a);
    template <class InputIterator>
        deque(InputIterator f, InputIterator l);
    template <class InputIterator>
        deque(InputIterator f, InputIterator l, const allocator_type& a);
    deque(const deque& c);
    deque(deque&& c)
        noexcept(is_nothrow_move_constructible<allocator_type>::value);
    deque(initializer_list<value_type> il, const Allocator& a =
allocator_type()); deque(const deque& c, const allocator_type& a); deque(deque&&
c, const allocator_type& a); ~deque();

    deque& operator=(const deque& c);
    deque& operator=(deque&& c)
        noexcept(
             allocator_type::propagate_on_container_move_assignment::value &&
             is_nothrow_move_assignable<allocator_type>::value);
    deque& operator=(initializer_list<value_type> il);

    template <class InputIterator>
        void assign(InputIterator f, InputIterator l);
    void assign(size_type n, const value_type& v);
    void assign(initializer_list<value_type> il);

    allocator_type get_allocator() const noexcept;

    // iterators:

    iterator       begin() noexcept;
    const_iterator begin() const noexcept;
    iterator       end() noexcept;
    const_iterator end() const noexcept;

    reverse_iterator       rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    reverse_iterator       rend() noexcept;
    const_reverse_iterator rend() const noexcept;

    const_iterator         cbegin() const noexcept;
    const_iterator         cend() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    const_reverse_iterator crend() const noexcept;

    // capacity:
    size_type size() const noexcept;
    size_type max_size() const noexcept;
    void resize(size_type n);
    void resize(size_type n, const value_type& v);
    void shrink_to_fit();
    bool empty() const noexcept;

    // element access:
    reference operator[](size_type i);
    const_reference operator[](size_type i) const;
    reference at(size_type i);
    const_reference at(size_type i) const;
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;

    // modifiers:
    void push_front(const value_type& v);
    void push_front(value_type&& v);
    void push_back(const value_type& v);
    void push_back(value_type&& v);
    template <class... Args> reference emplace_front(Args&&... args);  //
reference in C++17 template <class... Args> reference emplace_back(Args&&...
args);   // reference in C++17 template <class... Args> iterator
emplace(const_iterator p, Args&&... args); iterator insert(const_iterator p,
const value_type& v); iterator insert(const_iterator p, value_type&& v);
    iterator insert(const_iterator p, size_type n, const value_type& v);
    template <class InputIterator>
        iterator insert(const_iterator p, InputIterator f, InputIterator l);
    iterator insert(const_iterator p, initializer_list<value_type> il);
    void pop_front();
    void pop_back();
    iterator erase(const_iterator p);
    iterator erase(const_iterator f, const_iterator l);
    void swap(deque& c)
        noexcept(allocator_traits<allocator_type>::is_always_equal::value);  //
C++17 void clear() noexcept;
};

template <class InputIterator, class Allocator = allocator<typename
iterator_traits<InputIterator>::value_type>> deque(InputIterator, InputIterator,
Allocator = Allocator())
   -> deque<typename iterator_traits<InputIterator>::value_type, Allocator>;

template <class T, class Allocator>
    bool operator==(const deque<T,Allocator>& x, const deque<T,Allocator>& y);
template <class T, class Allocator>
    bool operator< (const deque<T,Allocator>& x, const deque<T,Allocator>& y);
template <class T, class Allocator>
    bool operator!=(const deque<T,Allocator>& x, const deque<T,Allocator>& y);
template <class T, class Allocator>
    bool operator> (const deque<T,Allocator>& x, const deque<T,Allocator>& y);
template <class T, class Allocator>
    bool operator>=(const deque<T,Allocator>& x, const deque<T,Allocator>& y);
template <class T, class Allocator>
    bool operator<=(const deque<T,Allocator>& x, const deque<T,Allocator>& y);

// specialized algorithms:
template <class T, class Allocator>
    void swap(deque<T,Allocator>& x, deque<T,Allocator>& y)
         noexcept(noexcept(x.swap(y)));

*/

STL_END

#endif  // _DEQUE_H__
