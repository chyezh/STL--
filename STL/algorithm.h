#ifndef _ALGORITHM_H__
#define _ALGORITHM_H__

#include "Def/stldef.h"

STL_BEGIN

// >>> non-modifying sequence operations

// all_of
template <class InputIterator, class Predicate>
bool all_of(InputIterator first, InputIterator last, Predicate pred)
{
  for (; first != last; ++first)
    if (!pred(*first))
      return false;
  return true;
}

// any_of
template <class InputIterator, class Predicate>
bool any_of(InputIterator first, InputIterator last, Predicate pred)
{
  for (; first != last; ++first)
    if (pred(*first))
      return true;
  return false;
}

// none_of
template <class InputIterator, class Predicate>
bool none_of(InputIterator first, InputIterator last, Predicate pred)
{
  for (; first != last; ++first)
    if (pred(*first))
      return false;
  return true;
}

// for_each
template <class InputIterator, class Function>
Function for_each(InputIterator first, InputIterator last, Function f)
{
  for (; first != last; ++first)
    f(*first);
  return f;
}

// find
template <class InputIterator, class T>
InputIterator find(InputIterator first, InputIterator last, const T &value)
{
  for (; first != last; ++first)
    if (*first == value)
      return first;
  return last;
}

// find_if
template <class InputIterator, class Predicate>
InputIterator find_if(InputIterator first, InputIterator last, Predicate pred)
{
  for (; first != last; ++first)
    if (pred(*first))
      return first;
  return last;
}

// find_if_not
template <class InputIterator, class Predicate>
InputIterator find_if_not(InputIterator first, InputIterator last,
                          Predicate pred)
{
  for (; first != last; ++first)
    if (!pred(*first))
      return first;
  return last;
}

// find_end
// for forward_iterator
// can only matching forward
template <class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
ForwardIterator1 __find_end(ForwardIterator1 first1, ForwardIterator1 last1,
                            ForwardIterator2 first2, ForwardIterator2 last2,
                            BinaryPredicate pred, forward_iterator_tag,
                            forward_iterator_tag)
{
  // return if range [first2, last2) is empty
  if (first2 == last2)
    return last1;
  auto return_iter = last1;
  for (;;)
  {
    // find the first element matching position
    for (; first1 != last1 && pred(*first1, *first2); ++first1)
      ;
    if (first1 == last1)
      return return_iter;
    ForwardIterator1 iter1 = ::std::next(first1);
    ForwardIterator2 iter2 = ::std::next(first2);
    // matching sequence
    for (; iter1 != last1 && iter2 != last2; ++iter1, ++iter2)
      if (!pred(*iter1, *iter2))
        break;
    // matching success, record position
    if (iter2 == last2)
      return_iter = first1;
    if (iter1 == last1)
      return return_iter;
    ++first1;
  }
}

// for bidirectional_iterator
// can be matching backward
template <class BidirectionalIterator1, class BidirectionalIterator2,
          class BinaryPredicate>
BidirectionalIterator1 __find_end(BidirectionalIterator1 first1,
                                  BidirectionalIterator1 last1,
                                  BidirectionalIterator2 first2,
                                  BidirectionalIterator2 last2,
                                  BinaryPredicate pred,
                                  bidirectional_iterator_tag,
                                  bidirectional_iterator_tag)
{
  // return if range [first2, last2) is empty
  if (first2 == last2)
    return last1;
  BidirectionalIterator1 iter1 = last1;
  BidirectionalIterator2 iter2 = std::prev(last2);
  for (;;)
  {
    // find the last element matching position
    for (; first1 != iter1 && pred(*--iter1, *iter2);)
      ;
    if (iter1 == first1 && *iter1 != *iter2)
      return last1;
    BidirectionalIterator1 iter1_f = iter1;
    BidirectionalIterator2 iter2_f = iter2;
    // matching sequence
    for (; iter1_f != first1 && iter2_f != first2;)
      if (!pred(*--iter1_f, *--iter2_f))
        break;
    // matching success, return position
    if (iter2_f == first2)
      return iter1_f;
    if (iter1_f == first1)
      return last1;
  }
}

template <class RandomAccessIterator1, class RandomAccessIterator2,
          class BinaryPredicate>
RandomAccessIterator1 __find_end(RandomAccessIterator1 first1,
                                 RandomAccessIterator1 last1,
                                 RandomAccessIterator2 first2,
                                 RandomAccessIterator2 last2,
                                 BinaryPredicate pred,
                                 random_access_iterator_tag,
                                 random_access_iterator_tag) {}

template <class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1,
                          ForwardIterator2 first2, ForwardIterator2 last2,
                          BinaryPredicate pred)
{
  __find_end(first1, last1, first2, last2, pred,
             typename iterator_traits<ForwardIterator1>::iterator_category{},
             typename iterator_traits<ForwardIterator2>::iterator_category{});
}

template <class ForwardIterator1, class ForwardIterator2>
ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1,
                          ForwardIterator2 first2, ForwardIterator2 last2)
{
}

template <class InputIterator, class ForwardIterator>
InputIterator find_first_of(InputIterator first1, InputIterator last1,
                            ForwardIterator first2, ForwardIterator last2);
template <class InputIterator, class ForwardIterator, class BinaryPredicate>
InputIterator find_first_of(InputIterator first1, InputIterator last1,
                            ForwardIterator first2, ForwardIterator last2,
                            BinaryPredicate pred);

template <class ForwardIterator>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last);
template <class ForwardIterator, class BinaryPredicate>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last,
                              BinaryPredicate pred);
template <class InputIterator, class T>
typename iterator_traits<InputIterator>::difference_type count(
    InputIterator first, InputIterator last, const T &value);
template <class InputIterator, class Predicate>
typename iterator_traits<InputIterator>::difference_type count_if(
    InputIterator first, InputIterator last, Predicate pred);

template <class InputIterator1, class InputIterator2>
pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1,
                                              InputIterator1 last1,
                                              InputIterator2 first2);
template <class InputIterator1, class InputIterator2, class BinaryPredicate>
pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1,
                                              InputIterator1 last1,
                                              InputIterator2 first2,
                                              BinaryPredicate pred);

template <class InputIterator1, class InputIterator2>
bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2);
template <class InputIterator1, class InputIterator2, class BinaryPredicate>
bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2,
           BinaryPredicate pred);

template <class ForwardIterator1, class ForwardIterator2>
bool is_permutation(ForwardIterator1 first1, ForwardIterator1 last1,
                    ForwardIterator2 first2);
template <class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
bool is_permutation(ForwardIterator1 first1, ForwardIterator1 last1,
                    ForwardIterator2 first2, BinaryPredicate pred);

template <class ForwardIterator1, class ForwardIterator2>
ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1,
                        ForwardIterator2 first2, ForwardIterator2 last2);
template <class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1,
                        ForwardIterator2 first2, ForwardIterator2 last2,
                        BinaryPredicate pred);

template <class ForwardIterator, class Size, class T>
ForwardIterator search_n(ForwardIterator first, ForwardIterator last,
                         Size count, const T &value);
template <class ForwardIterator, class Size, class T, class BinaryPredicate>
ForwardIterator search_n(ForwardIterator first, ForwardIterator last,
                         Size count, const T &value, BinaryPredicate pred);

// modifying sequence operations:

// copy:
template <class InputIterator, class OutputIterator>
OutputIterator copy(InputIterator first, InputIterator last,
                    OutputIterator result);
template <class InputIterator, class Size, class OutputIterator>
OutputIterator copy_n(InputIterator first, Size n, OutputIterator result);
template <class InputIterator, class OutputIterator, class Predicate>
OutputIterator copy_if(InputIterator first, InputIterator last,
                       OutputIterator result, Predicate pred);
template <class BidirectionalIterator1, class BidirectionalIterator2>
BidirectionalIterator2 copy_backward(BidirectionalIterator1 first,
                                     BidirectionalIterator1 last,
                                     BidirectionalIterator2 result);

// move:
template <class InputIterator, class OutputIterator>
OutputIterator move(InputIterator first, InputIterator last,
                    OutputIterator result);
template <class BidirectionalIterator1, class BidirectionalIterator2>
BidirectionalIterator2 move_backward(BidirectionalIterator1 first,
                                     BidirectionalIterator1 last,
                                     BidirectionalIterator2 result);

// swap:
template <class ForwardIterator1, class ForwardIterator2>
ForwardIterator2 swap_ranges(ForwardIterator1 first1, ForwardIterator1 last1,
                             ForwardIterator2 first2);
template <class ForwardIterator1, class ForwardIterator2>
void iter_swap(ForwardIterator1 a, ForwardIterator2 b);
template <class InputIterator, class OutputIterator, class UnaryOperation>
OutputIterator transform(InputIterator first, InputIterator last,
                         OutputIterator result, UnaryOperation op);

template <class InputIterator1, class InputIterator2, class OutputIterator,
          class BinaryOperation>
OutputIterator transform(InputIterator1 first1, InputIterator1 last1,
                         InputIterator2 first2, OutputIterator result,
                         BinaryOperation binary_op);

template <class ForwardIterator, class T>
void replace(ForwardIterator first, ForwardIterator last, const T &old_value,
             const T &new_value);
template <class ForwardIterator, class Predicate, class T>
void replace_if(ForwardIterator first, ForwardIterator last, Predicate pred,
                const T &new_value);
template <class InputIterator, class OutputIterator, class T>
OutputIterator replace_copy(InputIterator first, InputIterator last,
                            OutputIterator result, const T &old_value,
                            const T &new_value);
template <class InputIterator, class OutputIterator, class Predicate, class T>
OutputIterator replace_copy_if(InputIterator first, InputIterator last,
                               OutputIterator result, Predicate pred,
                               const T &new_value);

template <class ForwardIterator, class T>
void fill(ForwardIterator first, ForwardIterator last, const T &value);
template <class OutputIterator, class Size, class T>
OutputIterator fill_n(OutputIterator first, Size n, const T &value);
template <class ForwardIterator, class Generator>
void generate(ForwardIterator first, ForwardIterator last, Generator gen);
template <class OutputIterator, class Size, class Generator>
OutputIterator generate_n(OutputIterator first, Size n, Generator gen);

template <class ForwardIterator, class T>
ForwardIterator remove(ForwardIterator first, ForwardIterator last,
                       const T &value);
template <class ForwardIterator, class Predicate>
ForwardIterator remove_if(ForwardIterator first, ForwardIterator last,
                          Predicate pred);
template <class InputIterator, class OutputIterator, class T>
OutputIterator remove_copy(InputIterator first, InputIterator last,
                           OutputIterator result, const T &value);
template <class InputIterator, class OutputIterator, class Predicate>
OutputIterator remove_copy_if(InputIterator first, InputIterator last,
                              OutputIterator result, Predicate pred);

template <class ForwardIterator>
ForwardIterator unique(ForwardIterator first, ForwardIterator last);
template <class ForwardIterator, class BinaryPredicate>
ForwardIterator unique(ForwardIterator first, ForwardIterator last,
                       BinaryPredicate pred);
template <class InputIterator, class OutputIterator>
OutputIterator unique_copy(InputIterator first, InputIterator last,
                           OutputIterator result);
template <class InputIterator, class OutputIterator, class BinaryPredicate>
OutputIterator unique_copy(InputIterator first, InputIterator last,
                           OutputIterator result, BinaryPredicate pred);

template <class BidirectionalIterator>
void reverse(BidirectionalIterator first, BidirectionalIterator last);
template <class BidirectionalIterator, class OutputIterator>
OutputIterator reverse_copy(BidirectionalIterator first,
                            BidirectionalIterator last, OutputIterator result);

template <class ForwardIterator>
ForwardIterator rotate(ForwardIterator first, ForwardIterator middle,
                       ForwardIterator last);
template <class ForwardIterator, class OutputIterator>
OutputIterator rotate_copy(ForwardIterator first, ForwardIterator middle,
                           ForwardIterator last, OutputIterator result);

template <class RandomAccessIterator>
void random_shuffle(RandomAccessIterator first, RandomAccessIterator last);
template <class RandomAccessIterator, class RandomNumberGenerator>
void random_shuffle(RandomAccessIterator first, RandomAccessIterator last,
                    RandomNumberGenerator &&rand);
template <class RandomAccessIterator, class UniformRandomNumberGenerator>
void shuffle(RandomAccessIterator first, RandomAccessIterator last,
             UniformRandomNumberGenerator &&rand);

// partitions:
template <class InputIterator, class Predicate>
bool is_partitioned(InputIterator first, InputIterator last, Predicate pred);

template <class ForwardIterator, class Predicate>
ForwardIterator partition(ForwardIterator first, ForwardIterator last,
                          Predicate pred);

template <class BidirectionalIterator, class Predicate>
BidirectionalIterator stable_partition(BidirectionalIterator first,
                                       BidirectionalIterator last,
                                       Predicate pred);

template <class InputIterator, class OutputIterator1, class OutputIterator2,
          class Predicate>
pair<OutputIterator1, OutputIterator2> partition_copy(InputIterator first,
                                                      InputIterator last,
                                                      OutputIterator1 out_true,
                                                      OutputIterator2 out_false,
                                                      Predicate pred);

template <class ForwardIterator, class Predicate>
ForwardIterator partition_point(ForwardIterator first, ForwardIterator last,
                                Predicate pred);

// sorting and related operations:

// sorting:
template <class RandomAccessIterator>
void sort(RandomAccessIterator first, RandomAccessIterator last);
template <class RandomAccessIterator, class Compare>
void sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp);

template <class RandomAccessIterator>
void stable_sort(RandomAccessIterator first, RandomAccessIterator last);
template <class RandomAccessIterator, class Compare>
void stable_sort(RandomAccessIterator first, RandomAccessIterator last,
                 Compare comp);

template <class RandomAccessIterator>
void partial_sort(RandomAccessIterator first, RandomAccessIterator middle,
                  RandomAccessIterator last);
template <class RandomAccessIterator, class Compare>
void partial_sort(RandomAccessIterator first, RandomAccessIterator middle,
                  RandomAccessIterator last, Compare comp);
template <class InputIterator, class RandomAccessIterator>
RandomAccessIterator partial_sort_copy(InputIterator first, InputIterator last,
                                       RandomAccessIterator result_first,
                                       RandomAccessIterator result_last);
template <class InputIterator, class RandomAccessIterator, class Compare>
RandomAccessIterator partial_sort_copy(InputIterator first, InputIterator last,
                                       RandomAccessIterator result_first,
                                       RandomAccessIterator result_last,
                                       Compare comp);

template <class ForwardIterator>
bool is_sorted(ForwardIterator first, ForwardIterator last);
template <class ForwardIterator, class Compare>
bool is_sorted(ForwardIterator first, ForwardIterator last, Compare comp);
template <class ForwardIterator>
ForwardIterator is_sorted_until(ForwardIterator first, ForwardIterator last);
template <class ForwardIterator, class Compare>
ForwardIterator is_sorted_until(ForwardIterator first, ForwardIterator last,
                                Compare comp);

template <class RandomAccessIterator>
void nth_element(RandomAccessIterator first, RandomAccessIterator nth,
                 RandomAccessIterator last);
template <class RandomAccessIterator, class Compare>
void nth_element(RandomAccessIterator first, RandomAccessIterator nth,
                 RandomAccessIterator last, Compare comp);
// binary search:
template <class ForwardIterator, class T>
ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last,
                            const T &value);
template <class ForwardIterator, class T, class Compare>
ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last,
                            const T &value, Compare comp);

template <class ForwardIterator, class T>
ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last,
                            const T &value);
template <class ForwardIterator, class T, class Compare>
ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last,
                            const T &value, Compare comp);

template <class ForwardIterator, class T>
pair<ForwardIterator, ForwardIterator> equal_range(ForwardIterator first,
                                                   ForwardIterator last,
                                                   const T &value);
template <class ForwardIterator, class T, class Compare>
pair<ForwardIterator, ForwardIterator> equal_range(ForwardIterator first,
                                                   ForwardIterator last,
                                                   const T &value,
                                                   Compare comp);

template <class ForwardIterator, class T>
bool binary_search(ForwardIterator first, ForwardIterator last, const T &value);
template <class ForwardIterator, class T, class Compare>
bool binary_search(ForwardIterator first, ForwardIterator last, const T &value,
                   Compare comp);

// merge:
template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
                     InputIterator2 first2, InputIterator2 last2,
                     OutputIterator result);
template <class InputIterator1, class InputIterator2, class OutputIterator,
          class Compare>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
                     InputIterator2 first2, InputIterator2 last2,
                     OutputIterator result, Compare comp);

template <class BidirectionalIterator>
void inplace_merge(BidirectionalIterator first, BidirectionalIterator middle,
                   BidirectionalIterator last);
template <class BidirectionalIterator, class Compare>
void inplace_merge(BidirectionalIterator first, BidirectionalIterator middle,
                   BidirectionalIterator last, Compare comp);

// set operations:
template <class InputIterator1, class InputIterator2>
bool includes(InputIterator1 first1, InputIterator1 last1,
              InputIterator2 first2, InputIterator2 last2);
template <class InputIterator1, class InputIterator2, class Compare>
bool includes(InputIterator1 first1, InputIterator1 last1,
              InputIterator2 first2, InputIterator2 last2, Compare comp);

template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
                         InputIterator2 first2, InputIterator2 last2,
                         OutputIterator result);
template <class InputIterator1, class InputIterator2, class OutputIterator,
          class Compare>
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
                         InputIterator2 first2, InputIterator2 last2,
                         OutputIterator result, Compare comp);

template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1,
                                InputIterator2 first2, InputIterator2 last2,
                                OutputIterator result);
template <class InputIterator1, class InputIterator2, class OutputIterator,
          class Compare>
OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1,
                                InputIterator2 first2, InputIterator2 last2,
                                OutputIterator result, Compare comp);

template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1,
                              InputIterator2 first2, InputIterator2 last2,
                              OutputIterator result);
template <class InputIterator1, class InputIterator2, class OutputIterator,
          class Compare>
OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1,
                              InputIterator2 first2, InputIterator2 last2,
                              OutputIterator result, Compare comp);

template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_symmetric_difference(InputIterator1 first1,
                                        InputIterator1 last1,
                                        InputIterator2 first2,
                                        InputIterator2 last2,
                                        OutputIterator result);
template <class InputIterator1, class InputIterator2, class OutputIterator,
          class Compare>
OutputIterator set_symmetric_difference(InputIterator1 first1,
                                        InputIterator1 last1,
                                        InputIterator2 first2,
                                        InputIterator2 last2,
                                        OutputIterator result, Compare comp);

// heap operations:
template <class RandomAccessIterator>
void push_heap(RandomAccessIterator first, RandomAccessIterator last);
template <class RandomAccessIterator, class Compare>
void push_heap(RandomAccessIterator first, RandomAccessIterator last,
               Compare comp);
template <class RandomAccessIterator>
void pop_heap(RandomAccessIterator first, RandomAccessIterator last);
template <class RandomAccessIterator, class Compare>
void pop_heap(RandomAccessIterator first, RandomAccessIterator last,
              Compare comp);

template <class RandomAccessIterator>
void make_heap(RandomAccessIterator first, RandomAccessIterator last);
template <class RandomAccessIterator, class Compare>
void make_heap(RandomAccessIterator first, RandomAccessIterator last,
               Compare comp);

template <class RandomAccessIterator>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last);
template <class RandomAccessIterator, class Compare>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last,
               Compare comp);

template <class RandomAccessIterator>
bool is_heap(RandomAccessIterator first, RandomAccessIterator last);
template <class RandomAccessIterator, class Compare>
bool is_heap(RandomAccessIterator first, RandomAccessIterator last,
             Compare comp);
template <class RandomAccessIterator>
RandomAccessIterator is_heap_until(RandomAccessIterator first,
                                   RandomAccessIterator last);
template <class RandomAccessIterator, class Compare>
RandomAccessIterator is_heap_until(RandomAccessIterator first,
                                   RandomAccessIterator last, Compare comp);
// minimum and maximum:
template <class T>
const T &min(const T &a, const T &b);
template <class T, class Compare>
const T &min(const T &a, const T &b, Compare comp);
template <class T>
T min(initializer_list<T> t);
template <class T, class Compare>
T min(initializer_list<T> t, Compare comp);

template <class T>
const T &max(const T &a, const T &b);
template <class T, class Compare>
const T &max(const T &a, const T &b, Compare comp);
template <class T>
T max(initializer_list<T> t);
template <class T, class Compare>
T max(initializer_list<T> t, Compare comp);

template <class T>
pair<const T &, const T &> minmax(const T &a, const T &b);
template <class T, class Compare>
pair<const T &, const T &> minmax(const T &a, const T &b, Compare comp);
template <class T>
pair<T, T> minmax(initializer_list<T> t);
template <class T, class Compare>
pair<T, T> minmax(initializer_list<T> t, Compare comp);

template <class ForwardIterator>
ForwardIterator min_element(ForwardIterator first, ForwardIterator last);
template <class ForwardIterator, class Compare>
ForwardIterator min_element(ForwardIterator first, ForwardIterator last,
                            Compare comp);

template <class ForwardIterator>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last);
template <class ForwardIterator, class Compare>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last,
                            Compare comp);

template <class ForwardIterator>
pair<ForwardIterator, ForwardIterator> minmax_element(ForwardIterator first,
                                                      ForwardIterator last);
template <class ForwardIterator, class Compare>
pair<ForwardIterator, ForwardIterator> minmax_element(ForwardIterator first,
                                                      ForwardIterator last,
                                                      Compare comp);

template <class InputIterator1, class InputIterator2>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
                             InputIterator2 first2, InputIterator2 last2);
template <class InputIterator1, class InputIterator2, class Compare>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
                             InputIterator2 first2, InputIterator2 last2,
                             Compare comp);

// permutations:
template <class BidirectionalIterator>
bool next_permutation(BidirectionalIterator first, BidirectionalIterator last);
template <class BidirectionalIterator, class Compare>
bool next_permutation(BidirectionalIterator first, BidirectionalIterator last,
                      Compare comp);

template <class BidirectionalIterator>
bool prev_permutation(BidirectionalIterator first, BidirectionalIterator last);
template <class BidirectionalIterator, class Compare>
bool prev_permutation(BidirectionalIterator first, BidirectionalIterator last,
                      Compare comp);

STL_END
#endif // !_ALGORITHM_H__