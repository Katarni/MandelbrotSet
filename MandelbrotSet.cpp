//
// Created by Тимур Ахметзянов on 07.09.2024.
//

#include "MandelbrotSet.h"

MandelbrotSet::MandelbrotSet(int width, int height) {
    width_ = width;
    height_ = height;

    set.resize(width_, std::vector<int>(height_));

    top_left_point_ = {-(width_ * 0.005 / 2.0), -(height_ * 0.005 / 2.0)};
    bottom_right_point_ = {width_ * 0.005 / 2.0, height_ * 0.005 / 2.0};
}

void MandelbrotSet::generateSet() {
    for (int j = 1; j <= height_; ++j) {
        for (int i = 1; i <= width_; ++i) {
            int k = 0;
            Complex c = {top_left_point_.getReal() +
                         i *(bottom_right_point_.getReal() - top_left_point_.getReal()) / width_,
                         top_left_point_.getImag() +
                         j *(bottom_right_point_.getImag() - top_left_point_.getImag()) / height_};
            for (Complex z; !isInf(z) && k < 200; ++k, z = z*z + c) {}
            emit pixReady(i - 1, j - 1, k);
            set[i - 1][j - 1] = k;
        }
    }
    finished();
}

void MandelbrotSet::scale(int x_left, int y_top, int x_right, int y_bottom) {
    prev_.emplace(top_left_point_, bottom_right_point_);
    ++x_left, ++y_top, ++x_right, ++y_bottom;

    auto tmp_top_left = Complex(top_left_point_.getReal() +
                                x_left *(bottom_right_point_.getReal() - top_left_point_.getReal()) / width_,
                                top_left_point_.getImag() +
                                y_top *(bottom_right_point_.getImag() - top_left_point_.getImag()) / height_);

    bottom_right_point_ = Complex(top_left_point_.getReal() +
                                  x_right *(bottom_right_point_.getReal() - top_left_point_.getReal()) / width_,
                                top_left_point_.getImag() +
                                y_bottom *(bottom_right_point_.getImag() - top_left_point_.getImag()) / height_);
    top_left_point_ = tmp_top_left;
}

int MandelbrotSet::rollback() {
    if (prev_.empty()) return 0;
    top_left_point_ = prev_.top().first;
    bottom_right_point_ = prev_.top().second;
    prev_.pop();
    return 1;
}
