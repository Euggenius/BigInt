#include "hse_project.h"

size_t Number::precision = 50;

int main() {
    assert(Number(154.3) * Number(14.2) == Number(2191.06));
    assert((Number(-15) / Number(-3)) == Number(5));
    assert(Number(0) * Number(1543) == Number(0));
    assert(Number("-8238234.175645") + Number("83346393.24854") == Number(-8238234.175645 + 83346393.24854));
    assert(Number("-0") - Number(0) == Number("0"));
    assert(Number(123E-20) / Number(123) == Number(1E-20));
    assert(Number("0.0000000000999") < Number("0.0000000001"));
    assert(Number(18263363) != Number(18263353));
    assert(Number("0") + Number("0.1") == Number(1E-1));

    cout << "All tests passt";

    return 0;
}
