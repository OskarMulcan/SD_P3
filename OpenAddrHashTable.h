#ifndef P3_OPENADDRHASHTABLE_H
#define P3_OPENADDRHASHTABLE_H
#pragma once

#include "HashTable.h"
#include <iostream>
#include <string>
#include <utility>
#include <stdexcept>

template<typename K, typename V>
class OpenAddrHashTable : protected HashTable<K, V> {
private:
    struct Entry {
        K key;
        V value;
        EntryState state = EntryState::EMPTY;
    };

    Entry* table;
    size_t capacity;
    size_t min_capacity;
    size_t size;
    std::function<size_t(const K&, size_t)> hasher;

    void rehash_up() override {
        size_t new_capacity = next_prime(capacity * 2);
        Entry* old_table = table;
        size_t old_capacity = capacity;

        table = new Entry[new_capacity];
        capacity = new_capacity;
        size = 0;

        for (size_t i = 0; i < old_capacity; i++) {
            if (old_table[i].state == EntryState::OCCUPIED) {
                insert(old_table[i].key, old_table[i].value);
            }
        }

        delete[] old_table;
    }

    void rehash_down() override {
        if (capacity <= min_capacity) return;

        size_t new_capacity = previous_prime(capacity / 2);
        if (new_capacity < min_capacity) {
            new_capacity = min_capacity;
        }

        Entry* old_table = table;
        size_t old_capacity = capacity;

        table = new Entry[new_capacity];
        capacity = new_capacity;
        size = 0;

        for (size_t i = 0; i < old_capacity; i++) {
            if (old_table[i].state == EntryState::OCCUPIED) {
                insert(old_table[i].key, old_table[i].value);
            }
        }

        delete[] old_table;
    }

    size_t probe(const K& key, size_t i) const {
        return (hasher(key, capacity) + i) % capacity;
    }

public:
    explicit OpenAddrHashTable(size_t initial_capacity, std::function<size_t(const K&, size_t)> hashFunc)
            : capacity(next_prime(initial_capacity)), size(0), min_capacity(next_prime(initial_capacity)),
              hasher(std::move(hashFunc)) {
        table = new Entry[capacity];
    }

    ~OpenAddrHashTable() {
        delete[] table;
    }

    void insert(const K& key, const V& value) override {
        if ((size + 1) * 2 > capacity) {
            rehash_up();
        }
        for (size_t i = 0; i < capacity; i++) {
            size_t index = probe(key, i);
            if (table[index].state == EntryState::EMPTY || table[index].state == EntryState::DELETED) {
                table[index] = { key, value, EntryState::OCCUPIED };
                size++;
                return;
            } else if (table[index].state == EntryState::OCCUPIED && table[index].key == key) {
                table[index].value = value;
                return;
            }
        }
        throw std::overflow_error("HashTable is full");
    }

    bool remove(const K& key) override {
        for (size_t i = 0; i < capacity; i++) {
            size_t index = probe(key, i);
            if (table[index].state == EntryState::EMPTY) {
                return false;
            } else if (table[index].state == EntryState::OCCUPIED && table[index].key == key) {
                table[index].state = EntryState::DELETED;
                size--;
                if (size < capacity / 4 && capacity > min_capacity)
                    rehash_down();
                return true;
            }
        }
        return false;
    }

    V* getValue(const K& key) const override {
        for (size_t i = 0; i < capacity; i++) {
            size_t index = probe(key, i);
            if (table[index].state == EntryState::EMPTY) return nullptr;
            if (table[index].state == EntryState::OCCUPIED && table[index].key == key)
                return &table[index].value;
        }
        return nullptr;
    }

    void print() const override {
        for (size_t i = 0; i < capacity; i++) {
            std::cout << "[" << i << "]: ";
            if (table[i].state == EntryState::OCCUPIED)
                std::cout << "(" << table[i].key << "," << table[i].value << ")";
            std::cout << "\n";
        }
    }
};

#endif //P3_OPENADDRHASHTABLE_H
