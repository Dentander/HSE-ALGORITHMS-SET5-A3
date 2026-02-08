#ifndef HYPERLOGLOG_H
#define HYPERLOGLOG_H

#include <string>
#include <utility>
#include <vector>

#include "hash_func_gen.h"


class AHyperLogLog {
public:
    virtual ~AHyperLogLog() = default;

    virtual void add(const std::string& line) = 0;
    [[nodiscard]] virtual double uniqueCount() const = 0;
};


class HyperLogLog : public AHyperLogLog {
private:
    std::vector<char> registers_;
    HashFunc hashFunc_;
    size_t bitsCount_;
    double alpha_;

public:
    HyperLogLog(const size_t bitsCount, HashFunc hashFunc) :
        registers_(1 << bitsCount, 0),
        hashFunc_(std::move(hashFunc)),
        bitsCount_(bitsCount)
    {
        if (bitsCount == 1) { alpha_ = 0.3512; }
        else if (bitsCount == 2) { alpha_ = 0.5324; }
        else if (bitsCount == 4) { alpha_ = 0.637; }
        else if (bitsCount == 5) { alpha_ = 0.697; }
        else if (bitsCount == 6) { alpha_ = 0.709; }
        else {
            alpha_ = 0.7213 / (1 + 1.079 / static_cast<double>(registers_.size()));
        }
    }

    void add(const std::string& line) override {
        size_t hashed = hashFunc_(line);
        const size_t offset = (sizeof(size_t) * 8 - bitsCount_);
        const size_t index = hashed >> offset;
        hashed = (hashed << bitsCount_) >> bitsCount_;

        size_t leadingZeroes = offset + 1;
        while (hashed) {
            hashed >>= 1;
            --leadingZeroes;
        }

        registers_[index] = std::max<char>(registers_[index], leadingZeroes);
    }

    // OLD

    // [[nodiscard]]
    // double uniqueCount() const override {
    //     const double numerator = alpha_ * static_cast<double>(registers_.size() * registers_.size());
    //     double denominator = 0;
    //     for (const char reg : registers_) {
    //         denominator += 1.0 / (1 << reg);
    //     }
    //     return numerator / denominator;
    // }

    // NEW

    [[nodiscard]]
    double uniqueCount() const override {
        const double numerator = alpha_ * static_cast<double>(registers_.size() * registers_.size());
        double denominator = 0;

        int emptyRegs = 0;
        for (const char reg : registers_) {
            denominator += 1.0 / (1 << reg);
            if (reg == 0) ++emptyRegs;
        }

        double estimate = numerator / denominator;

        if (estimate <= 2.5 * registers_.size() && emptyRegs > 0) {
            estimate = registers_.size() * log(static_cast<double>(registers_.size()) / emptyRegs);
        }
        return estimate;
    }
};

#endif
