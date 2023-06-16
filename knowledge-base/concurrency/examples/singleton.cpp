#include<iostream>
#include<mutex>
#include<thread>
#include<atomic>

/* Method 1: Thread-safe but heavy lock penalty */
class Singleton1 {
public:
    static Singleton1* getInstance() {
        std::lock_guard<std::mutex> lock1(mtx1); // lock no matter if instance is created
        if (!instance) {
            instance = new Singleton1();
        }
        return instance;
    }

private:
    Singleton1() = default;
    ~Singleton1() = default;
    Singleton1(const Singleton1&) = delete;
    Singleton1& operator=(const Singleton1&) = delete;

    static Singleton1* instance;
    static std::mutex mtx1;
};
Singleton1* Singleton1::instance = nullptr;
std::mutex Singleton1::mtx1;
/************************************************/

/* Method 2 Double-checked locking: efficient but not thread-safe */
/*
 * C++ runtime can't guarantee the sequence of three-step of new(), Step3 may be before Step2.
 * Image Thread1 is in progress of new(), "instance" points to a non-initialized singleton,
 * Thread2 will pass Checkpoint1 and may use the non-initialized singleton with undefined behavior.
*/
class Singleton2 {
public:
    static Singleton2* getInstance() {
        if (!instance) { // check1
            std::lock_guard<std::mutex> lock2(mtx2); // lock
            if (!instance) { // check2
                instance = new Singleton2(); // 1. allocate memory; 2. initialize class object; 3. set the pointer to fully initialized object
            }
        }
        return instance;
    }

private:
    Singleton2() = default;
    ~Singleton2() = default;
    Singleton2(const Singleton2&) = delete;
    Singleton2& operator=(const Singleton2&) = delete;

    static Singleton2* instance;
    static std::mutex mtx2;
};
Singleton2* Singleton2::instance = nullptr;
std::mutex Singleton2::mtx2;
/******************************************************************/

/* Method 3 Meyers Singleton: Thread-safe and efficient */
class Singleton3 {
public:
    static Singleton3* getInstance() {
        // C++11 guarantees that static variables with block scope will be init in thread-safe way
        static Singleton3 instance;
        return &instance;
    }

private:
    Singleton3() = default;
    ~Singleton3() = default;
    Singleton3(const Singleton3&) = delete;
    Singleton3& operator=(const Singleton3&) = delete;
};
/********************************************************/

/* Method 4 Double-checked with atomic: Thread-safe */
class Singleton4 {
public:
    static Singleton4* getInstance() {
        Singleton4* instance = insAtomic.load(std::memory_order_acquire);
        if (!instance) { // check1
            std::lock_guard<std::mutex> lock4(mtx4); // lock
            instance = insAtomic.load(std::memory_order_relaxed);
            if (!instance) { // check2
                instance = new Singleton4();
                insAtomic.store(instance, std::memory_order_release); // ensure the insAtomic always stores an initialized object
            }
        }
        return instance;
    }

private:
    Singleton4() = default;
    ~Singleton4() = default;
    Singleton4(const Singleton4&) = delete;
    Singleton4& operator=(const Singleton4&) = delete;

    static std::atomic<Singleton4*> insAtomic;
    static std::mutex mtx4;
};

std::atomic<Singleton4*> Singleton4::insAtomic;
std::mutex Singleton4::mtx4;
/******************************************************************/

int main() {
    std::cout << "Singleton1::getInstance(): " << Singleton1::getInstance() << std::endl; // Performance No.3
    std::cout << "Singleton3::getInstance(): " << Singleton3::getInstance() << std::endl; // Performance No.1
    std::cout << "Singleton4::getInstance(): " << Singleton4::getInstance() << std::endl; // Performance No.2
    return 0;
}
