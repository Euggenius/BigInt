#include <iostream>
#include <chrono>
#include "hse_project.h"

void find_PI() {
    auto start = chrono::high_resolution_clock::now();

    Number Pi(0);
    Number hex(1);
    for(int k = 0; k < 0.8 * Number::precision + 5; k++) {
        Number sec = Number(4) / Number(8 * k + 1);
        Number third = Number(2) / Number(8 * k + 4);
        Number fourth = Number(1) / Number(8 * k + 5);
        Number fifth = Number(1) / Number(8 * k + 6);
        Pi = Pi + (sec - third - fourth - fifth) * hex;
        hex = hex / Number(16);
    }

    cout << Pi << '\n';

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> time = end - start;
    cout << "Time: " << time.count() << '\n';
}

size_t Number::precision = 50;

int main() {
    size_t precision;

    assert(Number(154.3) * Number(14.2) == Number(2191.06));
    assert((Number(-15) / Number(-3)) == Number(5));
    assert(Number(0) * Number(1543) == Number(0));
    assert(Number("-8238234.175645") + Number("83346393.24854") == Number(-8238234.175645 + 83346393.24854));
    assert(Number("-0") - Number(0) == Number("0"));

    cout << "Enter Pi precision: ";
    cin >> precision;

    Number::precision = precision;

    find_PI();

    return 0;
}