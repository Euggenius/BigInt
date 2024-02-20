#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

enum class Sign {
    Positive,
    Negative
};


char addDigits(char dig1, char dig2, char& carry) {
    char sum = dig1 + dig2 + carry;
    carry = sum / 10;
    return sum % 10;
}


char subtractDigits(char dig1, char dig2, char& borrow) {
    char diff = dig1 - dig2 - borrow;
    borrow = (diff < 0) ? 1 : 0;
    return (borrow == 1) ? diff + 10 : diff;
}


class Number {
    private:
        Sign sign;
        vector<char> integerPart;
        vector<char> decimalPart;
        // size_t integerPartLength;
        // size_t decimalPartLength;
        // bool is_positive;

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

    public:
        using enum Sign;

        Number(const vector<char>& integerPart_, const vector<char>& decimalPart_, const Sign sign_ = Positive) {
            integerPart = integerPart_;
            decimalPart = decimalPart_;
            sign = sign_;

            while (!decimalPart.empty() && !decimalPart.back()) {
                decimalPart.pop_back();
            }

            if (*integerPart.begin() == 0) {
                sign = Positive;
            }
        }

        Number(const string& numberStr) {
            stringstream ss(numberStr);
            char ch;
            
            sign = (numberStr[0] == '-') ? Negative : Positive;

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

            if (*integerPart.begin() == 0) {
                sign = Positive;
            }
        }

        Number operator+(const Number& other) const {
            if (sign == Negative && other.sign == Negative) {
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

            Number result(resultInteger, resultDecimal);

            return result;
        }

        Number operator-(const Number& other) const {
            if (sign == other.sign) {
                if (sign == Positive && other.sign == Positive) {
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

            Number result(resultInteger, resultDecimal, sign);

            return result;
        }

        Number operator*(const Number& other) const {
            vector<char> num1Combined = integerPart;
            num1Combined.insert(num1Combined.end(), decimalPart.begin(), decimalPart.end());

            vector<char> num2Combined = other.integerPart;
            num2Combined.insert(num2Combined.end(), other.decimalPart.begin(), other.decimalPart.end());

            vector<char> resultCombined = multiplyCombined(num1Combined, num2Combined);

            vector<char> resultInteger (resultCombined.begin(), resultCombined.end() - decimalPart.size() - other.decimalPart.size());
            vector<char> resultDecimal (resultCombined.end() - decimalPart.size() - other.decimalPart.size(), resultCombined.end());
            Sign resultSign = (sign == other.sign) ? Positive : Negative;

            while (!resultInteger.empty() && !*resultInteger.begin()) {
                resultInteger.erase(resultInteger.begin());
            }

            if (resultInteger.empty()) {
                resultInteger.push_back(0);
            }

            Number result(resultInteger, resultDecimal, resultSign);

            return result;
        }

        Number operator-() const {
            Number result = *this;
            result.sign = (result.sign == Positive) ? Negative : Positive;
            return result;
        }

        bool operator==(const Number& other) const {
            return (integerPart == other.integerPart) && (decimalPart == other.decimalPart) && (sign == other.sign);
        }

        bool operator!=(const Number& other) const {
            return !(*this == other);
        }

        bool operator<(const Number& other) const {
            if (sign == Negative && other.sign == Negative) return -other < -*this;

            if (sign == Positive && other.sign == Negative) return false;

            if (sign == Negative && other.sign == Positive) return true;

            if (integerPart.size() != other.integerPart.size()) {
                return integerPart.size() < other.integerPart.size();
            }

            if (decimalPart.size() != other.decimalPart.size()) {
                return decimalPart.size() < other.decimalPart.size();
            }

            for (size_t i = 0; i < integerPart.size(); ++i) {
                if (integerPart[i] != other.integerPart[i]) {
                    return integerPart[i] < other.integerPart[i];
                }
            }

            for (size_t i = 0; i < decimalPart.size(); ++i) {
                if (decimalPart[i] != other.decimalPart[i]) {
                    return decimalPart[i] < other.decimalPart[i];
                }
            }

            return false;
        }

        bool operator<=(const Number& other) const {
            return (*this < other) || (*this == other);
        }

        bool operator>(const Number& other) const {
            return !(*this <= other);
        }

        bool operator>=(const Number& other) const {
            return !(*this < other);
        }

        friend ostream& operator<<(ostream& os, const Number& number) {
            if (number.sign == Negative) os << '-';

            for (int digit : number.integerPart) {
                os << digit;
            }

            if (number.integerPart.empty()) os << '0';

            if (!number.decimalPart.empty()) {
                os << '.';

                for (int digit : number.decimalPart) {
                    os << digit;
                }
            }

            return os;
        }
};


int main() {
    Number num1("100.3733232");
    Number num2("90.023823");

    Number resultAdd = num1 + num2;

    cout << num1 << ' ' << num2 << '\n' << '\n';
    cout << "Subtraction: " << (num1 - num2) << '\n';
    cout << "Addition: " << resultAdd << '\n';
    cout << "Multiplication: " << num1 * num2 << '\n';

    return 0;
}