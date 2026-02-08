#ifndef RANDOM_STREAM_GEN_H
#define RANDOM_STREAM_GEN_H


#include <vector>
#include <string>
#include <random>

class RandomStreamGen {
private:
    std::vector<std::string> stream;
    std::mt19937 rng;
    const std::string alphabet =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789-";

public:
    explicit RandomStreamGen(const size_t streamSize, const uint32_t seed=42) :
        rng(seed)
    {
        std::uniform_int_distribution<int> lenDist(1, 30);
        std::uniform_int_distribution<int> charDist(0, alphabet.size() - 1);

        for (size_t i = 0; i < streamSize; ++i) {
            const int len = lenDist(rng);
            std::string s;
            s.reserve(len);

            for (int j = 0; j < len; ++j) {
                s.push_back(alphabet[charDist(rng)]);
            }
            stream.push_back(s);
        }
    }

    [[nodiscard]]
    std::string getRandomString(const int maxSize=10)  {
        std::uniform_int_distribution<int> lenDist(1, maxSize);
        std::uniform_int_distribution<int> charDist(0, alphabet.size() - 1);

        const int len = lenDist(rng);
        std::string s;
        s.reserve(len);

        for (int j = 0; j < len; ++j) {
            s.push_back(alphabet[charDist(rng)]);
        }
        return s;
    }

    [[nodiscard]]
    const std::vector<std::string>& getStream() const {
        return stream;
    }
};


#endif
