#ifndef TESTER_H
#define TESTER_H

#include <unordered_set>
#include <utility>

#include "hyperloglog.h"


class Tester {
private:
    std::vector<HyperLogLog> hlls_;
    std::unordered_set<std::string> tests_;

public:
    explicit Tester(std::vector<HyperLogLog> hlls) : hlls_(std::move(hlls)) {}

    void add(const std::string& test) {
        for (HyperLogLog& hll : hlls_) {
            hll.add(test);
        }
        tests_.insert(test);
    }

    double hllCount(const size_t ind) const {
        return hlls_[ind].uniqueCount();
    }

    size_t realCount() const {
        return tests_.size();
    }

    double metric() const {
        double res = 0;
        for (const HyperLogLog& hll : hlls_) {
            const double delta = hll.uniqueCount() - realCount();
            res += delta * delta;
        }
        return res / hlls_.size();
    }

    double meanEstimate() const {
        double sum = 0.0;
        for (const HyperLogLog& hll : hlls_) {
            sum += hll.uniqueCount();
        }
        return sum / hlls_.size();
    }

    double varianceEstimate() const {
        const double mean = meanEstimate();
        double var = 0.0;

        for (const HyperLogLog& hll : hlls_) {
            const double d = hll.uniqueCount() - mean;
            var += d * d;
        }
        return var / hlls_.size();
    }

    double sigmaEstimate() const {
        return std::sqrt(varianceEstimate());
    }
};


#endif
