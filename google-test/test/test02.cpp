#include <iostream>
#include <gtest/gtest.h>

using namespace std;

class Account {
public:
    Account(int val) : balance_(val) {}
    
    ~Account() {}

    int getBalance() {
        return balance_;
    }

    int withdraw(int val) {
        balance_ -= val;
    }

private:
    int balance_;
};

class AccountTest : public ::testing::Test {
public:
    void SetUp() {
        // code here will be called before each TEST_F
        cout << "-> SetUp" << endl;

        // Arrange
        myAccount.withdraw(10);
    }

    void TearDown() {
        // code here will be called after each TEST_F
        cout << "-> TearDown" << endl;
    }

protected:
    Account myAccount{100}; 
};

TEST_F(AccountTest, Subtest01) {

    ASSERT_EQ(myAccount.getBalance(), 90);
}

TEST_F(AccountTest, Subtest02) {

    // Act
    myAccount.withdraw(5);

    // Assert
    ASSERT_EQ(myAccount.getBalance(), 85);
}

int main(int argc, char **argv) {

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}