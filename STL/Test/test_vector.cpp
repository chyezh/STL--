#include <algorithm>
#include <random>
#include <vector>
#include "../vector.h"
#include "gtest/gtest.h"


class VectorTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    test_data.resize(100);
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
  for (int i = 0; i < sv.size(); ++i) EXPECT_EQ(sv[i], tv[i]);
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
  EXPECT_EQ(sv.size(), tv.size());
  for (int i = 0; i < sv.size(); ++i) EXPECT_EQ(sv[i], tv[i]);

  auto tv2 = tv;
  EXPECT_EQ(sv.size(), tv2.size());
  for (int i = 0; i < sv.size(); ++i) {
    EXPECT_EQ(sv[i], tv[i]);
    EXPECT_EQ(sv[i], tv2[i]);
  }

  auto tv3 = std::move(tv);
  EXPECT_EQ(sv.size(), tv3.size());
  EXPECT_EQ(0, tv.size());
  EXPECT_EQ(0, tv.capacity());
  for (int i = 0; i < sv.size(); ++i) EXPECT_EQ(sv[i], tv3[i]);

  sv.assign(10, 1);
  tv.assign(10, 1);
  EXPECT_EQ(sv.size(), tv.size());
  for (int i = 0; i < sv.size(); ++i) EXPECT_EQ(sv[i], tv[i]);

  sv.assign(test_data.begin(), test_data.end());
  tv.assign(test_data.begin(), test_data.end());
  EXPECT_EQ(sv.size(), tv.size());
  for (int i = 0; i < sv.size(); ++i) EXPECT_EQ(sv[i], tv[i]);

  tv.assign({1, 2, 3, 4, 5});
  sv.assign({1, 2, 3, 4, 5});
  EXPECT_EQ(sv.size(), tv.size());
  for (int i = 0; i < sv.size(); ++i) EXPECT_EQ(sv[i], tv[i]);
}

TEST_F(VectorTest, AccessOperation) {
  sv = {1,2,3,4,5};
  tv = {1,2,3,4,5};
  EXPECT_EQ(sv.back(), tv.back());
  EXPECT_EQ(sv.front(), tv.front());

  auto p_data_tv = tv.data();
  auto p_data_sv = sv.data();
  for(int i = 0; i < sv.size(); ++i) {
    EXPECT_EQ(sv[i], tv[i]);
    EXPECT_EQ(sv.at(i), tv.at(i));
    EXPECT_EQ(p_data_sv[i], p_data_tv[i]);
  }

  auto iter_tv = tv.begin();
  auto iter_sv = sv.begin();
  for(;iter_tv != tv.end() && iter_sv != sv.end(); ++iter_tv, ++iter_sv) {
    EXPECT_EQ(*iter_sv, *iter_tv);
  }
  EXPECT_EQ(iter_tv, tv.end());

  auto riter_tv = tv.rbegin();
  auto riter_sv = sv.rbegin();
  for(;riter_tv != tv.rend() && riter_sv != sv.rend(); ++riter_tv, ++riter_sv) {
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
  for (int i = 0; i < sv.size(); ++i) EXPECT_EQ(sv[i], tv[i]);

  for (int i = test_data.size() - 1; i >= 0; --i) {
    tv.emplace_back(test_data[i]);
    sv.emplace_back(test_data[i]);
  }
  for (int i = 0; i < sv.size(); ++i) EXPECT_EQ(sv[i], tv[i]);
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
  for(int i = 0; i < tv.size(); ++i)
    EXPECT_EQ(sv[i], tv[i]);
  
  iter_tv = tv.erase(iter_tv, iter_tv + 10);
  iter_sv = sv.erase(iter_sv, iter_sv + 10);
  EXPECT_EQ(sv.size(), tv.size());
  EXPECT_EQ(*iter_tv, *iter_sv);
  for(int i = 0; i < sv.size(); ++i)
    EXPECT_EQ(sv[i], tv[i]);

  iter_tv = tv.begin();
  iter_sv = sv.begin();
  while(iter_tv != tv.end() && iter_sv != sv.end()) {
    iter_tv = tv.erase(iter_tv);
    iter_sv = sv.erase(iter_sv);
    EXPECT_EQ(*iter_tv, *iter_sv);
  }
  EXPECT_EQ(sv.size(),tv.size());
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
