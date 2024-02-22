#include "hse_project.h"
#include <gtest/gtest.h>

TEST(mult, test1){
    Number a(to_string(12));
    Number b(to_string(13));
    Number c = a * b;
    ASSERT_EQ(c, Number("156"));
}