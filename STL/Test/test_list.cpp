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
  /*
  stl::list<int> tc6(::std::move(tc5));
  std::list<int> sc6(::std::move(sc5));
  test_range(sc5, tc5);
  test_range(sc6, tc6);
  */
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
  /*
  tc1 = ::std::move(tc2);
  sc1 = ::std::move(sc2);
  test_range(sc1, tc1);
  test_range(sc2, tc2);
  */
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

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}