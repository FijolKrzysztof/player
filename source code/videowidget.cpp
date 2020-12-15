#include "videowidget.h"

VideoWidget::VideoWidget(QWidget* parent) : QVideoWidget(parent)
{
}
void VideoWidget::enterEvent(QEvent*)
{
    emit Mouse_Enter();
}
void VideoWidget::leaveEvent(QEvent*)
{
    emit Mouse_Left();
}
void VideoWidget::mousePressEvent(QMouseEvent*)
{
    emit Mouse_Pressed();
}
