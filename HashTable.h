#ifndef P3_HASHTABLE_H
#define P3_HASHTABLE_H
#pragma once

#include <list>
#include <functional>

enum class EntryState { EMPTY, OCCUPIED, DELETED };

inline bool is_prime(size_t n) {
    if (n < 2) return false;
    for (size_t i = 2; i * i <= n; i++)
        if (n % i == 0) return false;
    return true;
}

inline size_t next_prime(size_t n) {
    while (!is_prime(n)) n++;
    return n;
}

inline size_t previous_prime(size_t n) {
    if (n <= 2) return 2;
    do {
        n--;
    } while (n >= 2 && !is_prime(n));
    return n;
}

template<typename K, typename V>
class HashTable {
protected:
    struct Entry {
        K key;
        V value;
        EntryState state = EntryState::EMPTY;
    };

    virtual void rehash_up() = 0;
    virtual void rehash_down() = 0;
public:
    HashTable() = default;
    ~HashTable() = default;

    virtual void insert(const K& key, const V& value) = 0;
    virtual bool remove(const K& key) = 0;
    virtual V* getValue(const K& key) const = 0;
    virtual void print() const = 0;

};


#endif //P3_HASHTABLE_H
