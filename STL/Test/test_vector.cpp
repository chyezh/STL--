#include <algorithm>
#include <vector>
#include "../vector.h"
#include "gtest/gtest.h"

class VectorTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    test_data.resize(10000);
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
  tv.reserve(100);
  sv.reserve(100);
  EXPECT_EQ(sv.size(), tv.size());
  EXPECT_EQ(sv.capacity(), tv.capacity());
  tv.shrink_to_fit();
  sv.shrink_to_fit();
  EXPECT_EQ(sv.size(), tv.size());
  EXPECT_EQ(sv.capacity(), tv.capacity());
  tv.clear();
  sv.clear();
  EXPECT_EQ(sv.size(), tv.size());
  EXPECT_EQ(sv.capacity(), tv.capacity());
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
