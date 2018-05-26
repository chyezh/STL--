#ifndef _VECTOR__
#define _VECTOR__

#include "Def/stldef.h"
#include <iterator>
#include <type_traits>
#include <initializer_list>

STL_BEGIN

template <class T, class Allocator = allocator<T>>
class vector {
public:
// typedef.
    typedef T                                        value_type;
    typedef Allocator                                allocator_type;
    typedef typename allocator_type::reference       reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef T*                                       iterator; // implementation-defined
    typedef const T*                                 const_iterator; //implementation-defined
    typedef typename allocator_type::size_type       size_type;
    typedef typename allocator_type::difference_type difference_type;
    typedef typename allocator_type::pointer         pointer;
    typedef typename allocator_type::const_pointer   const_pointer;
    typedef std::reverse_iterator<iterator>          reverse_iterator;
    typedef std::reverse_iterator<const_iterator>    const_reverse_iterator;
// construtor.
    vector() noexcept(std::is_nothrow_default_constructible<allocator_type>::value);
    explicit vector(const allocator_type&);
    explicit vector(size_type n);
    explicit vector(size_type n, const allocator_type&); 
    vector(size_type n, const value_type& value, const allocator_type& = allocator_type());
    template <class InputIterator>
        vector(InputIterator first, InputIterator last, const allocator_type& = allocator_type());
    vector(const vector& x);
    vector(vector&& x) noexcept(std::is_nothrow_move_constructible<allocator_type>::value);
    vector(std::initializer_list<value_type> init, const allocator_type& = allocator_type());
// deconstructor.
    ~vector();
// assignment operator.
    vector& operator=(const vector& x);
    vector& operator=(vector&& x) noexcept(
        std::allocator_traits<allocator_type>::propagate_on_container_move_assignment::value
        || std::allocator_traits<allocator_type>::is_always_equal::value);
    vector& operator=(std::initializer_list<value_type> init);
// member function.
// assign.
    void assign(size_type n, const value_type& value);
    void assign(std::initializer_list<value_type> init);
// allocator.
    allocator_type get_allocator() const noexcept;
// iterator.
// plain iterator.
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
// reverse iterator.
    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;
// const iterator
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    const_reverse_iterator crend() const noexcept;
// size.
    size_type size() const noexcept;
    size_type max_size() const noexcept;
    bool empty() const noexcept;
    void resize(size_type n);
    void resize(size_type n, const value_type & value);
// capacity.
    size_type capacity() const noexcept;
    void reserve(size_type n);
    void shrink_to_fit() noexcept;
// access.
    reference operator[](size_type n);
    const_reference operator[](size_type n) const;
    reference at(size_type n);
    const_reference at(size_type n) const;
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;
    value_type* data() noexcept;
    const value_type* data() const noexcept;
// modifier.
// insert.
    void push_back(const value_type& value);
    void push_back(value_type&& value);
    template <class... Args>
        reference emplace_back(Args&&... args);
    template <class... Args> 
        iterator emplace(const_iterator position, Args&&... args);
    iterator insert(const_iterator position, const value_type& value);
    iterator insert(const_iterator position, value_type&& value);
    iterator insert(const_iterator position, size_type n, const value_type& value);
    template <class InputIterator>
        iterator insert(const_iterator position, InputIterator first, InputIterator last);
    iterator insert(const_iterator position, std::initializer_list<value_type> init);
// remove.
    iterator erase(const_iterator position);
    iterator erase(const_iterator first, const_iterator last);
    void pop_back();
    void clear() noexcept;
    void swap(vector&) noexcept(
        std::allocator_traits<allocator_type>::propagate_on_container_swap::value
        || std::allocator_traits<allocator_type>::is_always_equal::value);
private:
    value_type* data_;
    
};

// the implementation of vector constrctor.






STL_END

#endif // !_VECTOR__
