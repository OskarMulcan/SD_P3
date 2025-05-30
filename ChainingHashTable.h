#ifndef P3_CHAININGHASHTABLE_H
#define P3_CHAININGHASHTABLE_H
#pragma once

#include <iostream>
#include <list>
#include <string>
#include <utility>
#include <functional>
#include "hash_functions.h"
#include "HashTable.h"
#include "OpenAddrHashTable.h"

template<typename K, typename V>
class ChainingHashTable : protected HashTable<K, V>{
private:
    struct Entry {
        K key;
        V value;
        EntryState state = EntryState::EMPTY;

    };

    std::list<Entry>* table;
    size_t capacity;
    size_t min_capacity;
    size_t size;
    std::function<size_t(const K&, size_t)> hasher;

    void rehash_up() override {
        size_t new_capacity = next_prime(capacity * 2);
        std::list<Entry>* new_table = new std::list<Entry>[new_capacity];

        for (size_t i = 0; i < capacity; i++) {
            for (const Entry& e : table[i]) {
                size_t index = hasher(e.key, capacity) % new_capacity;
                new_table[index].push_back(e);
            }
        }
        delete[] table;
        table = new_table;
        capacity = new_capacity;
    }

    void rehash_down() override {
        if (capacity <= min_capacity) return;

        size_t new_capacity = previous_prime(capacity / 2);
        if (new_capacity < min_capacity) {
            new_capacity = min_capacity;
        }

        std::list<Entry>* new_table = new std::list<Entry>[new_capacity];

        for (size_t i = 0; i < capacity; i++) {
            for (const Entry& e : table[i]) {
                size_t index = hasher(e.key, capacity) % new_capacity;
                new_table[index].push_back(e);
            }
        }
        delete[] table;
        table = new_table;
        capacity = new_capacity;
    }

public:
    explicit ChainingHashTable(size_t initial_capacity, std::function<size_t(const K&, size_t)> hashFunc)
            : capacity(next_prime(initial_capacity)),
              size(0), min_capacity(next_prime(initial_capacity)),
              hasher(hashFunc){
        table = new std::list<Entry>[capacity];
    }

    ~ChainingHashTable() {
        delete[] table;
    }

    void insert(const K& key, const V& value) override {
        if ((size + 1) * 2 > capacity)
            rehash_up();

        size_t index = hasher(key, capacity) % capacity;

        for (Entry& e : table[index]) {
            if (e.key == key) {
                e.value = value;
                return;
            }
        }

        table[index].push_back({key, value});
        size++;
    }

    bool remove(const K& key) override {
        size_t index = hasher(key, capacity) % capacity;
        for (auto it = table[index].begin(); it != table[index].end(); it++) {
            if (it->key == key) {
                table[index].erase(it);
                size--;
                if (size < capacity / 4) {
                    rehash_down();
                }
                return true;
            }
        }
        return false;
    }


    V* getValue(const K& key) const override {
        size_t index = hasher(key, capacity) % capacity;
        for (Entry& e : table[index]) {
            if (e.key == key)
                return &e.value;
        }
        return nullptr;
    }

    void print() const override {
        for (size_t i = 0; i < capacity; i++) {
            std::cout << "[" << i << "]: ";
            for (const Entry& e : table[i])
                std::cout << "(" << e.key << "," << e.value << ") ";
            std::cout << "\n";
        }
    }


};

#endif // P3_CHAININGHASHTABLE
