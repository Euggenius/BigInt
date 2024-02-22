#include "hse_project.h"

Number findApproximateDivision(const Number& dividend, const Number& divisor) {
    Number low("0");
    Number high("1");
    Number epsilon("0.5");

    const Number tenConst("10");
    const Number oneTenthConst("0.1");
    const Number oneSecondConst("0.5");

    for (size_t i = 0; i < divisor.decimalPart.size(); ++i) {
        high = high * tenConst;
    }

    high *= dividend;

    for (size_t i = 0; i < Number::precision + 10; ++i) {
        epsilon = epsilon * oneTenthConst;
    }

    while ((high - low) >= epsilon) {
        Number mid = (low + high) * oneSecondConst;
        Number multiplyResult = mid * divisor;

        if (multiplyResult > dividend) {
            high = mid;
        } else {
            low = mid;
            if (multiplyResult == dividend) {
                break;
            }
        }
    }

    low += epsilon;
    vector<char> decimalPartCropped(low.decimalPart.begin(), min(low.decimalPart.begin() + Number::precision + 5, low.decimalPart.end()));
    
    while (!decimalPartCropped.empty() && !decimalPartCropped.back()) {
        decimalPartCropped.pop_back();
    }

    low.decimalPart = decimalPartCropped;

    return low;
}

Number Number::operator+(const Number& other) const {
    if ((*this).sign == Sign::Negative && other.sign == Sign::Negative) {
        Number result = -*this + -other;
        return -result;
    } else if (sign != other.sign){
        Number result = *this - (-other);
        return result;
    }

    vector<char> resultInteger;
    vector<char> resultDecimal;

    size_t size1 = decimalPart.size();
    size_t size2 = other.decimalPart.size();

    char carry = 0;

    for (int ind = max(size1, size2) - 1; ind >= 0; --ind) {
        char digit1 = (ind < size1) ? decimalPart[ind] : 0;
        char digit2 = (ind < size2) ? other.decimalPart[ind] : 0;

        resultDecimal.push_back(addDigits(digit1, digit2, carry));
    }
    
    auto it1 = integerPart.rbegin();
    auto it2 = other.integerPart.rbegin();

    while (it1 != integerPart.rend() || it2 != other.integerPart.rend() || carry) {
        char digit1 = (it1 != integerPart.rend()) ? *it1 : 0;
        char digit2 = (it2 != other.integerPart.rend()) ? *it2 : 0;

        resultInteger.push_back(addDigits(digit1, digit2, carry));

        if (it1 != integerPart.rend()) ++it1;
        if (it2 != other.integerPart.rend()) ++it2;
    }

    reverse(resultInteger.begin(), resultInteger.end());
    reverse(resultDecimal.begin(), resultDecimal.end());

    return Number(resultInteger, resultDecimal);
}

Number Number::operator-(const Number& other) const {
    if (sign == other.sign) {
        if (sign == Sign::Positive && other.sign == Sign::Positive) {
            if (*this < other) {
                Number result = other - *this;
                return -result;
            }
        } else {
            Number result = -*this - (-other);
            return -result;
        }
    } else {
        Number result = *this + (-other);
        return result;
    }

    vector<char> resultInteger;
    vector<char> resultDecimal;

    size_t size1 = decimalPart.size();
    size_t size2 = other.decimalPart.size();

    char borrow = 0;

    for (int ind = max(size1, size2) - 1; ind >= 0; --ind) {
        char digit1 = (ind < size1) ? decimalPart[ind] : 0;
        char digit2 = (ind < size2) ? other.decimalPart[ind] : 0;

        resultDecimal.push_back(subtractDigits(digit1, digit2, borrow));
    }

    auto it1 = integerPart.rbegin();
    auto it2 = other.integerPart.rbegin();

    while (it1 != integerPart.rend() || it2 != other.integerPart.rend() || borrow != 0) {
        char digit1 = (it1 != integerPart.rend()) ? *it1 : 0;
        char digit2 = (it2 != other.integerPart.rend()) ? *it2 : 0;

        resultInteger.push_back(subtractDigits(digit1, digit2, borrow));

        if (it1 != integerPart.rend()) ++it1;
        if (it2 != other.integerPart.rend()) ++it2;
    }

    while (!resultInteger.empty() && resultInteger.back() == 0) {
        resultInteger.pop_back();
    }

    reverse(resultInteger.begin(), resultInteger.end());
    reverse(resultDecimal.begin(), resultDecimal.end());

    return Number(resultInteger, resultDecimal, sign);
}

Number Number::operator*(const Number& other) const {
    vector<char> num1Combined = integerPart;
    num1Combined.insert(num1Combined.end(), decimalPart.begin(), decimalPart.end());

    vector<char> num2Combined = other.integerPart;
    num2Combined.insert(num2Combined.end(), other.decimalPart.begin(), other.decimalPart.end());

    vector<char> resultCombined = multiplyCombined(num1Combined, num2Combined);

    vector<char> resultInteger (resultCombined.begin(), resultCombined.end() - decimalPart.size() - other.decimalPart.size());
    vector<char> resultDecimal (resultCombined.end() - decimalPart.size() - other.decimalPart.size(), resultCombined.end());
    Sign resultSign = ((*this).sign == other.sign) ? Sign::Positive : Sign::Negative;

    while (!resultInteger.empty() && !*resultInteger.begin()) {
        resultInteger.erase(resultInteger.begin());
    }

    if (resultInteger.empty()) {
        resultInteger.push_back(0);
    }

    return Number(resultInteger, resultDecimal, resultSign);
}

Number Number::operator/(const Number& other) const {

    if (other == Number("0")) {
        throw runtime_error("Division by zero is not allowed.");
    }

    if (other.sign == Sign::Positive){
        if (sign == Sign::Positive){
            return findApproximateDivision(*this, other);
        } else {
            return -findApproximateDivision(-*this, other);
        }
    } else {
        if (sign == Sign::Positive){
            return -findApproximateDivision(*this, -other);
        } else {
            return findApproximateDivision(-*this, -other);
        }
    }
}

Number Number::operator-() const {
    Number result = *this;
    result.sign = (result.sign == Sign::Positive) ? Sign::Negative : Sign::Positive;
    return result;
}

Number Number::operator+=(const Number& other) {
    return *this = *this + other;
}

Number Number::operator-=(const Number& other) {
    return *this = *this - other;
}

Number Number::operator*=(const Number& other) {
    return *this = *this * other;
}

Number Number::operator/=(const Number& other) {
    return *this = *this / other;
}

bool Number::operator==(const Number& other) const {
    return (integerPart == other.integerPart) && (decimalPart == other.decimalPart) && (sign == other.sign);
}

bool Number::operator!=(const Number& other) const {
    return !(*this == other);
}

bool Number::operator<(const Number& other) const {
    if (sign == Sign::Negative && other.sign == Sign::Negative) return -other < -*this;

    if (sign == Sign::Positive && other.sign == Sign::Negative) return false;

    if (sign == Sign::Negative && other.sign == Sign::Positive) return true;

    if (integerPart.size() != other.integerPart.size()) {
        return integerPart.size() < other.integerPart.size();
    }

    for (size_t i = 0; i < integerPart.size(); ++i) {
        if (integerPart[i] != other.integerPart[i]) {
            return integerPart[i] < other.integerPart[i];
        }
    }

    for (size_t i = 0; i < min(decimalPart.size(), other.decimalPart.size()); ++i) {
        if (decimalPart[i] != other.decimalPart[i]) {
            return decimalPart[i] < other.decimalPart[i];
        }
    }

    return decimalPart.size() < other.decimalPart.size();
}

bool Number::operator<=(const Number& other) const {
    return (*this < other) || (*this == other);
}

bool Number::operator>(const Number& other) const {
    return !(*this <= other);
}

bool Number::operator>=(const Number& other) const {
    return !(*this < other);
}

ostream& operator<<(ostream& os, const Number& number) {
    if (number.sign == Sign::Negative) os << '-';

    for (size_t i = 0; i < number.integerPart.size(); ++i) {
        os << (int)number.integerPart[i];
    }

    if (number.integerPart.empty()) os << '0';

    if (!number.decimalPart.empty()) {
        os << '.';

        for (size_t i = 0; i < min(number.decimalPart.size(), Number::precision); ++i) {
        os << (int)number.decimalPart[i];
    }
    }

    return os;
}
