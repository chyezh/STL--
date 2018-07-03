#ifndef _STL_LIST__
#define _STL_LIST__

#include "Def/stldef.h"

STL_BEGIN

template <class T, class VoidPtr>
struct __list_node;

// list node base type for data section
template <class T, class VoidPtr>
struct __list_node_base {
  typedef pointer_traits<VoidPtr>::rebind<__list_node<T, VoidPtr>>
      node_pointer_;
  typedef pointer_traits<VoidPtr>::rebind<__list_node_base<T, VoidPtr>>
      node_base_pointer_;
};

template <class T, class VoidPtr>
struct __list_node {};

template <class T, class Alloc = allocator<T>>
class list {
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