#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>

#include "hyperloglog.h"
#include "tester.h"
#include "random_stream_gen.h"
#include "hash_func_gen.h"

static constexpr size_t STREAM_SIZE = 1'000'000;
static constexpr size_t STEP        = 10'000;
static constexpr size_t RUNS        = 10;
static constexpr size_t BITS        = 17;

int main() {
    HashFuncGen hashGen;

    const size_t m = 1ULL << BITS;
    const double RSE_LOW  = 1.042 / std::sqrt(m);
    const double RSE_HIGH = 1.32  / std::sqrt(m);

    {
        std::cout << "# EXPERIMENT 1\n";
        std::cout << "# t, real, estimate, rel_error\n";

        std::vector<HyperLogLog> hlls;
        hlls.emplace_back(BITS, hashGen());

        Tester tester(std::move(hlls));
        RandomStreamGen gen(STREAM_SIZE, 123);

        size_t t = 0;
        size_t violations_low  = 0;
        size_t violations_high = 0;

        for (size_t i = 0; i < STREAM_SIZE; ++i) {
            tester.add(gen.getStream()[i]);

            if ((i + 1) % STEP == 0) {
                ++t;

                double real = tester.realCount();
                double est  = tester.hllCount(0);
                double rel_error = std::abs(est - real) / real;

                if (rel_error > RSE_LOW)  ++violations_low;
                if (rel_error > RSE_HIGH) ++violations_high;

                std::cout
                    << t << ","
                    << real << ","
                    << est << ","
                    << rel_error << "\n";
            }
        }

        std::cout << "\n# THEORETICAL BOUNDS\n";
        std::cout << "m = " << m << "\n";
        std::cout << "1.042/sqrt(m) = " << RSE_LOW  << "\n";
        std::cout << "1.32 /sqrt(m) = " << RSE_HIGH << "\n";
        std::cout << "Violations (1.042): " << violations_low  << "\n";
        std::cout << "Violations (1.32 ): " << violations_high << "\n\n";
    }

    {
        std::cout << "# EXPERIMENT 2\n";
        std::cout << "# t, real, mean, sigma, sigma_theory\n";

        std::vector<HyperLogLog> hlls;
        for (size_t i = 0; i < RUNS; ++i) {
            hlls.emplace_back(BITS, hashGen());
        }

        Tester tester(std::move(hlls));
        RandomStreamGen gen(STREAM_SIZE, 777);

        size_t t = 0;

        for (size_t i = 0; i < STREAM_SIZE; ++i) {
            tester.add(gen.getStream()[i]);

            if ((i + 1) % STEP == 0) {
                ++t;

                double real  = tester.realCount();
                double mean  = tester.meanEstimate();
                double sigma = tester.sigmaEstimate();

                double sigma_theory = RSE_LOW * real;

                std::cout
                    << t << ","
                    << real << ","
                    << mean << ","
                    << sigma << ","
                    << sigma_theory << "\n";
            }
        }
    }

    return 0;
}