#ifndef STL_VECTOR__
#define STL_VECTOR__

#include <initializer_list>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include "Def/stldef.h"

STL_BEGIN

// vector base type for data section
template <class T, class Allocator>
class __vector_base {
 protected:
  // typedef
  typedef T value_type;
  typedef Allocator allocator_type;
  typedef std::allocator_traits<allocator_type> alloc_traits_;
  typedef typename alloc_traits_::reference reference;
  typedef typename alloc_traits_::const_reference const_reference;
  typedef typename alloc_traits_::pointer iterator;
  typedef typename alloc_traits_::const_pointer const_iterator;
  typedef typename alloc_traits_::size_type size_type;
  typedef typename alloc_traits_::difference_type difference_type;
  typedef typename alloc_traits_::pointer pointer;
  typedef typename alloc_traits_::const_pointer const_pointer;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

 protected:
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

  // destructor.
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

  // destroy every element from end to loc
  // noexcept because of allocator::destroy is noexcept
  void destroy_at_end_(pointer loc) noexcept {
    pointer end_now = end_;
    while (loc != end_now) alloc_traits_::destroy(alloc_, --end_now);
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
        x, std::integral_constant<
               bool,
               alloc_traits_::propagate_on_container_move_assignment::value>());
  }

 private:
  // auxiliary function
  // propagate_on_container_copy_assignment
  // allocator need to be copied when container is copy-assigned
  void copy_assign_alloc_(const __vector_base &x, std::true_type) {
    // deallocate when allocators do not compare equal.
    if (alloc_ != x.alloc_) {
      clear();
      alloc_traits_::deallocate(alloc_, begin_, capacity());
      begin_ = end_ = cap_ = nullptr;
    }
    alloc_ = x.alloc_;
  }

  // copy-trival for allocator.
  void copy_assign_alloc_(const __vector_base &x, std::false_type) {}

  // propagate_on_container_move_assignment
  // allocator need to be moved when container is move-assigned
  // noexcept if allocator is nothrow move assignable
  void move_assign_alloc_(const __vector_base &x, std::true_type) noexcept(
      std::is_nothrow_move_assignable<allocator_type>::value) {
    alloc_ = std::move(x.alloc_);
  }

  // move-trival for allocator. noexcept
  void move_assign_alloc_(const __vector_base &x, std::false_type) noexcept {}

 protected:
  allocator_type alloc_;
  pointer begin_;
  pointer end_;
  pointer cap_;
};

template <class T, class Allocator = std::allocator<T>>
class vector : private __vector_base<T, Allocator> {
 private:
  typedef __vector_base<T, Allocator> base_;
  typedef typename base_::alloc_traits_ alloc_traits_;

 public:
  // typedef
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
  template <class InputIterator>
  vector(InputIterator first, InputIterator last,
         const allocator_type &alloc = allocator_type());

  // copy constructor
  vector(const vector &x);

  // move constructor
  // noexcept if allocator is nothrow move constructible
  vector(vector &&x) noexcept(
      std::is_nothrow_move_constructible<allocator_type>::value);

  // constructor with initial list
  vector(std::initializer_list<value_type> init,
         const allocator_type &alloc = allocator_type());

  // deconstructor
  ~vector();

  // assignment operator
  vector &operator=(const vector &x);
  vector &operator=(vector &&x) noexcept(
      alloc_traits_::propagate_on_container_move_assignment::value ||
      alloc_traits_::is_always_equal::value);
  vector &operator=(std::initializer_list<value_type> init);

  // member function
  // assign
  void assign(size_type n, const value_type &value);
  void assign(std::initializer_list<value_type> init);
  // allocator
  allocator_type get_allocator() const noexcept { return this->alloc_; }
  // iterator
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

  // size
  size_type size() const noexcept {
    return static_cast<size_type>(this->end_ - this->begin_);
  }

  size_type max_size() const noexcept {
    return alloc_traits_::max_size(this->alloc_);
  }

  bool empty() const noexcept { return this->begin_ == this->end_; }

  void resize(size_type n);
  void resize(size_type n, const value_type &value);
  // capacity
  size_type capacity() const noexcept { return base_::capacity(); }

  void reserve(size_type n);
  void shrink_to_fit() noexcept;
  // access
  reference operator[](size_type n);
  const_reference operator[](size_type n) const;
  reference at(size_type n);
  const_reference at(size_type n) const;

  reference front() {
    static_assert(!empty(), "front() called for empty vector");
    return *this->begin_;
  }

  const_reference front() const {
    static_assert(!empty(), "front() called for empty vector");
    return *this->begin_;
  }

  reference back() {
    static_assert(!empty(), "back() called for empty vector");
    return *(this->end_ - 1);
  }

  const_reference back() const {
    static_assert(!empty(), "back() called for emptu vector");
    return *(this->end_ - 1);
  }

  value_type *data() noexcept { return __to_raw_pointer(this->begin_); }

  const value_type *data() const noexcept {
    return __to_raw_pointer(this->begin_);
  }

  // modifier
  // insert
  void push_back(const value_type &value);
  void push_back(value_type &&value);
  template <class... Args>
  reference emplace_back(Args &&... args);
  template <class... Args>
  iterator emplace(const_iterator position, Args &&... args);
  iterator insert(const_iterator position, const value_type &value);
  iterator insert(const_iterator position, value_type &&value);
  iterator insert(const_iterator position, size_type n,
                  const value_type &value);
  template <class InputIterator>
  iterator insert(const_iterator position, InputIterator first,
                  InputIterator last);
  iterator insert(const_iterator position,
                  std::initializer_list<value_type> init);
  // remove
  iterator erase(const_iterator position);
  iterator erase(const_iterator first, const_iterator last);
  void pop_back();

  void clear() noexcept { base_::clear(); }

  void swap(vector &) noexcept(
      alloc_traits_::propagate_on_container_swap::value ||
      alloc_traits_::is_always_equal::value);

 private:
  // throw length error
  void throw_length_error_() { throw std::length_error("vector"); }

  void allocate_(size_type n);

  void deallocate_() noexcept;

  void default_construct_at_end_(size_type n);

  void copy_construct_at_end_(size_type n, const_reference value);

  size_type default_realloc_strategy_(size_type new_size) const {}
};

/* the implementation of vector private auxiliary function */

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

// copy constructs n element at end()
// throws if copy constructor of element throws
// preconditon: n > 0; size() + n <= capacity()
// postcondition: size() == size() + n
template <class T, class Allocator>
void vector<T, Allocator>::copy_construct_at_end_(size_type n,
                                                  const_reference value) {
  do {
    // copy constructor may throw.
    alloc_traits_::construct(this->alloc_, __to_raw_pointer(this->end_), value));
    // updates end_ after construct success.
    ++this->end_;
  } while (--n > 0);
}

/* the implementation of vector constructor */

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

/*****
template <class T, class Allocator>
template <class InputIterator>
vector<T, Allocator>::vector(InputIterator first, InputIterator last, const
allocator_type& alloc = allocator_type()) : alloc_(alloc), begin_(nullptr),
    end_(nullptr),
    cap_(nullptr) {
    allocate_(std::distance(first, last));
    while(first != last) {
        construct_at(end_++, *first++);
    }
}
****/
template <class T, class Allocator>
vector<T, Allocator>::vector(const vector &x)
    : base_(alloc_traits_::select_on_container_copy_constrcution(x.alloc_)) {
  size_type new_size = x.size();
  if(new_size > 0 ) {
    allocate_(new_size);
    
  }
}

template <class T, class Allocator>
vector<T, Allocator>::vector(vector &&x) noexcept(
    std::is_nothrow_move_constructible<allocator_type>::value)
    : alloc_(std::move(x.alloc_)),
      begin_(x.begin_),
      end_(x.end_),
      cap_(x.cap_) {
  x.begin_ = nullptr;
  x.end_ = nullptr;
  x.cap_ = nullptr;
}

template <class T, class Allocator>
vector<T, Allocator>::vector(std::initializer_list<value_type> init,
                             const allocator_type &alloc)
    : alloc_(alloc), begin_(nullptr), end_(nullptr), cap_(nullptr) {
  allocate_(init.size());
  const_iterator first = init.begin();
  const_iterator last = init.end();
  while (first != last) {
    construct_at_(end_++, *first++);
  }
}

template <class T, class Allocator>
vector<T, Allocator>::~vector() {
  while (end_ != begin_) {
    destroy_at_(--end_);
  }
  deallocate_();
}

// size.
template <class T, class Allocator>
typename vector<T, Allocator>::size_type vector<T, Allocator>::size() const
    noexcept {
  return end_ - begin_;
}

STL_END

#endif  // !STL_VECTOR__
