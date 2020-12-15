#ifndef PLAYER_H
#define PLAYER_H

#include "thread.h"
#include "videowidget.h"
#include "slider.h"
#include "mainwindow.h"
#include <QMainWindow>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QMediaPlayer>
#include <QPushButton>
#include <QLabel>
#include <QShortcut>
#include <vector>
#include <QPalette>
#include <QTimer>

#include <chrono>
using namespace std;
using namespace chrono;

class player : public QMainWindow
{
    Q_OBJECT

public:
    player(QWidget *parent = nullptr);
    ~player();

    QTimer* timerChecker = new QTimer();

    steady_clock::time_point start;
    steady_clock::time_point end;
    int currentVideo;
    int point;
    int volume;
    int pageCounter = 0;
    int pageNumber;
    int playerNumber;
    bool setRandom;
    bool play = true;
    bool mainWindowExists;
    QPalette backgroundPalette = QPalette();
    QPalette textPalette = QPalette();
    QPalette firstBackgroundPalette = QPalette();
    QPalette sliderPalette = QPalette();
    Thread* thread;
    MainWindow* mainWindow = new MainWindow();
    QScrollBar* scrollBar;
    QLabel* label = new QLabel();
    QSlider* volumeSlider = new QSlider;
    Slider* mainSlider;
    QHBoxLayout* rowLayout[10];
    QMediaPlayer* smallPlayer;
    QMediaPlayer* mainVideoPlayer;
    QMediaPlayer* videoPlayer[30];
    VideoWidget* smallWidget;
    VideoWidget* mainVideoWidget = new VideoWidget();
    VideoWidget* videoWidget[30];
    QLabel* blackLabel = new QLabel();
    QPushButton* reloadButton = new QPushButton();
    QPushButton* upperBack = new QPushButton();
    QPushButton* upperNext = new QPushButton();
    QLabel* upperPage = new QLabel();
    QPushButton* buttonBack = new QPushButton();
    QPushButton* buttonNext = new QPushButton();
    QLabel* page = new QLabel();
    QShortcut* hideMain = new QShortcut(QKeySequence(Qt::Key_Escape), this, SLOT(hide_all()));
    QShortcut* up = new QShortcut(QKeySequence(Qt::Key_Up), this, SLOT(Volume_up()));
    QShortcut* down = new QShortcut(QKeySequence(Qt::Key_Down), this, SLOT(Volume_down()));
    QShortcut* scroll = new QShortcut(QKeySequence(Qt::Key_Right), this, SLOT(Scroll()));
    QShortcut* rewind = new QShortcut(QKeySequence(Qt::Key_Left), this, SLOT(Rewind()));
    QShortcut* space = new QShortcut(QKeySequence(Qt::Key_Space), this, SLOT(Play_stop()));

public slots:
    void reload();
    void Previous_page(bool);
    void Next_page(bool);
    void Set_random();
    void Play_random(int);
    void Mouse_enter(int);
    void Mouse_left(int);
    void Mouse_pressed(int);
    void Volume_up();
    void Volume_down();
    void Scroll();
    void Rewind();
    void Play_stop();
    void Exit_player();
    void Auto_move_slider(qint64);
    void Manual_move_slider(int);
    void Slider_enter();
    void Slider_left();
    void Slider_in();
    void Move_slider(int);
    void hide_all();
    void timer_remid();
};

extern vector<string> all_videos;
extern vector<int> points;
extern int files;
extern int rows;
extern string videoFolderPath;
extern string dataFolderPath;

#endif // PLAYER_H
