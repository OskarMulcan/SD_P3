#include "ChainingHashTable.h"
#include "hash_functions.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <chrono>
#define NUM_TESTS 50

using namespace std;

string generateKey(int size = 10){
    string result;
    static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (int i = 0; i < size; i++)
        result += charset[rand() % (sizeof(charset) - 1)];
    return result;
}

std::vector<std::string> generateDeterministicKeys(size_t n) {
    std::vector<std::string> keys;
    for (size_t i = 0; i < n; i++) {
        keys.push_back("key_" + std::to_string(i));
    }
    return keys;
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

    float percentages[] = { 0.10, 0.25, 0.33, 0.50, 0.75, 1.00};
    const std::vector<size_t> sizes = {10, 50, 100, 500, 1000, 5000, 10000, 50000};

    cout << "Percentage; Initial_size; Function; Time_Insert_Ch; Time_Insert_OA; Time_Remove_Ch; Time_Remove_OA\n";
    for(auto percentage : percentages) {
        for (auto &[name, hashFunc]: hashFunctions) {
            for (auto size: sizes) {
                auto tableCh = new ChainingHashTable<string, int>(size, hashFunc);
                auto tableOA = new OpenAddrHashTable<string, int>(size, hashFunc);
                double timerChInsert = 0;
                double timerOAInsert = 0;
                double timerChRemove = 0;
                double timerOARemove = 0;
                for (int i = 0; i < NUM_TESTS; i++) {
                    std::string key;
                    int value;
                    for (int j = 0; j < size * percentage; j++) {
                        key = generateKey();
                        value = rand() % 1000;
                        tableCh->insert(key, value);
                        tableOA->insert(key, value);
                    }

                    string key_insert = generateKey();
                    value = rand() % 1000;
                    auto start = chrono::high_resolution_clock::now();
                    tableCh->insert(key_insert, value);
                    auto stop = chrono::high_resolution_clock::now();
                    timerChInsert += chrono::duration_cast<chrono::nanoseconds>(stop - start).count();

                    start = chrono::high_resolution_clock::now();
                    tableOA->insert(key_insert, value);
                    stop = chrono::high_resolution_clock::now();
                    timerOAInsert += chrono::duration_cast<chrono::nanoseconds>(stop - start).count();

                    tableCh->remove(key_insert);
                    tableOA->remove(key_insert);


                    start = chrono::high_resolution_clock::now();
                    tableCh->remove(key);
                    stop = chrono::high_resolution_clock::now();
                    timerChRemove += chrono::duration_cast<chrono::nanoseconds>(stop - start).count();

                    start = chrono::high_resolution_clock::now();
                    tableOA->remove(key);
                    stop = chrono::high_resolution_clock::now();
                    timerOARemove += chrono::duration_cast<chrono::nanoseconds>(stop - start).count();
                }
                timerChInsert /= NUM_TESTS;
                timerOAInsert /= NUM_TESTS;
                timerChRemove /= NUM_TESTS;
                timerOARemove /= NUM_TESTS;
                cout << percentage << "; " << size << "; " << name << "; "
                     << timerChInsert << "; " << timerOAInsert << "; " << timerChRemove << "; " << timerOARemove
                     << "\n";
                delete tableCh;
                delete tableOA;
            }
        }
    }


    return 0;
}
