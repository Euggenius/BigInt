#ifndef BIGINT_BIGINT_H
#define BIGINT_BIGINT_H

#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

enum class Sign {
    Positive,
    Negative
};

class Number {
    private:
        Sign sign;
        vector<char> integerPart;
        vector<char> decimalPart;

        char addDigits(const char& dig1, const char& dig2, char& carry) const {
            char sum = dig1 + dig2 + carry;
            carry = sum / 10;
            return sum % 10;
        }

        char subtractDigits(const char& dig1, const char& dig2, char& borrow) const {
            char diff = dig1 - dig2 - borrow;
            borrow = (diff < 0) ? 1 : 0;
            return (borrow == 1) ? diff + 10 : diff;
        }

        vector<char> multiplyCombined(const vector<char>& num1, const vector<char>& num2) const {
            vector<char> result(num1.size() + num2.size(), 0);

            for (int i = num1.size() - 1; i >= 0; --i) {
                char carry = 0;
                for (int j = num2.size() - 1; j >= 0 || carry != 0; --j) {
                    char current = result[i + j + 1] + num1[i] * (j >= 0 ? num2[j] : 0) + carry;
                    result[i + j + 1] = current % 10;
                    carry = current / 10;
                }
            }

            return result;
        }

        friend Number findApproximateDivision(const Number& divisor);

    public:
        static size_t precision;

        Number(const vector<char>& integerPart_, const vector<char>& decimalPart_, const Sign sign_ = Sign::Positive) {
            integerPart = integerPart_;
            decimalPart = decimalPart_;
            sign = sign_;

            while (!decimalPart.empty() && !decimalPart.back()) {
                decimalPart.pop_back();
            }

            if (integerPart.empty()){
                integerPart.push_back(0);
            }

            if (decimalPart.empty() && *integerPart.begin() == 0) {
                sign = Sign::Positive;
            }
        }
    
        Number(const string& numberStr) {
            stringstream ss(numberStr);
            char ch;
            
            sign = (numberStr[0] == '-') ?Sign::Negative : Sign::Positive;

            while (ss.get(ch) && ch != '.') {
                if (isdigit(ch)) {
                    integerPart.push_back(ch - '0');
                }
            }

            while (ss.get(ch)) {
                if (isdigit(ch)) {
                    decimalPart.push_back(ch - '0');
                }
            }

            while (!decimalPart.empty() && !decimalPart.back()) {
                decimalPart.pop_back();
            }

            if (integerPart.empty()){
                integerPart.push_back(0);
            }

            if (decimalPart.empty() && *integerPart.begin() == 0) {
                sign = Sign::Positive;
            }
        }

        Number operator+(const Number& other) const;
        Number operator-(const Number& other) const;
        Number operator*(const Number& other) const;
        Number operator/(const Number& other) const;
        Number operator-() const;

        bool operator==(const Number& other) const;
        bool operator!=(const Number& other) const;
        bool operator<(const Number& other) const;
        bool operator<=(const Number& other) const;
        bool operator>(const Number& other) const;
        bool operator>=(const Number& other) const;

        friend ostream& operator<<(ostream& os, const Number& number);
};
#endif