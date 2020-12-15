#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QVideoWidget>
#include <QWidget>
#include <QMouseEvent>
#include <QEvent>

class VideoWidget : public QVideoWidget
{
    Q_OBJECT

public:
    explicit VideoWidget(QWidget* parent = 0);

    void enterEvent(QEvent*);
    void leaveEvent(QEvent*);
    void mousePressEvent(QMouseEvent*);

signals:
    void Mouse_Enter();
    void Mouse_Left();
    void Mouse_Pressed();
};

#endif // VIDEOWIDGET_H
