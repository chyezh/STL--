#ifndef STL_UTILITY_H__
#define STL_UTILITY_H__

#include "../Def/stldef.h"
#include <iterator>

STL_BEGIN

template <class Iterator>
using iterator_traits = std::iterator_traits<Iterator>;

template <class Iterator>
class __iter_wraper {
public:
    typedef Iterator                                                   iterator_type;
    typedef typename iterator_traits<iterator_type>::iterator_category iterator_category;
    typedef typename iterator_traits<iterator_type>::value_type        value_type;
    typedef typename iterator_traits<iterator_type>::difference_type   difference_type;
    typedef typename iterator_traits<iterator_type>::pointer           pointer;
    typedef typename iterator_traits<iterator_type>::reference         reference;
};


STL_END

#endif // !STL_UTILITY_H__