#include <iostream>
#include <string>
#include <algorithm>
#include <utility>

class big_integer {
private:
    std::string digits_;  // цифры числа в обратном порядке (младшие в начале)

    // Удаление ведущих нулей (в конце строки, т.к. храним в обратном порядке)
    void trim() {
        while (digits_.size() > 1 && digits_.back() == '0') {
            digits_.pop_back();
        }
    }

public:
    // Конструктор по умолчанию
    big_integer() : digits_("0") {}

    // Конструктор из строки
    big_integer(const std::string& str) {
        if (str.empty()) {
            digits_ = "0";
            return;
        }
        // Записываем цифры в обратном порядке
        digits_.reserve(str.size());
        for (auto it = str.rbegin(); it != str.rend(); ++it) {
            if (*it < '0' || *it > '9') {
                throw std::invalid_argument("Invalid character in number");
            }
            digits_.push_back(*it);
        }
        trim();
    }

    // Конструктор копирования
    big_integer(const big_integer& other) : digits_(other.digits_) {}

    // Конструктор перемещения
    big_integer(big_integer&& other) noexcept
        : digits_(std::move(other.digits_)) {
        other.digits_ = "0";
    }

    // Оператор присваивания копированием
    big_integer& operator=(const big_integer& other) {
        if (this != &other) {
            digits_ = other.digits_;
        }
        return *this;
    }

    // Перемещающий оператор присваивания
    big_integer& operator=(big_integer&& other) noexcept {
        if (this != &other) {
            digits_ = std::move(other.digits_);
            other.digits_ = "0";
        }
        return *this;
    }

    // Оператор сложения двух больших чисел
    big_integer operator+(const big_integer& other) const {
        big_integer result;
        result.digits_.clear();

        const std::string& a = digits_;
        const std::string& b = other.digits_;
        size_t n = std::max(a.size(), b.size());
        result.digits_.reserve(n + 1);

        int carry = 0;
        for (size_t i = 0; i < n || carry; ++i) {
            int sum = carry;
            if (i < a.size()) sum += a[i] - '0';
            if (i < b.size()) sum += b[i] - '0';
            result.digits_.push_back(static_cast<char>('0' + sum % 10));
            carry = sum / 10;
        }

        result.trim();
        return result;
    }

    // Оператор умножения на число (int)
    big_integer operator*(int multiplier) const {
        if (multiplier == 0 || digits_ == "0") {
            return big_integer("0");
        }
        if (multiplier == 1) {
            return *this;
        }

        bool negative = multiplier < 0;
        long long m = std::llabs(static_cast<long long>(multiplier));

        big_integer result;
        result.digits_.clear();
        result.digits_.reserve(digits_.size() + 20);

        long long carry = 0;
        for (size_t i = 0; i < digits_.size() || carry; ++i) {
            long long cur = carry;
            if (i < digits_.size()) {
                cur += (digits_[i] - '0') * m;
            }
            result.digits_.push_back(static_cast<char>('0' + cur % 10));
            carry = cur / 10;
        }

        result.trim();

        if (negative && result.digits_ != "0") {
            result.digits_.push_back('-');
        }
        return result;
    }

    // Вывод в поток
    friend std::ostream& operator<<(std::ostream& os, const big_integer& num) {
        for (auto it = num.digits_.rbegin(); it != num.digits_.rend(); ++it) {
            os << *it;
        }
        return os;
    }
};

int main() {
    auto number1 = big_integer("114575");
    auto number2 = big_integer("78524");
    auto result = number1 + number2;
    std::cout << result << std::endl;  // 193099

    // Проверка перемещения
    auto number3 = std::move(number1);
    std::cout << number3 << std::endl;  // 114575

    // Проверка умножения
    auto product = number2 * 3;
    std::cout << product << std::endl;  // 235572

    return 0;
}