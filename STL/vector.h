#ifndef _VECTOR__
#define _VECTOR__

#include "Def/stldef.h"
#include <iterator>
#include <type_traits>
#include <initializer_list>
#include <iostream>

STL_BEGIN

template <class T, class Allocator = std::allocator<T>>
class vector 
{
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
// constructor.
    vector() noexcept(std::is_nothrow_default_constructible<allocator_type>::value);
    explicit vector(const allocator_type& alloc);
    explicit vector(size_type n);
    explicit vector(size_type n, const allocator_type& alloc); 
    vector(size_type n, const value_type& value, const allocator_type& alloc = allocator_type());
    template <class InputIterator>
        vector(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type());
    vector(const vector& x);
    vector(vector&& x) noexcept(std::is_nothrow_move_constructible<allocator_type>::value);
    vector(std::initializer_list<value_type> init, const allocator_type& alloc = allocator_type());
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
    void allocate_(size_type n);
    void deallocate_();
    void construct_default_at_(pointer loc);
    void construct_at_(pointer loc, const value_type& value);
    void destroy_at_(pointer loc);

    allocator_type alloc_;
    pointer begin_;
    pointer end_;
    pointer cap_;
};

// private method.
template <class T, class Allocator>
void vector<T, Allocator>::allocate_(size_type n) {
    begin_ = std::allocator_traits<allocator_type>::allocate(alloc_, n);
    cap_ = begin_ + n;
    end_ = begin_;
}

template <class T, class Allocator>
void vector<T, Allocator>::deallocate_() {
    std::allocator_traits<allocator_type>::deallocate(alloc_, begin_, size());
}

template <class T, class Allocator>
void vector<T, Allocator>::construct_default_at_(pointer loc) {
    std::allocator_traits<allocator_type>::construct(alloc_, loc);
}

template <class T, class Allocator>
void vector<T, Allocator>::construct_at_(pointer loc, const value_type& value) {
    std::allocator_traits<allocator_type>::construct(alloc_, loc, value);
}

template <class T, class Allocator>
void vector<T, Allocator>::destroy_at_(pointer loc) {
    std::allocator_traits<allocator_type>::destroy(alloc_, loc);
}


// the implementation of vector constructor.
template <class T, class Allocator>
vector<T, Allocator>::vector() noexcept(
    std::is_nothrow_default_constructible<allocator_type>::value) : 
    alloc_(allocator_type()),
    begin_(nullptr),
    end_(nullptr),
    cap_(nullptr) {}

template <class T, class Allocator>
vector<T, Allocator>::vector(const allocator_type& alloc) : 
    alloc_(alloc),
    begin_(nullptr),
    end_(nullptr),
    cap_(nullptr) {}

template <class T, class Allocator>
vector<T, Allocator>::vector(size_type n) : 
    alloc_(allocator_type()),
    begin_(nullptr),
    end_(nullptr),
    cap_(nullptr) {
    allocate_(n);
    while(n-- > 0) {
        construct_default_at_(end_++);
    }
}

template <class T, class Allocator>
vector<T, Allocator>::vector(size_type n, const allocator_type& alloc) :
    alloc_(alloc),
    begin_(nullptr),
    end_(nullptr),
    cap_(nullptr) {
    allocate_(n);
    while(n-- > 0) {
        construct_default_at_(end_++);
    }
}

template <class T, class Allocator>
vector<T, Allocator>::vector(size_type n, const value_type& value, const allocator_type& alloc) :
    alloc_(alloc),
    begin_(nullptr),
    end_(nullptr),
    cap_(nullptr) {
    allocate_(n);
    while(n-- > 0) {
        construct_at_(end_++, value);
    }
}
/*****
template <class T, class Allocator>
template <class InputIterator>
vector<T, Allocator>::vector(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type()) :
    alloc_(alloc),
    begin_(nullptr),
    end_(nullptr),
    cap_(nullptr) {
    allocate_(std::distance(first, last));
    while(first != last) {
        construct_at(end_++, *first++);
    }
}
****/
template <class T, class Allocator>
vector<T, Allocator>::vector(const vector& x) :
    alloc_(x.alloc_), 
    begin_(nullptr),
    end_(nullptr),
    cap_(nullptr) {
    allocate_(x.size());
    const_iterator first = x.begin();
    const_iterator last = x.end();
    while(first != last) {
        construct_at_(end_++, *first++);
    }
}

template <class T, class Allocator>
vector<T, Allocator>::vector(vector&& x) noexcept(
    std::is_nothrow_move_constructible<allocator_type>::value) : 
    alloc_(std::move(x.alloc_)),
    begin_(x.begin_), 
    end_(x.end_),
    cap_(x.cap_) {
    x.begin_ = nullptr;
    x.end_ = nullptr;
    x.cap_ = nullptr;
}

template <class T, class Allocator>
vector<T, Allocator>::vector(std::initializer_list<value_type> init, const allocator_type& alloc) :
    alloc_(alloc),
    begin_(nullptr),
    end_(nullptr),
    cap_(nullptr) {
    allocate_(init.size());
    const_iterator first = init.begin();
    const_iterator last = init.end();
    while(first != last) {
        construct_at_(end_++, *first++);
    }
}

template <class T, class Allocator>
vector<T, Allocator>::~vector() {
    while(end_ != begin_) {
        destroy_at_(--end_);
    }
    deallocate_();
}


// size.
template <class T, class Allocator>
typename vector<T, Allocator>::size_type vector<T, Allocator>::size() const noexcept { 
    return end_ - begin_;
}

STL_END

#endif // !_VECTOR__
