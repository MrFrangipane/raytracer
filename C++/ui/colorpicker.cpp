#include "colorpicker.h"

// Paint CSS
void ColorPicker::paintEvent(QPaintEvent *event)
{
  QStyleOption options;
  options.initFrom(this);
  QPainter painter(this);
  style()->drawPrimitive(
    QStyle::PE_Widget, &options, &painter, this);
}


// Constructor
ColorPicker::ColorPicker(QWidget *parent) : QWidget(parent)
{
    setStyleSheet("ColorPicker {background-color: rgba(0, 0, 0) solid}");
}


// Set Color
void ColorPicker::set_color(std::uint8_t r_, std::uint8_t g_, std::uint8_t b_, std::uint8_t a_)
{
    r = r_;
    g = g_;
    b = b_;
    a = a_;
    setStyleSheet(QString::fromStdString(
        "ColorPicker {background-color: rgb(" + std::to_string(r) + ", " + std::to_string(g) + ", " + std::to_string(b) + ")}"
    ));
}


// Mouse Press
void ColorPicker::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        // Color Picker
        QColorDialog color_dialog(this);
        color_dialog.setWindowTitle("Pick Color");
        color_dialog.setCurrentColor(QColor(r, g, b, a));
        int result = color_dialog.exec();
        QColor color = color_dialog.selectedColor();

        // Exit if Canceled
        if (result == QColorDialog::DialogCode::Rejected) return;

        // Update
        set_color(
            color.red(),
            color.green(),
            color.blue(),
            color.alpha()
        );

        // Emit
        emit(color_changed());
    }
}
