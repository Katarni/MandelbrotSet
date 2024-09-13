//
// Created by Тимур Ахметзянов on 07.09.2024.
//

#pragma once

#include "Complex.h"


class MandelbrotSet : public QObject {
    Q_OBJECT

 public:
    MandelbrotSet() : MandelbrotSet(0, 0) {}
    MandelbrotSet(int width, int height);

    void scale(int x_left, int y_top, int x_right, int y_bottom);

    [[nodiscard]]
    int at(int i, int j) const {
        return set[i][j];
    }

    int rollback();

 public slots:
    void generateSet();

 signals:
    void pixReady(int i, int j, int val);
    void finished();

 protected:
    Complex top_left_point_, bottom_right_point_;
    int width_, height_;
    std::stack<std::pair<Complex, Complex>> prev_;

    std::vector<std::vector<int>> set;
};
