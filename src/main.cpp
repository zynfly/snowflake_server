#include <iostream>
#include <bitset>
#include <vector>
#include <list>
#include <thread>
#include <atomic>


#include "snowflake.h"

#define THREAD_NUM 2
#define THREAD_LOOP 10000

int main(int argc, char **argv)
{
    std::cout << std::time(nullptr) << std::endl;
    SnowFlake s(1535731200000, 1, 1, 41, 5, 6, 11);

    std::vector<uint64_t> v(THREAD_NUM * THREAD_LOOP);

    std::vector<std::thread> vt;

    std::atomic_uint count(0);


    for (int i = 0; i < THREAD_NUM; ++i) {
        vt.emplace_back(
                [&]() {
                    int i = THREAD_LOOP;
                    while (i--) {
                        v[count++] = s.nextId();
                    }
                }
        );
    }

    for (int j = 0; j < vt.size(); ++j) {
        vt[j].join();
    }

    std::cout << "size:" << v.size() << std::endl;
    for (int k = 0; k < v.size() - 1; ++k) {
        if (v[k] > v[k+1])
        {
            std::cout << "err in queue" << std::endl;
        }
//        for (int i = k + 1; i < v.size(); ++i) {
//            if (v[k] == v[i]) {
//                std::cout << "error:" << std::bitset<64>(v[k]) << ":" << k << ":" << i << std::endl;
//            }
//        }
    }
    return 0;
}

