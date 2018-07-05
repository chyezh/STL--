#ifndef _STL_LIST__
#define _STL_LIST__

#include "Def/stldef.h"

STL_BEGIN

template <class T, class VoidPtr>
struct __list_node;

// list node base type for pointer section
template <class T, class VoidPtr>
struct __list_node_base {
  // >>> member types
  typedef
      typename pointer_traits<VoidPtr>::template rebind<__list_node<T, VoidPtr>>
          node_pointer_;
  typedef typename pointer_traits<VoidPtr>::template rebind<
      __list_node_base<T, VoidPtr>>
      link_pointer_;

  __list_node_base() : prev_(self_()), next_(self_()) {}

  // get pointer to base self
  link_pointer_ self_() {
    return pointer_traits<link_pointer_>::pointer_to(*this);
  }

  // a cast function from base to node
  node_pointer_ as_node_() { return static_cast<node_pointer_>(self_()); }

  // pointer for link
  link_pointer_ prev_;
  link_pointer_ next_;
};

// list node keep data section
template <class T, class VoidPtr>
struct __list_node : public __list_node_base<T, VoidPtr> {
  // >>> member types
  typedef __list_node_base<T, VoidPtr> base_;
  typedef typename base_::link_pointer_ link_pointer_;

  // a cast function from node to base
  link_pointer_ as_link_() {
    return static_cast<link_pointer_>(base_::self_());
  }

  // data section
  T value_;
};

template <class T, class Allocator>
class list;
template <class T, class Allocator>
class __list_base;
template <class T, class VoidPtr>
class __list_const_iterator;

template <class T, class VoidPtr>
class __list_iterator {
  // >>> memeber types
  typedef typename __list_node_base<T, VoidPtr>::link_pointer_ link_pointer_;

 public:
  // type traits
  typedef bidirectional_iterator_tag iterator_category;
  typedef T value_type;
  typedef value_type &reference;
  typedef typename pointer_traits<VoidPtr>::template rebind<value_type> pointer;
  typedef typename pointer_traits<pointer>::difference_type difference_type;

 private:
  // friend class
  template <class, class>
  friend class list;
  template <class, class>
  friend class __list_base;
  template <class, class>
  friend class __list_const_iterator;

 public:
  // bidirectional iterator interface
  reference operator*() const { return ptr_->as_node_()->value_; }

  pointer operator->() const {
    return pointer_traits<pointer>::pointer_to(ptr_->as_node_()->value_);
  }

  __list_iterator &operator++() {
    ptr_ = ptr_->next_;
    return *this;
  }

  __list_iterator operator++(int) {
    __list_iterator old_iter(ptr_);
    ptr_ = ptr_->next_;
    return old_iter;
  }

  __list_iterator &operator--() {
    ptr_ = ptr_->prev_;
    return *this;
  }

  __list_iterator operator--(int) {
    __list_iterator old_iter(ptr_);
    ptr_ = ptr_->prev_;
    return old_iter;
  }

  friend bool operator==(const __list_iterator &lhs,
                         const __list_iterator &rhs) {
    return lhs.ptr_ == rhs.ptr_;
  }

  friend bool operator!=(const __list_iterator &lhs,
                         const __list_iterator &rhs) {
    return !(lhs == rhs);
  }

  // >>> constructor
 public:
  // default constructor
  __list_iterator() noexcept : ptr_(nullptr) {}

 private:
  // for friend class
  explicit __list_iterator(link_pointer_ p) noexcept : ptr_(p) {}

 private:
  // data section
  link_pointer_ ptr_;
};

template <class T, class VoidPtr>
class __list_const_iterator {
  // >>> memeber types
  typedef typename __list_node_base<T, VoidPtr>::link_pointer_ link_pointer_;

 public:
  // type traits
  typedef bidirectional_iterator_tag iterator_category;
  typedef T value_type;
  typedef const value_type &reference;
  typedef typename pointer_traits<VoidPtr>::template rebind<const value_type>
      pointer;
  typedef typename pointer_traits<pointer>::difference_type difference_type;

 private:
  template <class, class>
  friend class list;
  template <class, class>
  friend class __list_base;

 public:
  // bidirectional iterator interface
  reference operator*() const { return ptr_->as_node_()->value_; }

  pointer operator->() const {
    return pointer_traits<pointer>::pointer_to(ptr_->as_node_()->value_);
  }

  __list_const_iterator &operator++() {
    ptr_ = ptr_->next_;
    return *this;
  }

  __list_const_iterator operator++(int) {
    __list_const_iterator old_iter(ptr_);
    ptr_ = ptr_->next_;
    return old_iter;
  }

  __list_const_iterator &operator--() {
    ptr_ = ptr_->prev_;
    return *this;
  }

  __list_const_iterator operator--(int) {
    __list_const_iterator old_iter(ptr_);
    ptr_ = ptr_->prev_;
    return old_iter;
  }

  friend bool operator==(const __list_const_iterator &lhs,
                         const __list_const_iterator &rhs) {
    return lhs.ptr_ == rhs.ptr_;
  }

  friend bool operator!=(const __list_const_iterator &lhs,
                         const __list_const_iterator &rhs) {
    return !(lhs == rhs);
  }

  // >>> constructor
 public:
  // default constructor
  __list_const_iterator() noexcept : ptr_(nullptr) {}

  // construct from __list_iterator
  __list_const_iterator(const __list_iterator<T, VoidPtr> &iter)
      : ptr_(iter.ptr_) {}

 private:
  // for friend class
  explicit __list_const_iterator(link_pointer_ p) noexcept : ptr_(p) {}

 private:
  // data section
  link_pointer_ ptr_;
};

template <class T, class Allocator>
class __list_base {
  // >>> member types
 protected:
  typedef T value_type;
  typedef Allocator allocator_type;
  typedef allocator_traits<allocator_type> alloc_traits_;
  typedef typename alloc_traits_::size_type size_type;
  typedef typename alloc_traits_::void_pointer void_pointer_;
  // node and iterator
  typedef __list_node<value_type, void_pointer_> node_;
  typedef __list_node_base<value_type, void_pointer_> node_base_;
  typedef typename node_::link_pointer_ link_pointer_;
  typedef typename node_::node_pointer_ node_pointer_;
  typedef __list_iterator<value_type, void_pointer_> iterator;
  typedef __list_const_iterator<value_type, void_pointer_> const_iterator;
  typedef typename iterator::difference_type difference_type;
  // rebind allocator type
  typedef typename alloc_traits_::template rebind_alloc<node_>
      node_allocator_type_;
  typedef allocator_traits<node_allocator_type_> node_alloc_traits_;
  typedef typename alloc_traits_::template rebind_alloc<node_base_>
      node_base_allocator_type_;
  typedef allocator_traits<node_allocator_type_> node_base_alloc_traits_;

  // >>> constructor
 public:
  // default constructor
  __list_base() noexcept(
      is_nothrow_default_constructible<node_allocator_type_>::value)
      : size_(0), node_alloc_(node_allocator_type_()) {}

  // no copy operation
  __list_base(const __list_base &) = delete;

  __list_base &operator=(const __list_base &) = delete;

  // construct with given node allocator
  __list_base(const allocator_type &alloc) : size_(0), node_alloc_(alloc) {}

  // >>> destructor
  ~__list_base() { clear(); }

  // >>> access
  iterator begin() noexcept { return iterator(end_.next_); }

  const_iterator begin() const noexcept { return const_iterator(end_.next_); }

  iterator end() noexcept { return iterator(end_.self_()); }

  const_iterator end() const noexcept { return const_iterator(end_.self_()); }

  // check isempty
  bool empty() const noexcept { return size_ == 0; }

  // swap operation
  void swap(__list_base &x) noexcept(
      alloc_traits_::propagate_on_container_swap::value ||
      alloc_traits_::is_always_equal::value);

  // clear all node
  void clear() noexcept;

  // copy assignment of allocator
  void copy_assign_alloc_(const __list_base &x) {
    copy_assign_alloc_(
        x, std::integral_constant<
               bool, node_alloc_traits_::
                         propagate_on_container_copy_assignment::value>());
  }

  // move assignment for allocator
  void move_assign_alloc_(const __list_base &x) noexcept(
      !node_alloc_traits_::propagate_on_container_move_assignment::value ||
      std::is_nothrow_move_assignable<allocator_type>::value) {
    move_assign_alloc_(
        x,
        integral_constant<bool,
                          node_alloc_traits_::
                              propagate_on_container_move_assignment::value>());
  }

 private:
  // >>> private auxiliary function
  // propagate_on_container_copy_assignment
  // allocator need to be copied when container is copy-assigned
  void copy_assign_alloc_(const __list_base &x, true_type) {
    // clear when allocators do not compare equal.
    if (node_alloc_ != x.node_alloc_) clear();
    node_alloc_ = x.node_alloc_;
  }

  // copy-trival for allocator.
  void copy_assign_alloc_(const __list_base &x, false_type) {}

  // propagate_on_container_move_assignment
  // allocattor need to be moved when container is move-assigned
  // noexcept if allocator is nothrow move assignable
  void move_assign_alloc_(const __list_base &x, true_type) noexcept(
      std::is_nothrow_move_assignable<allocator_type>::value) {
    clear();
    node_alloc_ = std::move(x.node_alloc_);
  }

  // move-trival for allocator noexcept
  void move_assign_alloc_(const __list_base &x, false_type) noexcept {}

  // >>> data member
 private:
  node_base_ end_;
  size_type size_;
  node_allocator_type_ node_alloc_;
};

template <class T, class Allocator>
void __list_base<T, Allocator>::clear() noexcept {
  if (!empty()) {
    link_pointer_ first = end_.next_;
    link_pointer_ last = end_;
    while (first != last) {
      node_pointer_ node_d = first->as_node_();
      first = first->next_;
      node_alloc_traits_::destroy(node_alloc_, std::addressof(node_d->value_));
      node_alloc_traits_::deallocate(node_alloc_, node_d, 1);
    }
    size_ = 0;
  }
}

template <class T, class Allocator>
void __list_base<T, Allocator>::swap(__list_base &x) noexcept(
    alloc_traits_::propagate_on_container_swap::value ||
    alloc_traits_::is_always_equal::value) {
  // swap allocator and size
  __swap_allocator(node_alloc_, x.node_alloc_);
  std::swap(size_, x.size_);
  std::swap(end_, x.end_);
  if(size_ == 0)
    // if size == 0, keep next_ and prev_ pointer to node self
    end_.next_ = end_.prev_ = end_.self_();
  else
    end_.prev_->next_ = end_.next_->perv_ = end_.self_();
  if(x.size_ == 0)
    // if size == 0, keep next_ and prev_ pointer to node self
    x.end_.next_ = x.end_.perv_ = x.end_.self_();
  else
    x.end_.perv_->next_ = x.end_.next_->perv_ = x.end_.self();
}

template <class T, class Allocator = allocator<T>>
class list : private __list_base<T, Allocator> {
 public:
  // >>> member types
  typedef T value_type;
  typedef Alloc allocator_type;
  typedef typename allocator_type::reference reference;
  typedef typename allocator_type::const_reference const_reference;
  typedef typename allocator_type::pointer pointer;
  typedef typename allocator_type::const_pointer const_pointer;
  typedef implementation - defined iterator;
  typedef implementation - defined const_iterator;
  typedef implementation - defined size_type;
  typedef implementation - defined difference_type;
  typedef reverse_iterator<iterator> reverse_iterator;
  typedef reverse_iterator<const_iterator> const_reverse_iterator;

  list() noexcept(is_nothrow_default_constructible<allocator_type>::value);
  explicit list(const allocator_type &a);
  explicit list(size_type n);
  explicit list(size_type n, const allocator_type &a);  // C++14
  list(size_type n, const value_type &value);
  list(size_type n, const value_type &value, const allocator_type &a);
  template <class Iter>
  list(Iter first, Iter last);
  template <class Iter>
  list(Iter first, Iter last, const allocator_type &a);
  list(const list &x);
  list(const list &, const allocator_type &a);
  list(list &&x) noexcept(is_nothrow_move_constructible<allocator_type>::value);
  list(list &&, const allocator_type &a);
  list(initializer_list<value_type>);
  list(initializer_list<value_type>, const allocator_type &a);

  ~list();

  list &operator=(const list &x);
  list &operator=(list &&x) noexcept(
      allocator_type::propagate_on_container_move_assignment::value
          &&is_nothrow_move_assignable<allocator_type>::value);
  list &operator=(initializer_list<value_type>);
  template <class Iter>
  void assign(Iter first, Iter last);
  void assign(size_type n, const value_type &t);
  void assign(initializer_list<value_type>);

  allocator_type get_allocator() const noexcept;

  iterator begin() noexcept;
  const_iterator begin() const noexcept;
  iterator end() noexcept;
  const_iterator end() const noexcept;
  reverse_iterator rbegin() noexcept;
  const_reverse_iterator rbegin() const noexcept;
  reverse_iterator rend() noexcept;
  const_reverse_iterator rend() const noexcept;
  const_iterator cbegin() const noexcept;
  const_iterator cend() const noexcept;
  const_reverse_iterator crbegin() const noexcept;
  const_reverse_iterator crend() const noexcept;

  reference front();
  const_reference front() const;
  reference back();
  const_reference back() const;

  bool empty() const noexcept;
  size_type size() const noexcept;
  size_type max_size() const noexcept;

  template <class... Args>
  reference emplace_front(Args &&... args);  // reference in C++17
  void pop_front();
  template <class... Args>
  reference emplace_back(Args &&... args);  // reference in C++17
  void pop_back();
  void push_front(const value_type &x);
  void push_front(value_type &&x);
  void push_back(const value_type &x);
  void push_back(value_type &&x);
  template <class... Args>
  iterator emplace(const_iterator position, Args &&... args);
  iterator insert(const_iterator position, const value_type &x);
  iterator insert(const_iterator position, value_type &&x);
  iterator insert(const_iterator position, size_type n, const value_type &x);
  template <class Iter>
  iterator insert(const_iterator position, Iter first, Iter last);
  iterator insert(const_iterator position, initializer_list<value_type> il);

  iterator erase(const_iterator position);
  iterator erase(const_iterator position, const_iterator last);

  void resize(size_type sz);
  void resize(size_type sz, const value_type &c);

  void swap(list &) noexcept(
      allocator_traits<allocator_type>::is_always_equal::value);  // C++17
  void clear() noexcept;

  void splice(const_iterator position, list &x);
  void splice(const_iterator position, list &&x);
  void splice(const_iterator position, list &x, const_iterator i);
  void splice(const_iterator position, list &&x, const_iterator i);
  void splice(const_iterator position, list &x, const_iterator first,
              const_iterator last);
  void splice(const_iterator position, list &&x, const_iterator first,
              const_iterator last);

  void remove(const value_type &value);
  template <class Pred>
  void remove_if(Pred pred);
  void unique();
  template <class BinaryPredicate>
  void unique(BinaryPredicate binary_pred);
  void merge(list &x);
  void merge(list &&x);
  template <class Compare>
  void merge(list &x, Compare comp);
  template <class Compare>
  void merge(list &&x, Compare comp);
  void sort();
  template <class Compare>
  void sort(Compare comp);
  void reverse() noexcept;
};

STL_END

#endif  // !_STL_LIST__