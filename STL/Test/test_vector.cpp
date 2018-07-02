#include <algorithm>
#include <random>
#include <vector>
#include "../vector.h"
#include "gtest/gtest.h"

template <typename C1, typename C2>
void test_range(const C1 &c1, const C2 &c2) {
  EXPECT_EQ(c1.size(), c2.size());
  for (int i = 0; i < c1.size(); ++i) EXPECT_EQ(c1[i], c2[i]);
}

class VectorTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    test_data.resize(10);
    std::iota(test_data.begin(), test_data.end(), 0);
  }

  virtual void TearDown() {}

  stl::vector<int> tv;
  std::vector<int> sv;
  std::vector<int> test_data;
};

TEST_F(VectorTest, IsEmptyInitialized) {
  EXPECT_EQ(0, tv.size());
  EXPECT_EQ(true, tv.empty());
  EXPECT_EQ(0, tv.capacity());
}

TEST_F(VectorTest, CapacityOperation) {
  EXPECT_EQ(0, tv.size());
  EXPECT_EQ(true, tv.empty());
  EXPECT_EQ(0, tv.capacity());
  EXPECT_EQ(sv.max_size(), tv.max_size());
  tv.resize(10);
  sv.resize(10);
  EXPECT_EQ(sv.size(), tv.size());
  EXPECT_EQ(false, tv.empty());
  tv.resize(5);
  sv.resize(5);
  EXPECT_EQ(sv.size(), tv.size());
  tv.resize(10, 1);
  sv.resize(10, 1);
  test_range(sv, tv);
  tv.reserve(20);
  sv.reserve(20);
  EXPECT_EQ(sv.size(), tv.size());
  EXPECT_EQ(sv.capacity(), tv.capacity());
  tv.shrink_to_fit();
  sv.shrink_to_fit();
  EXPECT_EQ(sv.size(), tv.size());
  EXPECT_EQ(sv.capacity(), tv.capacity());
}

TEST_F(VectorTest, AssignmentOperation) {
  sv = {1, 2, 3, 4, 5};
  tv = {1, 2, 3, 4, 5};
  test_range(sv, tv);

  auto tv2 = tv;
  test_range(sv, tv2);
  test_range(sv, tv);

  auto tv3 = std::move(tv);
  test_range(sv, tv3);
  EXPECT_EQ(0, tv.size());
  EXPECT_EQ(0, tv.capacity());

  sv.assign(10, 1);
  tv.assign(10, 1);
  test_range(sv, tv);

  sv.assign(test_data.begin(), test_data.end());
  tv.assign(test_data.begin(), test_data.end());
  test_range(sv, tv);

  tv.assign({1, 2, 3, 4, 5});
  sv.assign({1, 2, 3, 4, 5});
  test_range(sv, tv);
}

TEST_F(VectorTest, AccessOperation) {
  sv = {1, 2, 3, 4, 5};
  tv = {1, 2, 3, 4, 5};
  EXPECT_EQ(sv.back(), tv.back());
  EXPECT_EQ(sv.front(), tv.front());

  auto p_data_tv = tv.data();
  auto p_data_sv = sv.data();
  for (int i = 0; i < sv.size(); ++i) {
    EXPECT_EQ(sv[i], tv[i]);
    EXPECT_EQ(sv.at(i), tv.at(i));
    EXPECT_EQ(p_data_sv[i], p_data_tv[i]);
  }

  auto iter_tv = tv.begin();
  auto iter_sv = sv.begin();
  for (; iter_tv != tv.end() && iter_sv != sv.end(); ++iter_tv, ++iter_sv) {
    EXPECT_EQ(*iter_sv, *iter_tv);
  }
  EXPECT_EQ(iter_tv, tv.end());

  auto riter_tv = tv.rbegin();
  auto riter_sv = sv.rbegin();
  for (; riter_tv != tv.rend() && riter_sv != sv.rend();
       ++riter_tv, ++riter_sv) {
    EXPECT_EQ(*riter_sv, *riter_tv);
  }
  EXPECT_EQ(riter_tv, tv.rend());
}

TEST_F(VectorTest, PlacebackOperation) {
  tv.clear();
  sv.clear();
  EXPECT_EQ(sv.size(), tv.size());
  EXPECT_EQ(sv.capacity(), tv.capacity());

  for (int i = 0; i < test_data.size(); ++i) {
    tv.push_back(test_data[i]);
    tv.push_back(std::move(test_data[i]));
    sv.push_back(std::move(test_data[i]));
    sv.push_back(test_data[i]);
  }
  test_range(sv, tv);

  for (int i = test_data.size() - 1; i >= 0; --i) {
    tv.emplace_back(test_data[i]);
    sv.emplace_back(test_data[i]);
  }
  test_range(sv, tv);
}

TEST_F(VectorTest, EraseOpeartion) {
  tv.clear();
  sv.clear();
  tv.shrink_to_fit();
  sv.shrink_to_fit();
  EXPECT_EQ(sv.size(), tv.size());
  EXPECT_EQ(sv.capacity(), tv.capacity());

  tv.assign(test_data.begin(), test_data.end());
  sv.assign(test_data.begin(), test_data.end());
  EXPECT_EQ(sv.size(), tv.size());
  EXPECT_EQ(sv.back(), tv.back());
  tv.pop_back();
  sv.pop_back();
  EXPECT_EQ(sv.size(), tv.size());
  EXPECT_EQ(sv.back(), tv.back());

  auto iter_tv = tv.begin();
  auto iter_sv = sv.begin();
  iter_tv = tv.erase(iter_tv);
  iter_sv = sv.erase(iter_sv);
  EXPECT_EQ(sv.size(), tv.size());
  EXPECT_EQ(*iter_sv, *iter_tv);
  test_range(sv, tv);

  iter_tv = tv.erase(iter_tv, iter_tv + 2);
  iter_sv = sv.erase(iter_sv, iter_sv + 2);
  EXPECT_EQ(sv.size(), tv.size());
  EXPECT_EQ(*iter_tv, *iter_sv);
  test_range(sv, tv);

  iter_tv = tv.begin();
  iter_sv = sv.begin();
  while (iter_tv != tv.end() && iter_sv != sv.end()) {
    iter_tv = tv.erase(iter_tv);
    iter_sv = sv.erase(iter_sv);
    EXPECT_EQ(*iter_tv, *iter_sv);
  }
  EXPECT_EQ(sv.size(), tv.size());
}

TEST_F(VectorTest, InsertOperation) {
  tv.clear();
  sv.clear();
  tv = {0, 0, 0, 0, 0};
  sv = {0, 0, 0, 0, 0};
  auto iter_tv = tv.begin() + 3;
  auto iter_sv = sv.begin() + 3;

  iter_tv = tv.insert(iter_tv, {1, 2, 3, 4, 5});
  iter_sv = sv.insert(iter_sv, {1, 2, 3, 4, 5});
  EXPECT_EQ(*iter_sv, *iter_sv);
  test_range(sv, tv);

  iter_tv = tv.insert(tv.end(), {1, 2, 3, 4, 5});
  iter_sv = sv.insert(sv.end(), {1, 2, 3, 4, 5});
  EXPECT_EQ(*iter_sv, *iter_tv);
  test_range(sv, tv);

  tv.reserve(100);
  sv.reserve(100);
  iter_tv = tv.insert(tv.end(), {1, 2, 3, 4, 5});
  iter_sv = sv.insert(sv.end(), {1, 2, 3, 4, 5});
  EXPECT_EQ(*iter_sv, *iter_tv);
  test_range(sv, tv);

  for (int i = 0; i < test_data.size(); ++i) {
    iter_tv = tv.insert(iter_tv, test_data[i]);
    iter_sv = sv.insert(iter_sv, test_data[i]);
    EXPECT_EQ(*iter_sv, *iter_tv);
  }
  test_range(sv, tv);
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
