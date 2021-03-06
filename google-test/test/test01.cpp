#include <iostream>
#include <gtest/gtest.h>

using namespace std;

TEST(TestSuite01, Subtest01) {

    EXPECT_EQ(5, 8);    // non-fatal failure, continue to next assertion
    cout << "-> Subtest01 continues" << endl;
    ASSERT_EQ(1, 2);    // fatal failure, abort, jump to Test02
    cout << "-> Subtest01 aborts from the above line" << endl;
    ASSERT_EQ(1, 1);
}

TEST(TestSuite01, Subtest02) {

    // Arrange
    int base = 12;
    int inc = 3;
    int result = 0;

    // Act
    result = base + inc;

    // Assert
    ASSERT_EQ(result, 15);
}

int main(int argc, char **argv) {

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}