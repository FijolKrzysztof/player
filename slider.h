#ifndef SLIDER_H
#define SLIDER_H

#include <QSlider>
#include <QMouseEvent>
#include <QAbstractSlider>
#include <QStyle>
#include <QStyleOptionSlider>

class Slider : public QSlider
{
    Q_OBJECT

protected:
    void mousePressEvent(QMouseEvent *event);

public:
    explicit Slider(QWidget* parent = 0);
    ~Slider();

    void enterEvent(QEvent*);
    void leaveEvent(QEvent*);
    void mouseMoveEvent(QMouseEvent*);

signals:
    void Mouse_Enter();
    void Mouse_Left();
    void Mouse_In();
    void Mouse_Click(int value);
};

#endif // SLIDER_H
