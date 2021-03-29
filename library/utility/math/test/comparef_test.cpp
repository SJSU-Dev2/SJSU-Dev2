#include <cstdint>

#include "testing_frameworks.hpp"
#include "comparef.hpp"

namespace sjsu 
{
    TEST_CASE("Testing compare_floats"){
    bool test = compare_floats(0.0145f, 0.0143f, 0.0001f);
    }
}