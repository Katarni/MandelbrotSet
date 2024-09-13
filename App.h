//
// Created by Тимур Ахметзянов on 09.09.2024.
//

#pragma once

#include "MandelbrotSet.h"


class App : public QMainWindow {
    Q_OBJECT

 public:
    App();

 protected:
    void keyReleaseEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

 private:
    bool mouse_pressed_, is_rect_selected_, size_set_, started_;
    int selected_width_, selected_height_;
    int width_, height_;
    QPoint start_mouse_pos_;
    QString main_styles_;

    QWidget *selected_rect_;
    QGraphicsScene *mandelbrot_scene_;
    QGraphicsView *mandelbrot_view_;

    MandelbrotSet *set_;

    QThread *mandelbrot_thread_;

    void generateSet();

    void formatRect(bool width);
};
