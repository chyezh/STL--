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

  // get position of value_
  T *get_adressof_value_() { return ::std::addressof(as_node_()->value_); }

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

 protected:
  // >>> constructor
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

  // access end_, remove cv qualifier before access end_
  link_pointer_ end_link_() const noexcept {
    return const_cast<node_base_ &>(end_).self_();
  }

  // >>> access
  iterator begin() noexcept { return iterator(end_link_()->next_); }

  const_iterator begin() const noexcept {
    return const_iterator(end_link_()->next_);
  }

  iterator end() noexcept { return iterator(end_link_()); }

  const_iterator end() const noexcept { return const_iterator(end_link_()); }

  // check isempty
  bool empty() const noexcept { return size_ == 0; }

  size_type size() const noexcept { return size_; }

  // swap operation
  void swap(__list_base &x) noexcept(
      alloc_traits_::propagate_on_container_swap::value ||
      alloc_traits_::is_always_equal::value);

  // clear all node
  void clear() noexcept;

  // copy assignment of allocator
  void copy_assign_alloc_(const __list_base &x) {
    copy_assign_alloc_(
        x, ::std::integral_constant<
               bool, node_alloc_traits_::
                         propagate_on_container_copy_assignment::value>());
  }

  // move assignment for allocator
  void move_assign_alloc_(const __list_base &x) noexcept(
      !node_alloc_traits_::propagate_on_container_move_assignment::value ||
      ::std::is_nothrow_move_assignable<allocator_type>::value) {
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
      ::std::is_nothrow_move_assignable<allocator_type>::value) {
    clear();
    node_alloc_ = ::std::move(x.node_alloc_);
  }

  // move-trival for allocator noexcept
  void move_assign_alloc_(const __list_base &x, false_type) noexcept {}

  // >>> data member
 protected:
  node_base_ end_;
  size_type size_;
  node_allocator_type_ node_alloc_;
};

// clear all data in the list base
template <class T, class Allocator>
void __list_base<T, Allocator>::clear() noexcept {
  if (!empty()) {
    link_pointer_ first = end_link_()->next_;
    link_pointer_ last = end_link_();
    while (first != last) {
      node_alloc_traits_::destroy(node_alloc_, first->get_adressof_value_());
      node_alloc_traits_::deallocate(node_alloc_, first->as_node_(), 1);
      first = first->next_;
    }
    size_ = 0;
  }
}

// swap list base class
template <class T, class Allocator>
void __list_base<T, Allocator>::swap(__list_base &x) noexcept(
    alloc_traits_::propagate_on_container_swap::value ||
    alloc_traits_::is_always_equal::value) {
  // swap allocator and size
  __swap_allocator(node_alloc_, x.node_alloc_);
  ::std::swap(size_, x.size_);
  ::std::swap(end_, x.end_);
  if (size_ == 0)
    // if size == 0, keep next_ and prev_ pointer to node self
    end_.next_ = end_.prev_ = end_.self_();
  else
    end_.prev_->next_ = end_.next_->perv_ = end_.self_();
  if (x.size_ == 0)
    // if size == 0, keep next_ and prev_ pointer to node self
    x.end_.next_ = x.end_.perv_ = x.end_.self_();
  else
    x.end_.perv_->next_ = x.end_.next_->perv_ = x.end_.self();
}

template <class T, class Allocator = allocator<T>>
class list : private __list_base<T, Allocator> {
  // >>> member types

  typedef __list_base<T, Allocator> base_;
  typedef typename base_::node_ node_;
  typedef typename base_::node_allocator_type_ node_allocator_type_;
  typedef typename base_::node_alloc_traits_ node_alloc_traits_;
  typedef typename base_::node_base_ node_base_;
  typedef typename base_::node_base_allocator_type_ node_base_allocator_type_;
  typedef typename base_::node_base_alloc_traits_ node_base_alloc_traits_;
  typedef typename base_::link_pointer_ link_pointer_;
  typedef typename base_::node_pointer_ node_pointer_;

  // type for RAII to keep exception safe when constructor of value throw
  typedef __allocator_destructor<node_allocator_type_> node_destructor_;
  typedef unique_ptr<node_, node_destructor_> hold_pointer_;

 public:
  typedef T value_type;
  typedef Allocator allocator_type;
  typedef typename allocator_type::reference reference;
  typedef typename allocator_type::const_reference const_reference;
  typedef typename allocator_type::pointer pointer;
  typedef typename allocator_type::const_pointer const_pointer;
  typedef typename base_::size_type size_type;
  typedef typename base_::difference_type difference_type;
  typedef typename base_::iterator iterator;
  typedef typename base_::const_iterator const_iterator;
  typedef ::std::reverse_iterator<iterator> reverse_iterator;
  typedef ::std::reverse_iterator<const_iterator> const_reverse_iterator;

  // >>> constructors

 public:
  list() noexcept(is_nothrow_default_constructible<allocator_type>::value) {}

  explicit list(const allocator_type &a) : base_(a) {}

  explicit list(size_type n);

  explicit list(size_type n, const allocator_type &a);

  list(size_type n, const value_type &value);

  list(size_type n, const value_type &value, const allocator_type &a);

  // construct with given range
  template <class InputIterator>
  list(InputIterator first,
       typename enable_if<__is_input_iterator<InputIterator>::value,
                          InputIterator>::type last);

  template <class InputIterator>
  list(InputIterator first,
       typename enable_if<__is_input_iterator<InputIterator>::value,
                          InputIterator>::type last,
       const allocator_type &a);

  // copy constructor
  list(const list &x);

  list(const list &x, const allocator_type &a);

  // move constructor
  list(list &&x) noexcept(is_nothrow_move_constructible<allocator_type>::value);

  list(list &&x, const allocator_type &a);

  // construct with given initializer
  list(::std::initializer_list<value_type> init);

  list(::std::initializer_list<value_type> init, const allocator_type &a);

  // >>> assignment operator

  list &operator=(const list &x);

  list &operator=(list &&x) noexcept(
      allocator_type::propagate_on_container_move_assignment::value
          &&is_nothrow_move_assignable<allocator_type>::value);

  list &operator=(::std::initializer_list<value_type>);

  template <class InputIterator>
  void assign(InputIterator first,
              typename enable_if<__is_input_iterator<InputIterator>::value,
                                 InputIterator>::type last);

  void assign(size_type n, const value_type &val);

  void assign(::std::initializer_list<value_type> init);

  void swap(list &x) noexcept(allocator_traits<allocator_type>::is_always_equal::value);

  // >>> allocator

  allocator_type get_allocator() const noexcept {
    return allcator_type(this->node_alloc_);
  }

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

  bool empty() const noexcept { return base_::empty(); }

  size_type size() const noexcept { return base_::size(); }

  size_type max_size() const noexcept {
    return node_alloc_traits_::max_size(this->node_alloc_);
  }

  // >>> element acces

  reference front() {
    assert(!empty());
    return this->end_link_()->next_->as_node_()->value_;
  }

  const_reference front() const {
    assert(!empty());
    return this->end_link_()->next_->as_node_()->value_;
  }

  reference back() {
    assert(!empty());
    return this->end_link_()->prev_->as_node_()->value_;
  }

  const_reference back() const {
    assert(!empty());
    return this->end_link_()->prev_->as_node_()->value_;
  }

  // >>> modifier
  // front
  template <class... Args>
  reference emplace_front(Args &&... args);

  void pop_front();
  
  void push_front(const value_type &val);

  void push_front(value_type &&val);

  // back
  template <class... Args>
  reference emplace_back(Args &&... args);

  void pop_back();

  void push_back(const value_type &val);

  void push_back(value_type &&val);

  // insert
  template <class... Args>
  iterator emplace(const_iterator position, Args &&... args);

  iterator insert(const_iterator position, const value_type &val);

  iterator insert(const_iterator position, value_type &&val);

  iterator insert(const_iterator position, size_type n, const value_type &val);

  template <class InputIterator>
  iterator insert(const_iterator position, InputIterator first, 
  enable_if<__is_input_iterator<InputIterator>::value, InputIterator>::type last);

  iterator insert(const_iterator position,
                  ::std::initializer_list<value_type> init);

  // erase
  iterator erase(const_iterator position);

  iterator erase(const_iterator position, const_iterator last);
  
  void clear() noexcept;

  // >>> size
  
  void resize(size_type sz);

  void resize(size_type sz, const value_type &val);


  
  // >>> algorithm
  
  void splice(const_iterator position, list &x);

  void splice(const_iterator position, list &&x);

  void splice(const_iterator position, list &x, const_iterator i);

  void splice(const_iterator position, list &&x, const_iterator i);

  void splice(const_iterator position, list &x, const_iterator first,
              const_iterator last);

  void splice(const_iterator position, list &&x, const_iterator first,
              const_iterator last);

  void remove(const value_type &val);

  template <class Predicate>
  void remove_if(Predicate pred);

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
 private:
  // >>> private auxiliary function
  // allocate a node
  hold_pointer_ allocate_node_();

  // link nodes in ranges to pos
  void link_nodes_(link_pointer_ pos, link_pointer_ first, link_pointer_ last);

  void link_nodes_at_front_(link_pointer_ first, link_pointer_ last);

  void link_nodes_at_back_(link_pointer_ first, link_pointer_ last);

  void move_assign_(list &x, true_type);

  void move_assign_(list &x, false_type);
};

template <class T, class Allocator>
typename list<T, Allocator>::hold_pointer_
list<T, Allocator>::allocate_node_() {
  node_pointer_ p = node_alloc_traits_::allocate(this->node_alloc_, 1);
  return hold_pointer_(p, node_destructor_(this->node_alloc_, 1));
}

template <class T, class Allocator>
void list<T, Allocator>::link_nodes_(link_pointer_ pos, link_pointer_ first,
                                     link_pointer_ last) {
  pos->prev_->next_ = first;
  first->prev_ = pos->prev_;
  pos->prev_ = last;
  last->next_ = pos;
}

template <class T, class Allocator>
void list<T, Allocator>::link_nodes_at_front_(link_pointer_ first,
                                              link_pointer_ last) {
  link_nodes_(this->end_link_()->next_, first, last);
}

template <class T, class Allocator>
void list<T, Allocator>::link_nodes_at_back_(link_pointer_ first,
                                             link_pointer_ last) {
  link_nodes_(this->end_link_(), first, last);
}

template <class T, class Allocator>
list<T, Allocator>::list(size_type n) {
  for (; n > 0; --n) emplace_back();
}

template <class T, class Allocator>
list<T, Allocator>::list(size_type n, const allocator_type &a) : base_(a) {
  for (; n > 0; --n) emplace_back();
}

template <class T, class Allocator>
list<T, Allocator>::list(size_type n, const value_type &value) {
  for (; n > 0; --n) emplace_back(value);
}

template <class T, class Allocator>
list<T, Allocator>::list(size_type n, const value_type &value,
                         const allocator_type &a)
    : base_(a) {
  for (; n > 0; --n) emplace_back(value);
}

template <class T, class Allocator>
template <class InputIterator>
list<T, Allocator>::list(
    InputIterator first,
    typename enable_if<__is_input_iterator<InputIterator>::value,
                       InputIterator>::type last) {
  for (; first != last; ++first) emplace_back(*first);
}

template <class T, class Allocator>
template <class InputIterator>
list<T, Allocator>::list(
    InputIterator first,
    typename enable_if<__is_input_iterator<InputIterator>::value,
                       InputIterator>::type last,
    const allocator_type &a)
    : base_(a) {
  for (; first != last; ++first) emplace_back(*first);
}

template <class T, class Allocator>
list<T, Allocator>::list(const list &x) {}

template <class T, class Allocator>
list<T, Allocator>::list(const list &x, const allocator_type &a) {}

template <class T, class Allocator>
list<T, Allocator>::list(list &&x) noexcept(
    is_nothrow_move_constructible<allocator_type>::value) {}

template <class T, class Allocator>
list<T, Allocator>::list(list &&x, const allocator_type &a) {}

template <class T, class Allocator>
list<T, Allocator>::list(::std::initializer_list<value_type> init) {}

template <class T, class Allocator>
list<T, Allocator>::list(::std::initializer_list<value_type> init,
                         const allocator_type &a) {}

// >>> assignment operator
template <class T, class Allocator>
list<T, Allocator> &list<T, Allocator>::operator=(cosnt list &x) {}

template <class T, class Allocator>
list<T, Allcator> &list<T, Allocator>::operator=(list &&x) noexcept(
    allocator_type::propagate_on_container_move_assignment::value
        &&is_nothrow_move_assignable<allocator_type>::value) {}

template <class T, class Allocator>
list<T, Allcator> &list<T, Allocator>::operator=(
    ::std::initializer_list<value_type>) {}

template <class T, class Allocator>
template <class InputIterator>
void list<T, Allocator>::assign(
    InputIterator first,
    typename enable_if<__is_input_iterator<InputIterator>::value,
                       InputIterator>::type last) {}

template <class T, class Allocator>
void list<T, Allocator>::assign(size_type n, const value_type &t) {}

template <class T, class Allocator>
void list<T, Allocator>::assign(::std::initializer_list<value_type>) {}

template <class T, class Allocator>
void list<T, Allocator>::swap(list &x) noexcept(allocator_traits<allocator_type>::is_always_equal::value) {}

// >>> modifier
// front
template <class T, class Allocator>
template <class... Args>
typename list<T, Allocator>::reference list<T, Allocator>::emplace_front(
    Args &&... args) {
  hold_pointer_ hold_ptr = allocate_node_();
  node_alloc_traits_::construct(this->node_alloc_,
                                hold_ptr->get_adressof_value_(),
                                ::std::forward<Args>(args)...);
  link_nodes_at_front_(hold_ptr.get()->as_link_(), hold_ptr.get()->as_link_());
  ++this->size_;
  return hold_ptr.release()->value_;
}

template <class T, class Allocator>
void list<T, Allocator>::push_front(const value_type &val) {
  hold_pointer_ hold_ptr = allocate_node_();
  node_alloc_traits_::construct(this->node_alloc_,
                                hold_ptr->get_adressof_value_(), val);
  link_nodes_at_front_(hold_ptr.get()->as_link_(), hold_ptr.get()->as_link_());
  ++this->size_;
  hold_ptr.release();
}

template <class T, class Allocator>
void list<T, Allocator>::push_front(value_type &&val) {
  hold_pointer_ hold_ptr = allocate_node_();
  node_alloc_traits_::construct(
      this->node_alloc_, hold_ptr->get_adressof_value_(), ::std::move(val));
  link_nodes_at_front_(hold_ptr.get()->as_link_(), hold_ptr.get()->as_link_());
  ++this->size_;
  hold_ptr.release();
}

// back
template <class T, class Allocator>
template <class... Args>
typename list<T, Allocator>::reference list<T, Allocator>::emplace_back(
    Args &&... args) {
  hold_pointer_ hold_ptr = allocate_node_();
  node_alloc_traits_::construct(this->node_alloc_,
                                hold_ptr->get_adressof_value_(),
                                ::std::forward<Args>(args)...);
  link_nodes_at_back_(hold_ptr.get()->as_link_(), hold_ptr.get()->as_link_());
  ++this->size_;
  return hold_ptr.release()->value_;
}

template <class T, class Allocator>
void list<T, Allocator>::push_back(const value_type &al) {
  hold_pointer_ hold_ptr = allocate_node_();
  node_alloc_traits_::construct(this->node_alloc_,
                                hold_ptr->get_adressof_value_(), val);
  link_nodes_at_back_(hold_ptr.get()->as_link_(), hold_ptr.get()->as_link_());
  ++this->size_;
  hold_ptr.release();
}

template <class T, class Allocator>
void list<T, Allocator>::push_back(value_type &&val) {
  hold_pointer_ hold_ptr = allocate_node_();
  node_alloc_traits_::construct(
      this->node_alloc_, hold_ptr->get_adressof_value_(), ::std::move(val));
  link_nodes_at_back_(hold_ptr.get()->as_link_(), hold_ptr.get()->as_link_());
  ++this->size_;
  hold_ptr.release();
}

// insert
template <class T, class Allocator>
template <class... Args>
list<T, Allocator>::iterator list<T, Allocator>::emplace(const_iterator position, Args &&... args) {}

template <class T, class Allocator>
list<T, Allocator>::iterator list<T, Allocator>::insert(const_iterator position, const value_type &val) {}

template <class T, class Allocator>
list<T, Allocator>::iterator list<T, Allocator>::insert(const_iterator position, value_type &&val) {}

template <class T, class Allocator>
list<T, Allocator>::iterator list<T, Allocator>::insert(const_iterator position, size_type n, const value_type &val) {}

template <class T, class Allocator>
template <class InputIterator>
list<T, Allocator>::iterator list<T, Allocator>::insert(const_iterator position, InputIterator first, 
  enable_if<__is_input_iterator<InputIterator>::value, InputIterator>::type last){}

template <class T, class Allocator>
list<T, Allocator>::iterator list<T, Allocator>::insert(const_iterator position,
                  ::std::initializer_list<value_type> init) {}

// erase
template <class T, class Allocator>
list<T, Allocator>::iterator list<T, Allocator>::erase(const_iterator position) {}

template <class T, class Allocator>
list<T, Allocator>::iterator list<T, Allocator>::erase(const_iterator position, const_iterator last) {}


template <class T, class Allocator>
void list<T, Allocator>::clear() noexcept {}

// >>> size
template <class T, class Allocator>
void list<T, Allocator>::resize(size_type sz) {}

template <class T, class Allocator>
void list<T, Allocator>::resize(size_type sz, const value_type &val) {}

template <class T, class Allocator>
void list<T, Allocator>::splice(const_iterator position, list &x) {}

template <class T, class Allocator>
void list<T, Allocator>::splice(const_iterator position, list &&x) {}

template <class T, class Allocator>
void list<T, Allocator>::splice(const_iterator position, list &x, const_iterator iter) {}

template <class T, class Allocator>
void list<T, Allocator>::splice(const_iterator position, list &&x, const_iterator iter) {}

template <class T, class Allocator>
void list<T, Allocator>::splice(const_iterator position, list &x, const_iterator first, const_iterator last) {}

template <class T, class Allocator>
void list<T, Allocator>::splice(const_iterator position, list &&x, const_iterator first, const_iterator last) {}

template <class T, class Allocator>
void list<T, Allocator>::remove(const value_type &val) {}

template <class T, class Allocator>
template <class Predicate>
void list<T, Allocator>::remove_if(Predicate pred) {}

template <class T, class Allocator>
void list<T, Allocator>::unique() {}

template <class T, class Allocator>
template <class BinaryPredicate>
void list<T, Allocator>::unique(BinaryPredicate binary_pred) {}

template <class T, class Allocator>
void list<T, Allocator>::merge(list &x) {}

template <class T, class Allocator>
void list<T, Allocator>::merge(list &&x) {}


template <class T, class Allocator>
template <class Compare>
void list<T, Allocator>::merge(list &x, Compare comp) {}


template <class T, class Allocator>
template <class Compare>
void list<T, Allocator>::merge(list &&x, Compare comp) {}

template <class T, class Allocator>
void list<T, Allocator>::sort() {}

template <class T, class Allocator>
template <class Compare>
void list<T, Allocator>::sort(Compare comp) {} 

template <class T>
void list<T, Allocator>::reverse() noexcept {}

STL_END

#endif  // !_STL_LIST__