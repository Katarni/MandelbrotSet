//
// Created by Тимур Ахметзянов on 07.09.2024.
//

#pragma once

#include "includes.h"


class Complex {
 public:
    Complex() : Complex(0, 0) {}
    explicit Complex(double real) : Complex(real, 0) {}
    Complex(double real, double imag) : real_(real), imag_(imag) {}

    friend Complex operator+(const Complex& a, const Complex& b) {
        return {a.real_ + b.real_, a.imag_ + b.imag_};
    }

    friend Complex operator*(const Complex& a, const Complex& b) {
        return {a.real_*b.real_ - a.imag_*b.imag_,
                a.real_*b.imag_ + a.imag_*b.real_};
    }

    friend bool isInf(const Complex& num) {
        return num.real_*num.real_ + num.imag_*num.imag_ > 4;
    }

    [[nodiscard]]
    double getReal() const {
        return real_;
    }

    [[nodiscard]]
    double getImag() const {
        return imag_;
    }

 private:
    double real_, imag_;
};
