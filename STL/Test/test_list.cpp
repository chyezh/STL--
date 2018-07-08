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
  stl::list<int> sc2(10);
  test_range(sc2, tc2);
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}