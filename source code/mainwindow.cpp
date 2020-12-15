#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
}

void MainWindow::closeEvent(QCloseEvent*)
{
    emit Close_Event();
}
