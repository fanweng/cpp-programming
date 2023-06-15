#include<iostream>
#include<vector>
#include<random>
#include<mutex>
#include<thread>
#include<atomic>
#include<future>

constexpr long long SIZE = 100000000;

int main() {
    std::vector<int> values;
    values.reserve(SIZE);

    std::random_device seed;
    std::mt19937 engine(seed());
    std::uniform_int_distribution<> uniformDist(1, 10);
    for (long long i = 0; i < SIZE; ++i) {
        values.push_back(uniformDist(engine));
    }

    /* Method 1: Single thread summation using a for-loop */
    unsigned long long sum1 = 0;
    for (auto n : values) sum1 += n;
    /******************************************************/

    /* Method 2: Single thread summation using STL function */
    unsigned long long sum2 = std::accumulate(values.begin(), values.end(), 0);
    /********************************************************/

    /* Method 3: Multithreaded summation using lock_guard */
    unsigned long long sum3 = 0;
    std::mutex mtx3;
    // capture mtx3 by reference
    auto rangedSum3 = [&](unsigned long long& sum, const std::vector<int>& v, unsigned long long start, unsigned long long end) {
        for (auto i = start; i < end; ++i) {
            std::lock_guard<std::mutex> myLock3(mtx3);
            sum += v[i];
        }
    };
    std::thread t31(rangedSum3, std::ref(sum3), std::ref(values), 0, 25000000);
    std::thread t32(rangedSum3, std::ref(sum3), std::ref(values), 25000000, 50000000);
    std::thread t33(rangedSum3, std::ref(sum3), std::ref(values), 50000000, 75000000);
    std::thread t34(rangedSum3, std::ref(sum3), std::ref(values), 75000000, SIZE);
    t31.join();
    t32.join();
    t33.join();
    t34.join();
    /******************************************************/

    /* Method 4: Multithreaded summation using atomic */
    std::atomic<unsigned long long> sum4 {};
    auto rangedSum4 = [](std::atomic<unsigned long long>& sum, const std::vector<int>& v, unsigned long long start, unsigned long long end) {
        for (auto i = start; i < end; ++i) {
            // 1) default memory model for atomic is sequential consistency
            // sum += v[i];
            // 2) relaxed semantic is the weakest memory model
            sum.fetch_add(v[i], std::memory_order_relaxed);
        }
    };
    std::thread t41(rangedSum4, std::ref(sum4), std::ref(values), 0, 25000000);
    std::thread t42(rangedSum4, std::ref(sum4), std::ref(values), 25000000, 50000000);
    std::thread t43(rangedSum4, std::ref(sum4), std::ref(values), 50000000, 75000000);
    std::thread t44(rangedSum4, std::ref(sum4), std::ref(values), 75000000, SIZE);
    t41.join();
    t42.join();
    t43.join();
    t44.join();
    /******************************************************/

    /* Method 5: Multithreaded summation using lock_guard and local variable */
    unsigned long long sum5 = 0;
    std::mutex mtx5;
    // capture mtx5 by reference
    auto rangedSum5 = [&](unsigned long long& sum, const std::vector<int>& v, unsigned long long start, unsigned long long end) {
        unsigned long long tmpSum = 0;
        for (auto i = start; i < end; ++i) {
            tmpSum += v[i];
        }
        std::lock_guard<std::mutex> myLock5(mtx5);
        sum += tmpSum;
    };
    std::thread t51(rangedSum5, std::ref(sum5), std::ref(values), 0, 25000000);
    std::thread t52(rangedSum5, std::ref(sum5), std::ref(values), 25000000, 50000000);
    std::thread t53(rangedSum5, std::ref(sum5), std::ref(values), 50000000, 75000000);
    std::thread t54(rangedSum5, std::ref(sum5), std::ref(values), 75000000, SIZE);
    t51.join();
    t52.join();
    t53.join();
    t54.join();
    /*************************************************************************/


    /* Method 6: Multithreaded summation using atomic and local variable */
    std::atomic<unsigned long long> sum6 {};
    auto rangedSum6 = [](std::atomic<unsigned long long>& sum, const std::vector<int>& v, unsigned long long start, unsigned long long end) {
        unsigned long long tmpSum = 0;
        for (auto i = start; i < end; ++i) {
            tmpSum += v[i];
        }
        sum.fetch_add(tmpSum, std::memory_order_relaxed);
    };
    std::thread t61(rangedSum6, std::ref(sum6), std::ref(values), 0, 25000000);
    std::thread t62(rangedSum6, std::ref(sum6), std::ref(values), 25000000, 50000000);
    std::thread t63(rangedSum6, std::ref(sum6), std::ref(values), 50000000, 75000000);
    std::thread t64(rangedSum6, std::ref(sum6), std::ref(values), 75000000, SIZE);
    t61.join();
    t62.join();
    t63.join();
    t64.join();
    /*********************************************************************/

    /* Method 7: Multithreaded summation using atomic and thread_local data */
    std::atomic<unsigned long long> sum7 {};
    thread_local unsigned long long tmpSum7 = 0;
    auto rangedSum7 = [](std::atomic<unsigned long long>& sum, const std::vector<int>& v, unsigned long long start, unsigned long long end) {
        for (auto i = start; i < end; ++i) {
            tmpSum7 += v[i];
        }
        sum.fetch_add(tmpSum7, std::memory_order_relaxed);
    };
    std::thread t71(rangedSum7, std::ref(sum7), std::ref(values), 0, 25000000);
    std::thread t72(rangedSum7, std::ref(sum7), std::ref(values), 25000000, 50000000);
    std::thread t73(rangedSum7, std::ref(sum7), std::ref(values), 50000000, 75000000);
    std::thread t74(rangedSum7, std::ref(sum7), std::ref(values), 75000000, SIZE);
    t71.join();
    t72.join();
    t73.join();
    t74.join();
    /************************************************************************/

    /* Method 8: Multithreaded summation using task and local variable */
    unsigned long long sum8 = 0;
    auto rangedSum8 = [](std::promise<unsigned long long>&& prom, const std::vector<int>& v, unsigned long long start, unsigned long long end) {
        unsigned long long tmpSum = 0;
        for (auto i = start; i < end; ++i) {
            tmpSum += v[i];
        }
        prom.set_value(tmpSum);
    };

    std::promise<unsigned long long> prom1;
    std::promise<unsigned long long> prom2;
    std::promise<unsigned long long> prom3;
    std::promise<unsigned long long> prom4;
    auto fut1 = prom1.get_future();
    auto fut2 = prom2.get_future();
    auto fut3 = prom3.get_future();
    auto fut4 = prom4.get_future();
    std::thread t81(rangedSum8, std::move(prom1), std::ref(values), 0, 25000000);
    std::thread t82(rangedSum8, std::move(prom2), std::ref(values), 25000000, 50000000);
    std::thread t83(rangedSum8, std::move(prom3), std::ref(values), 50000000, 75000000);
    std::thread t84(rangedSum8, std::move(prom4), std::ref(values), 75000000, SIZE);
    sum8 = fut1.get() + fut2.get() + fut3.get() + fut4.get();
    t81.join();
    t82.join();
    t83.join();
    t84.join();
    /*******************************************************************/

    return 0;
}
