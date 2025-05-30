#ifndef P3_HASH_FUNCTIONS_H
#define P3_HASH_FUNCTIONS_H

#include "ChainingHashTable.h"
#include <math.h>

struct AdditiveHash {
    size_t operator()(const std::string& key, size_t capacity) const {
        size_t sum = 0;
        for (char c : key) sum += c;
        return sum;
    }
};

struct MultiplicativeHash {
    size_t operator()(const std::string& key, size_t capacity) const {
        const double A = 0.6180339887;
        double frac = 0;
        for (char c : key){
            frac += fmod(c*A, 1.0);
        }
        return static_cast<size_t>(frac * capacity);
    }
};


struct DJB2Hash {
    size_t operator()(const std::string& key, size_t capacity) const {
        size_t hash = 5381;
        for (char c : key)
            hash = hash * 33 + c;
        return hash;
    }
};

struct FibonacciHash {
    size_t operator()(const std::string& key, size_t capacity) const {
        size_t intKey = 0;
        for (char c : key)
            intKey = intKey * 31 + static_cast<unsigned char>(c);
        const double A = 0.6180339887;
        return static_cast<size_t>(capacity * fmod(intKey * A, 1.0));
    }
};


#endif //P3_HASH_FUNCTIONS_H
