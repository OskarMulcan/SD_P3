#include "ChainingHashTable.h"
#include "hash_functions.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#define NUM_TESTS 100

using namespace std;

string generateKey(int size = 10){
    string result;
    static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (int i = 0; i < size; ++i)
        result += charset[rand() % (sizeof(charset) - 1)];
    return result;
}

int main() {
    srand((time(NULL)));

    using HashFunction = std::function<size_t(const std::string&, size_t)>;

    std::vector<std::pair<std::string, HashFunction>> hashFunctions = {
            { "AdditiveHash", [](const std::string& key, size_t cap) { return AdditiveHash()(key, cap); } },
            { "DJB2Hash", [](const std::string& key, size_t cap) { return DJB2Hash()(key, cap); } },
            { "FibonacciHash", [](const std::string& key, size_t cap) { return FibonacciHash()(key, cap); } },
            { "MultiplicativeHash", [](const std::string& key, size_t cap) { return MultiplicativeHash()(key, cap); } }
    };

    int size_divisors[] = {4, 2, 1};

    for (int divisor : size_divisors){
        const std::vector<size_t> sizes = {10, 50, 100, 500, 1000, 5000, 10000, 50000};
        for (auto& [name, hashFunc] : hashFunctions) {
            for (size_t size : sizes) {
                auto tableCh = new ChainingHashTable<string, int>(size, hashFunc);
                auto tableOA = new OpenAddrHashTable<string, int>(size, hashFunc);
                double timerCh = 0;
                double timerOA = 0;
                for (int i = 0; i < NUM_TESTS; i++) {
                    std::string key = generateKey();
                    int value = rand() % 1000;
                    for (int j = 0; j < size/divisor; j++) {
                        tableCh->insert(key, value);
                        tableOA->insert(key, value);
                    }
                    key = generateKey();
                    value = rand() % 1000;
                    auto start = chrono::high_resolution_clock::now();
                    tableCh->insert(key, value);
                    auto stop = chrono::high_resolution_clock::now();
                    timerCh += chrono::duration_cast<chrono::nanoseconds>(stop - start).count();

                    start = chrono::high_resolution_clock::now();
                    tableOA->insert(key, value);
                    stop = chrono::high_resolution_clock::now();
                    timerOA += chrono::duration_cast<chrono::nanoseconds>(stop - start).count();

                }
                timerCh /= 100;
                timerOA /= 100;
                cout << size << " " << divisor << " " << name << " " << timerCh << " " << timerOA << "\n";
                delete tableCh;
                delete tableOA;
            }
        }
    }

    return 0;
}
