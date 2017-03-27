#include "catch.hpp"
#include "fff.hpp"
#include "mocks.hpp"
#include <fl-lib.h>

extern "C"
{
    #include "ios.h"
}

TEST_CASE ("ios")
{
    CHECK (1 == ios_port_number (ios_port_1_pin_32));
    CHECK (32 == ios_pin_number (ios_port_1_pin_32));

    CHECK (0 == ios_port_number (ios_port_0_pin_15));
    CHECK (15 == ios_pin_number (ios_port_0_pin_15));
}
