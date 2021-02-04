#include "main.hpp"

EXPOSE_API(test, void, int x, float y) {
    // do something
}

extern "C" void load() {
    // Do nothing
}