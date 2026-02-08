#ifndef HASH_FUNC_GEN_H
#define HASH_FUNC_GEN_H

#include <string>
#include <utility>

#include "random_stream_gen.h"


class HashFunc {
private:
    std::string solt;

public:
    explicit HashFunc(std::string solt) : solt(std::move(solt)) {}

    size_t operator() (const std::string& key) const {
        return std::hash<std::string>()(key + solt);
    }
};


class HashFuncGen {
private:
    RandomStreamGen streamGen{0};

public:
    HashFunc operator() () {
        return HashFunc(streamGen.getRandomString());
    }
};

#endif
