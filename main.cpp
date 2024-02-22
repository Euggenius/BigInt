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

    cout << "Enter Pi precision: ";
    cin >> precision;

    Number::precision = precision;

    find_PI();

    return 0;
}