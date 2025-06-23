//
// Created by admin on 17.05.2025.
//

#ifndef LAB3_LINAL_H
#define LAB3_LINAL_H

#include <ostream>
#include <cmath>

namespace linal {
    class Complex {
    public:
        Complex(double real = 0.0, double imag = 0.0) : real_(real), imag_(imag) {}

        double real() const{
            return real_;
        }

        double imag() const{
            return imag_;
        }

        Complex operator+(const Complex& other) const{
            return {real_ + other.real_, imag_ + other.imag_};
        }

        Complex operator*(const Complex& other) const {
            return {real_ * other.real_ - imag_ * other.imag_,
                    real_ * other.imag_ + imag_ * other.real_};
        }

        Complex operator-() const {
            return {-real_, -imag_};
        }

        Complex conjugate() const {
            return {real_, -imag_};
        }

        double abs() const {
            return std::hypot(real_, imag_);
        }

        bool operator==(const Complex& other) const{
            const double d = 1e-10;
            return std::abs(real_ - other.real_) < d &&
                   std::abs(imag_ - other.imag_) < d;
        }

        bool operator!=(const Complex& other) const {
            return !(*this == other);
        }
    private:
        double real_;
        double imag_;
    };

    float conjugate(float x) { return x; }

    double conjugate(double x) { return x; }

    long double conjugate(long double x) { return x; }

    Complex conjugate(Complex z) { return z.conjugate(); }

    float abs(float x) { return std::abs(x); }

    double abs(double x) { return std::abs(x); }

    long double abs(long double x) { return std::abs(x); }

    double abs(Complex z) { return z.abs(); }

    float inverse(float x) {
        if (x == 0.0f) throw std::invalid_argument("inverse: Division by zero.");
        return 1.0f / x;
    }

    double inverse(double x) {
        if (x == 0.0) throw std::invalid_argument("inverse: Division by zero.");
        return 1.0 / x;
    }

    long double inverse(long double x) {
        if (x == 0.0l) throw std::invalid_argument("inverse: Division by zero.");
        return 1.0l / x;
    }

    Complex inverse(Complex z) {
        double denom = z.real() * z.real() + z.imag() * z.imag();
        if (denom == 0.0) throw std::invalid_argument("inverse: Division by zero.");
        return {z.real() / denom, -z.imag() / denom};
    }

    template <typename T>
    concept Field = requires(T a, T b, T c) {
        { a == b } -> std::convertible_to<bool>;
        { a != b } -> std::convertible_to<bool>;

        { a + b } -> std::same_as<T>;
        { T{} } -> std::same_as<T>;
        { -a } -> std::same_as<T>;

        { a * b } -> std::same_as<T>;
        { T{1} } -> std::same_as<T>;
        { inverse(a) } -> std::same_as<T>;

        requires !std::is_same_v<T, bool>;
    };

    template <typename T>
    concept EuclideanField = Field<T> && requires(T a) {
        { linal::abs(a) } -> std::convertible_to<double>;
        { linal::conjugate(a) } -> std::convertible_to<T>;
        requires std::is_same_v<T, float> ||
                 std::is_same_v<T, double> ||
                 std::is_same_v<T, long double> ||
                 std::is_same_v<T, Complex>;
    };
}

std::ostream& operator<< (std::ostream& os, linal::Complex z){
    os << "{Re: " << z.real() << ", Im: " << z.imag() << "}";
    return os;
}

#endif //LAB3_LINAL_H
