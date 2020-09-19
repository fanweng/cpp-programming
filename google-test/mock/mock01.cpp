#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;
using ::testing::Return;

class DataBaseConnect {

public:
    virtual bool login(string username, string password) {
        return true;
    }

    virtual bool login2(string username, string password) {
        return true;
    }

    virtual bool logout(string username) {
        return true;
    }

    virtual int fetchRecord() {
        return -1;
    }
};

class MockDB : public DataBaseConnect {

public:
    MOCK_METHOD0(fetchRecord, int());
    MOCK_METHOD1(logout, bool(string username));
    MOCK_METHOD2(login, bool(string username, string password));
    MOCK_METHOD2(login2, bool(string username, string password));
};

class MyDataBase {

public:
    MyDataBase(DataBaseConnect &_dbC) : dbC(_dbC) {}

    ~MyDataBase() {}

    int Init(string username, string password) {
        if (dbC.login(username, password) != true) {
            if (dbC.login2(username, password) != true) {
                cout << "login failed for the 2nd time" << endl;
                return -1;
            }
            else {
                cout << "login succeeded at the 2nd attemp" << endl;
                return 0;
            }
        }
        else {
            cout << "login succeeded" << endl;
            return 0;
        }
    }

private:
    DataBaseConnect &dbC;
};

TEST(MyDBTest, loginTest) {

    MockDB mdb;
    MyDataBase db(mdb);

    // EXPECT_CALL(mdb, login(_, _))   // "_" means anything input is OK
    EXPECT_CALL(mdb, login("mike", "12345"))
    .Times(1)
    .WillOnce(Return(true));

    /* ON_CALL allows the login2() may not be called */
    ON_CALL(mdb, login2(_,_)).WillByDefault(Return(true));

    int ret = db.Init("mike", "12345");
    EXPECT_EQ(ret, 0);
}

int main(int argc, char **argv) {

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}