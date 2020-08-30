#include <gtest/gtest.h>

using namespace std;

TEST(Test01, testcase01) {

    EXPECT_EQ(1, 2);
    ASSERT_EQ(1, 1);
}

int main(int argc, char **argv) {

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}