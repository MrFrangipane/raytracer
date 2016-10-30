#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QWidget>
#include <QMouseEvent>
#include <QColor>
#include <QColorDialog>
#include <QString>
#include <QPaintEvent>
#include <QStyle>
#include <QStyleOption>
#include <QPainter>


class ColorPicker : public QWidget
{
    Q_OBJECT
public:
    // Members
    std::uint8_t r = 0;
    std::uint8_t g = 0;
    std::uint8_t b = 0;
    std::uint8_t a = 255;

    explicit ColorPicker(QWidget *parent = 0);
    void set_color(std::uint8_t r_, std::uint8_t g_, std::uint8_t b_, std::uint8_t a_);
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

signals:
    void color_changed();

public slots:
};

#endif // COLORPICKER_H
