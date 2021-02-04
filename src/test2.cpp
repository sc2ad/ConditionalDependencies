#include "main.hpp"

// If this does not crash, it performs as expected.
// See test.cpp to see the definition of the test function.
extern "C" void load() {
    auto test = CondDep::Find<void, int, float>("conditional-dependencies-test", "test");
    test.value()(3, 4.1f);
}