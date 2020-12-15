#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);

    void closeEvent(QCloseEvent*);

signals:
    void Close_Event();
};

#endif // MAINWINDOW_H
