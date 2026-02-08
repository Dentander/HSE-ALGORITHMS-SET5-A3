#include <iostream>
#include <bits/ostream.tcc>

#include "tester.h"
#include "hash_func_gen.h"


constexpr size_t INSERTIONS = 1'000'000;
constexpr size_t TESTS = 3;


double testBits(const size_t bitsCount) {
    HashFuncGen hashGen;
    std::vector<HyperLogLog> hlls;
    for (size_t i = 0; i < TESTS; ++i) {
        hlls.emplace_back(bitsCount, hashGen());
    }

    Tester tester(hlls);
    RandomStreamGen streamGen(INSERTIONS);
    for (const std::string& toInsert : streamGen.getStream()) {
        tester.add(toInsert);
    }

    // for (size_t i = 0; i < TESTS; ++i) {
    //     std::cout << tester.realCount() << ' ' << tester.hllCount(i) << '\n';
    // }

    return tester.metric();
}


// int main() {
//     double bestMetrics = -1;
//     size_t bestBits = 0;
//     for (size_t i = 1; i <= 20; ++i) {
//         const double metric = testBits(i);
//         if (bestBits == 0 || metric < bestMetrics) {
//             bestBits = i;
//             bestMetrics = metric;
//         }
//         std::cout << "Bits: " << i << " Metrics: " << metric << "\n";
//     }
//
//     std::cout << "Best bits: " << bestBits << "\n";
// }