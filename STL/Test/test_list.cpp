#include <algorithm>
#include <list>
#include "../list.h"
#include "gtest/gtest.h"

template <typename C1, typename C2>
void test_range(const C1 &c1, const C2 &c2) {
  EXPECT_EQ(c1.size(), c2.size());
  auto iter1 = c1.begin();
  auto iter2 = c2.begin();
  for(;iter1 != c1.end() && iter2 != c2.end(); ++iter1, ++iter2)
    EXPECT_EQ(*iter1, *iter2);
  EXPECT_EQ(iter1, c1.end());
  EXPECT_EQ(iter2, c2.end());
}

template <typename C>
void print_container(const C& c) {
  for(auto&& i : c)
    std::cout << i << ' ';
  std::cout << std::endl;
}

class ListTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    test_data.resize(10);
    std::iota(test_data.begin(), test_data.end(), 0);
  }

  virtual void TearDown() {}

  stl::list<int> tc;
  std::list<int> sc;
  std::list<int> test_data;
};

TEST_F(ListTest, IsEmptyInitialized) {
  EXPECT_EQ(0, tc.size());
  EXPECT_EQ(true, tc.empty());
}

TEST_F(ListTest, Construction) {
  stl::list<int> tc2(10);
  std::list<int> sc2(10);
  test_range(sc2, tc2);
  stl::list<int> tc3(10, 1);
  std::list<int> sc3(10, 1);
  test_range(sc3, tc3);
  stl::list<int> tc4{1, 2, 3, 4};
  std::list<int> sc4{1, 2, 3, 4};
  test_range(sc4, tc4);
  stl::list<int> tc5(tc4);
  std::list<int> sc5(sc4);
  test_range(sc4, tc4);
  test_range(sc5, tc5);
  stl::list<int> tc6(::std::move(tc5));
  std::list<int> sc6(::std::move(sc5));
  test_range(sc5, tc5);
  test_range(sc6, tc6);
}

TEST_F(ListTest, EraseOperation) {
  tc.clear();
  sc.clear();
  test_range(tc, sc);
  tc = {1, 2, 3, 4, 5, 6, 4};
  sc = {1, 2, 3, 4, 5, 6, 4};
  test_range(tc, sc);
  auto iter_tc = tc.erase(tc.begin());
  auto iter_sc = sc.erase(sc.begin());
  EXPECT_EQ(*iter_tc, *iter_sc);
  test_range(tc, sc);
  tc.remove(4);
  sc.remove(4);
  test_range(tc, sc);
  tc.remove_if([](int i){ return i == 5; });
  sc.remove_if([](int i){ return i == 5; });
  test_range(tc, sc);
  iter_tc = tc.erase(iter_tc, tc.end());
  iter_sc = sc.erase(iter_sc, sc.end());
  EXPECT_EQ(*iter_tc, *iter_sc);
  test_range(tc, sc);
}

TEST_F(ListTest, Assignment) {
  stl::list<int> tc1{1, 2, 3, 4, 5};
  stl::list<int> tc2{1, 2, 3};
  std::list<int> sc1{1, 2, 3, 4, 5};
  std::list<int> sc2{1, 2, 3};
  tc1.assign({1});
  sc1.assign({1});
  test_range(sc1, tc1);
  test_range(sc2, tc2);
  tc1 = ::std::move(tc2);
  sc1 = ::std::move(sc2);
  test_range(sc1, tc1);
  test_range(sc2, tc2);
}

TEST_F(ListTest, InsertOperation) {
  tc.emplace_back(1);
  sc.emplace_back(1);
  test_range(sc, tc);
  tc.emplace_front(2);
  sc.emplace_front(2);
  test_range(sc, tc);
  for(int i = 0; i < 5; ++i) {
    tc.push_back(i);
    sc.push_back(i);
    test_range(sc, tc);
    tc.push_front(i+1);
    sc.push_front(i+1);
    test_range(sc, tc);
  }
  auto iter_tc = tc.begin();
  auto iter_sc = sc.begin();
  for(int i = 0; i < 6; ++i) {
    iter_tc++;
    iter_sc++;
  }
  auto iter_tc_r = tc.insert(iter_tc, 1);
  auto iter_sc_r = sc.insert(iter_sc, 1);
  EXPECT_EQ(std::distance(tc.begin(), iter_tc_r), std::distance(sc.begin(), iter_sc_r));
  test_range(sc, tc);
  iter_tc_r = tc.emplace(iter_tc, 1);
  iter_sc_r = sc.emplace(iter_sc, 1);
  EXPECT_EQ(std::distance(tc.begin(), iter_tc_r), std::distance(sc.begin(), iter_sc_r));
  test_range(sc, tc);
  iter_tc_r = tc.insert(iter_tc, 5, 1);
  iter_sc_r = sc.insert(iter_sc, 5, 1);
  EXPECT_EQ(std::distance(tc.begin(), iter_tc_r), std::distance(sc.begin(), iter_sc_r));
  test_range(sc, tc);
  iter_tc_r = tc.insert(iter_tc, {1, 2, 3, 4, 5});
  iter_sc_r = sc.insert(iter_sc, {1, 2, 3, 4, 5});
  EXPECT_EQ(std::distance(tc.begin(), iter_tc_r), std::distance(sc.begin(), iter_sc_r));
  test_range(sc, tc);
}

TEST_F(ListTest, AlgorithmOperation) {
  tc = {1,2,3,3,4,4,5,6,6,7};
  sc.assign(tc.begin(), tc.end());
  test_range(sc, tc);
  tc.unique();
  sc.unique();
  test_range(sc, tc);
  tc = {1,2,3,3,4,4,5,6,6,7};
  sc.assign(tc.begin(), tc.end());
  tc.unique([](int l, int r) { return r - l == 1; });
  sc.unique([](int l, int r) { return r - l == 1; });
  test_range(sc, tc);
  stl::list<int> tc1{1,3,6,8,10};
  std::list<int> sc1 {tc1.begin(), tc1.end()};
  stl::list<int> tc2{1,2,4,8,9,10,11};
  std::list<int> sc2(tc2.begin(), tc2.end());
  tc1.merge(tc2);
  sc1.merge(sc2);
  test_range(sc1, tc1);
  test_range(sc2, tc2);
  stl::list<int> tc3{2, 3, 4, 5, 6, 10};
  std::list<int> sc3(tc3.begin(), tc3.end());
  tc1.merge(tc3);
  sc1.merge(sc3);
  test_range(sc1, tc1);
  test_range(sc3, tc3);
  tc1 = {10,5,5,3,2,1};
  tc2 = {11,8,6,6,3,3,2,1};
  sc1.assign(tc1.begin(), tc1.end());
  sc2.assign(tc2.begin(),  tc2.end());
  tc1.merge(tc2, std::greater<int>());
  sc1.merge(sc2, std::greater<int>());
  test_range(sc1, tc1);
  test_range(sc2, tc2);
  tc1 = {1,2,4,1,5,4,12,3,5,3,6,7,5};
  sc1.assign(tc1.begin(), tc1.end());
  tc1.sort();
  sc1.sort();
  test_range(sc1, tc1);
  tc1.reverse();
  sc1.reverse();
  test_range(sc1, tc1);
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}