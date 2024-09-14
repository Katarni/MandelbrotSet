//
// Created by Тимур Ахметзянов on 09.09.2024.
//

#include "App.h"

App::App() {
    std::ifstream style_sheet_file("../main-styles.qss");
    std::stringstream string_stream;
    string_stream << style_sheet_file.rdbuf();
    main_styles_ = QString::fromStdString(string_stream.str());
    style_sheet_file.close();

    width_ = 1000, height_ = 700;
    setFixedSize(width_, height_);
    setWindowTitle("Mandelbrot Set");
    setMouseTracking(true);

    mouse_pressed_ = false;
    size_set_ = false;
    started_ = true;
    is_rect_selected_ = false;
    selected_width_ = selected_height_ = 0;

    mandelbrot_scene_ = new QGraphicsScene(this);
    mandelbrot_scene_->setSceneRect(0, 0, width_, height_);
    mandelbrot_scene_->setBackgroundBrush(Qt::white);

    mandelbrot_view_ = new QGraphicsView(this);
    mandelbrot_view_->resize(width_, height_);
    mandelbrot_view_->setScene(mandelbrot_scene_);
    mandelbrot_view_->setMouseTracking(true);
    mandelbrot_view_->setDisabled(true);
    mandelbrot_view_->setStyleSheet(main_styles_);
    mandelbrot_view_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mandelbrot_view_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    selected_rect_ = new QWidget(this);
    selected_rect_->resize(0, 0);
    selected_rect_->setObjectName("SelectedRect");
    selected_rect_->setStyleSheet(main_styles_);
    mandelbrot_thread_ = new QThread;

    set_ = new MandelbrotSet(width_, height_);
    generateSet();

    connect(set_, &MandelbrotSet::pixReady, this, [this](int i, int j, int val) {
        if (started_) return;
        auto pix = mandelbrot_scene_->addRect(i, j, 1, 1);
        pix->setBrush(getBrush(val));
        pix->setPen(Qt::NoPen);
        update();
    });
    connect(mandelbrot_thread_, &QThread::started, set_, &MandelbrotSet::generateSet);
    connect(set_, &MandelbrotSet::finished, this, [this]() {
        mandelbrot_thread_->quit();

        mandelbrot_scene_->clear();
        for (int j = 0; j < height_; ++j) {
            for (int i = 0; i < width_; ++i) {
                auto pix = mandelbrot_scene_->addRect(i, j, 1, 1);
                pix->setBrush(getBrush(set_->at(i, j)));
                pix->setPen(Qt::NoPen);
            }
        }
        update();
        started_ = false;
    });
}

void App::mouseMoveEvent(QMouseEvent *event) {
    if (mouse_pressed_) {
        selected_rect_->resize(std::abs(start_mouse_pos_.x() - QWidget::mapFromGlobal(QCursor::pos()).x()),
                               std::abs(start_mouse_pos_.y() - QWidget::mapFromGlobal(QCursor::pos()).y()));

        selected_rect_->move(std::min(start_mouse_pos_.x(), QWidget::mapFromGlobal(QCursor::pos()).x()),
                             std::min(start_mouse_pos_.y(), QWidget::mapFromGlobal(QCursor::pos()).y()));
    }

    QWidget::mouseMoveEvent(event);
}

void App::mousePressEvent(QMouseEvent *event) {
    mouse_pressed_ = true;
    is_rect_selected_ = false;
    size_set_ = false;
    start_mouse_pos_ = QWidget::mapFromGlobal(QCursor::pos());
    selected_rect_->move(start_mouse_pos_);
    selected_rect_->resize(0, 0);
    QWidget::mousePressEvent(event);
}

void App::mouseReleaseEvent(QMouseEvent *event) {
    mouse_pressed_ = false;
    is_rect_selected_ = true;
    selected_width_ = selected_rect_->width();
    selected_height_ = selected_rect_->height();

    QWidget::mouseReleaseEvent(event);
}

void App::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Backspace) {
        selected_rect_->resize(0, 0);
        is_rect_selected_ = false;
        if (set_->rollback()) generateSet();
        update();
        return;
    }

    if (!is_rect_selected_) return;

    if (event->key() == Qt::Key_Enter || event->key()==Qt::Key_Return) {
        if (!size_set_) formatRect(true);
        set_->scale(selected_rect_->x(), selected_rect_->y(),
                  selected_rect_->x() + selected_rect_->width(),
                  selected_rect_->y() + selected_rect_->height());
        selected_rect_->resize(0, 0);
        is_rect_selected_ = false;

        generateSet();
    } else if (event->key() == Qt::Key_Right) {
        formatRect(true);
    } else if (event->key() == Qt::Key_Down) {
        formatRect(false);
    }
    QWidget::keyReleaseEvent(event);
}

void App::generateSet() {
    set_->moveToThread(mandelbrot_thread_);
    mandelbrot_thread_->start();
}

void App::formatRect(bool width) {
    if (width) {
        selected_rect_->resize(selected_width_, double(selected_width_) * height_ / width_);
        if (selected_rect_->x() < start_mouse_pos_.x()) {
            selected_rect_->move(start_mouse_pos_.x() - selected_rect_->width(),
                                 selected_rect_->y());
        }
        if (selected_rect_->y() < start_mouse_pos_.y()) {
            selected_rect_->move(selected_rect_->x(),
                                 start_mouse_pos_.y() - selected_rect_->height());
        }
    } else {
        selected_rect_->resize(double(selected_height_) * width_ / height_, selected_height_);
        if (selected_rect_->x() < start_mouse_pos_.x()) {
            selected_rect_->move(start_mouse_pos_.x() - selected_rect_->width(),
                                 selected_rect_->y());
        }
        if (selected_rect_->y() < start_mouse_pos_.y()) {
            selected_rect_->move(selected_rect_->x(),
                                 start_mouse_pos_.y() - selected_rect_->height());
        }
    }
    size_set_ = true;
    update();
}

QBrush App::getBrush(int val) {
    if (val >= 200) {
        return Qt::black;
    } else if (val >= 185) {
        return QColor(153, 255, 250);
    } else if (val >= 170) {
        return QColor(105, 255, 255);
    } else if (val >= 150) {
        return QColor(25, 252, 211);
    } else if (val >= 130) {
        return QColor(7, 230, 178);
    } else if (val >= 100) {
        return QColor(67, 178, 230);
    } else if (val >= 80) {
        return QColor(47, 131, 204);
    } else if (val >= 50) {
        return QColor(72, 61, 227);
    } else if (val >= 20) {
        return QColor(88, 53, 204);
    } else if (val >= 15) {
        return QColor(84, 49, 181);
    } else if (val >= 10) {
        return QColor(68, 36, 156);
    } else if (val >= 5) {
        return QColor(59, 29, 143);
    } else if (val >= 3) {
        return QColor(48, 23, 117);
    } else if (val >= 2) {
        return QColor(36, 15, 97);
    }
    return QColor(25, 6, 79);
}
