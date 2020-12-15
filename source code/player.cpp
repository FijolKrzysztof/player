#include "player.h"
#include <QCursor>
#include <QScrollArea>
#include <QWidget>
#include <QVBoxLayout>
#include <QSignalMapper>
#include <chrono>
#include <fstream>
#include <QMessageBox>
#include <iostream>

using namespace std;
using namespace chrono;

int minimalFrame = 8;

player::player(QWidget *parent)
    : QMainWindow(parent)
{
    backgroundPalette.setColor(QPalette::Background, QColor(30,30,30));
    textPalette.setColor(QPalette::WindowText, Qt::white);
    firstBackgroundPalette.setColor(QPalette::Background, QColor(255,153,0));
    sliderPalette.setColor(QPalette::Button, QColor(0,0,0));
    sliderPalette.setColor(QPalette::Highlight, QColor(255,153,0));
    sliderPalette.setColor(QPalette::Window, Qt::white);
    QCursor::setPos(1915,100);
    thread = new Thread();
    QScrollArea* scrollArea = new QScrollArea();
    scrollBar = new QScrollBar();
    QWidget* central = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(central);

    connect(thread, SIGNAL(valueChanged(int)), this, SLOT(Play_random(int)));

    setGeometry(0,0,1850,2000);
    setCentralWidget(scrollArea);
    setAutoFillBackground(true);
    setPalette(firstBackgroundPalette);

    scrollArea->setWidgetResizable(true);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setVerticalScrollBar(scrollBar);
    scrollArea->setWidget(central);

    scrollBar->setStyleSheet("QScrollBar:vertical{ background: #f90; }"
                             "QScrollBar::handle:vertical{ background: #333333; }"
                             "QScrollBar::add-line:vertical { background: #f90; }"
                             "QScrollBar::sub-line:vertical { background: #f90; }");

    central->setAutoFillBackground(true);
    central->setPalette(firstBackgroundPalette);

    layout->setContentsMargins(10,15,0,15);

    QWidget* topRowWidget = new QWidget();
    QHBoxLayout* topRowLayout = new QHBoxLayout(topRowWidget);

    layout->addWidget(topRowWidget);

    int counter = 0;
    int layoutCounter = 0;
    int allVideosSize = all_videos.size();

    for(int i = 0; i < rows; i++)
    {
        QWidget* rowWidget = new QWidget();
        rowLayout[layoutCounter] = new QHBoxLayout(rowWidget);

        layout->addWidget(rowWidget);

        rowWidget->setContentsMargins(0,0,0,0);

        rowLayout[layoutCounter]->setContentsMargins(0,0,0,0);
        rowLayout[layoutCounter]->setSpacing(0);

        for(int i = 0; i < 3; i++)
        {
            videoWidget[counter] = new VideoWidget();
            videoPlayer[counter] = new QMediaPlayer();
            QSignalMapper* mapper1 = new QSignalMapper();
            QSignalMapper* mapper2 = new QSignalMapper();
            QSignalMapper* mapper3 = new QSignalMapper();

            rowLayout[layoutCounter]->addWidget(videoWidget[counter]);

            videoWidget[counter]->setFixedSize(600, 320);

            volume = 2;
            videoPlayer[counter]->setVolume(volume);
            if(allVideosSize > counter)
            {
                videoPlayer[counter]->setMedia(QUrl::fromLocalFile(QString::fromStdString(videoFolderPath + "/" + all_videos[counter])));

                mapper1->setMapping(videoWidget[counter], counter);
                mapper2->setMapping(videoWidget[counter], counter);
                mapper3->setMapping(videoWidget[counter], counter);

                connect(videoWidget[counter], SIGNAL(Mouse_Enter()), mapper1, SLOT(map()));
                connect(videoWidget[counter], SIGNAL(Mouse_Left()), mapper2, SLOT(map()));
                connect(videoWidget[counter], SIGNAL(Mouse_Pressed()), mapper3, SLOT(map()));
                connect(mapper1, SIGNAL(mapped(int)), this, SLOT(Mouse_enter(int)));
                connect(mapper2, SIGNAL(mapped(int)), this, SLOT(Mouse_left(int)));
                connect(mapper3, SIGNAL(mapped(int)), this, SLOT(Mouse_pressed(int)));
            }
            videoPlayer[counter]->setVideoOutput(videoWidget[counter]);

            counter++;
        }
        layoutCounter++;
    }
    setRandom = true;

    QWidget* bottomRowWidget = new QWidget();
    QHBoxLayout* bottomRowLayout = new QHBoxLayout(bottomRowWidget);

    layout->addWidget(bottomRowWidget);

    topRowWidget->setContentsMargins(5,0,5,0);
    bottomRowWidget->setContentsMargins(5,0,5,0);

    topRowLayout->setAlignment(Qt::AlignRight);
    topRowLayout->setContentsMargins(0,0,2,0);
    topRowLayout->setSpacing(5);
    bottomRowLayout->setContentsMargins(0,0,0,0);
    bottomRowLayout->setSpacing(0);

    topRowLayout->addWidget(blackLabel);
    topRowLayout->addWidget(upperBack);
    topRowLayout->addWidget(upperPage);
    topRowLayout->addWidget(upperNext);
    topRowLayout->addWidget(reloadButton);
    bottomRowLayout->addWidget(buttonBack);
    bottomRowLayout->addWidget(page);
    bottomRowLayout->addWidget(buttonNext);

    reloadButton->setFixedSize(30,30);
    upperBack->setFixedSize(30, 30);
    upperNext->setFixedSize(30, 30);
    reloadButton->setText("↻");
    upperBack->setText("<-");
    upperNext->setText("->");
    reloadButton->setPalette((QColor(30,30,30)));
    upperBack->setPalette(QColor(30,30,30));
    upperNext->setPalette(QColor(30,30,30));

    buttonBack->setFixedSize(860, 30);
    buttonNext->setFixedSize(860, 30);
    buttonBack->setText("Previous Page");
    buttonNext->setText("Next Page");
    buttonBack->setPalette(QColor(30,30,30));
    buttonNext->setPalette(QColor(30,30,30));

    QFont font = page->font();
    font.setPointSize(15);
    font.setBold(true);
    upperPage->setFont(font);
    upperPage->setText("1");
    upperPage->setAlignment(Qt::AlignCenter);
    page->setFont(font);
    page->setText("Page: 1");
    page->setAlignment(Qt::AlignCenter);

    blackLabel->setStyleSheet("background-color: rgb(40,40,40);");
    blackLabel->setFixedSize(1688, 30);

    connect(reloadButton, SIGNAL(clicked(bool)), this, SLOT(reload()));
    connect(upperBack, SIGNAL(clicked(bool)), SLOT(Previous_page(bool)));
    connect(upperNext, SIGNAL(clicked(bool)), SLOT(Next_page(bool)));
    connect(buttonBack, SIGNAL(clicked(bool)), SLOT(Previous_page(bool)));
    connect(buttonNext, SIGNAL(clicked(bool)), SLOT(Next_page(bool)));

    pageNumber = 1;

    QShortcut(QKeySequence(Qt::Key_Escape), this, SLOT(hide_all()));
}

player::~player()
{
}
void player::reload()
{
    setRandom = true;
}
void player::Previous_page(bool)
{
    if(pageNumber != 1)
    {
        QCursor::setPos(1915,100);
        if(files - pageNumber * 30 < 0)
        {
            int hide = (files - pageNumber * 30) * -1;
            hide = (hide / 3) * 3;
            for(int i = 0; i < hide; i++)
            {
                rowLayout[i / 3 + (10 - hide / 3)]->addWidget(videoWidget[30 - hide + i]);
            }
        }
        pageNumber -= 1;
        pageCounter -= 30;
        for(int i = 0; i < rows * 3; i++)
        {
            videoPlayer[i]->setMedia(QUrl::fromLocalFile(QString::fromStdString(videoFolderPath + "/" + all_videos[i + pageCounter])));
        }
        setRandom = true;
        upperPage->setText(QString::number(pageNumber));
        page->setText("Page: " + QString::number(pageNumber));
        scrollBar->setSliderDown(true);
        scrollBar->setSliderPosition(0);
    }
}
void player::Next_page(bool)
{
    if(files - pageNumber * 30 > 0)
    {
        QCursor::setPos(1915,100);
        pageNumber += 1;
        if(files - pageNumber * 30 < 0)
        {
            int hide = (files - pageNumber * 30) * -1;
            hide = (hide / 3) * 3;
            for(int i = 0; i < hide; i++)
            {
                rowLayout[9 - (i / 3)]->removeWidget(videoWidget[29 - i]);
            }
        }
        pageCounter += 30;
        for(int i = 0; i < rows * 3; i++)
        {
            videoPlayer[i]->setMedia(QUrl::fromLocalFile(QString::fromStdString(videoFolderPath + "/" + all_videos[i + pageCounter])));
        }
        setRandom = true;
        upperPage->setText(QString::number(pageNumber));
        page->setText("Page: " + QString::number(pageNumber));
        scrollBar->setSliderDown(true);
        scrollBar->setSliderPosition(0);
    }
}

void player::Set_random()
{
    if(setRandom == true)
    {
        setRandom = false;
        int d = 29;
        if(files < 30)
        {
            d = files - 1;
        }
        for(int i = d; i > -1; i--)
        {
            int number = rand() % 100;
            if(number < minimalFrame)
            {
                number = minimalFrame;
            }
            int duration = videoPlayer[i]->duration();
            videoPlayer[i]->setPosition(duration * number / 100);
            videoPlayer[i]->pause();
        }
    }
}

void player::Play_random(int randomNumber)
{
    int duration = videoPlayer[playerNumber]->duration();
    videoPlayer[playerNumber]->setPosition(duration * randomNumber / 100);
}

void player::Mouse_enter(int counter)
{
    playerNumber = counter;
    videoPlayer[counter]->play();
    thread->Stop = false;
    thread->start();
    Set_random();
}

void player::Mouse_left(int counter)
{
    thread->Stop = true;
    int number = rand() % 100;
    if(number < minimalFrame)
    {
        number = minimalFrame;
    }
    int duration = videoPlayer[counter]->duration();
    videoPlayer[counter]->setPosition(duration * number / 100);
    videoPlayer[counter]->pause();
}

void player::Mouse_pressed(int counter)
{
    currentVideo = counter;

    QWidget* mainWidget = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(mainWidget);
    mainSlider = new Slider();
    mainVideoPlayer = new QMediaPlayer();
    QWidget* horizontalWidget = new QWidget();
    QHBoxLayout* horizonatalLayout = new QHBoxLayout(horizontalWidget);
    QWidget* verticalWidget = new QWidget();
    QVBoxLayout* verticalLayout = new QVBoxLayout(verticalWidget);

    mainWindow->setCentralWidget(mainWidget);
    mainWindow->setGeometry(0,0,1850,900);
    mainWindow->show();

    mainWidget->setAutoFillBackground(true);
    mainWidget->setPalette(backgroundPalette);

    mainLayout->addWidget(horizontalWidget);
    mainLayout->addWidget(mainSlider);
    mainLayout->setContentsMargins(0,0,0,0);

    horizonatalLayout->addWidget(mainVideoWidget);
    horizonatalLayout->addWidget(verticalWidget);
    horizonatalLayout->setContentsMargins(0,0,0,0);

    verticalLayout->addWidget(volumeSlider);
    verticalLayout->addWidget(label);
    verticalLayout->setContentsMargins(0,0,0,0);

    volume = 20;

    QFont font = label->font();
    font.setBold(true);

    label->setFont(font);
    label->setPalette(textPalette);
    label->setFixedSize(20,650);
    label->setText(QString::number(volume));
    label->setAlignment(Qt::AlignTop);
    label->setAlignment(Qt::AlignHCenter);

    volumeSlider->setFixedWidth(20);
    volumeSlider->setSliderPosition(volume);
    volumeSlider->setPalette(sliderPalette);

    mainSlider->setFixedHeight(20);
    mainSlider->setOrientation(Qt::Horizontal);
    mainSlider->setRange(0, 10000);
    mainSlider->setMouseTracking(true);
    mainSlider->setPalette(sliderPalette);

    smallWidget = new VideoWidget();
    smallPlayer = new QMediaPlayer();

    smallWidget->setGeometry(0, 850, 300, 200);

    smallPlayer->setVideoOutput(smallWidget);
    smallPlayer->setMedia(QUrl::fromLocalFile(QString::fromStdString(videoFolderPath + "/" + all_videos[counter + pageCounter])));
    smallPlayer->setMuted(true);

    mainVideoPlayer->setVolume(volume);
    mainVideoPlayer->setMedia(QUrl::fromLocalFile(QString::fromStdString(videoFolderPath + "/" + all_videos[counter + pageCounter])));
    mainVideoPlayer->setVideoOutput(mainVideoWidget);
    mainVideoPlayer->play();
    play = true;
    start = steady_clock::now();
    point = 0;

    mainWindowExists = true;

    up->setParent(mainWindow);
    down->setParent(mainWindow);
    scroll->setParent(mainWindow);
    rewind->setParent(mainWindow);
    space->setParent(mainWindow);
    hideMain->setParent(mainWindow);

    timerChecker->start(250000);

    connect(timerChecker, SIGNAL(timeout()), this, SLOT(timer_remid()));
    connect(mainWindow, SIGNAL(Close_Event()), this, SLOT(Exit_player()));
    connect(mainVideoPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(Auto_move_slider(qint64)));
    connect(mainSlider, SIGNAL(Mouse_Click(int)), this, SLOT(Manual_move_slider(int)));
    connect(mainSlider, SIGNAL(Mouse_Enter()), this, SLOT(Slider_enter()));
    connect(mainSlider, SIGNAL(Mouse_Left()), this, SLOT(Slider_left()));
    connect(mainSlider, SIGNAL(Mouse_In()), this, SLOT(Slider_in()));
    connect(volumeSlider, SIGNAL(sliderMoved(int)), this, SLOT(Move_slider(int)));
}

void player::Volume_up()
{
    if(19 < volume && volume < 91)
    {
        volume += 10;
    }
    if(4 < volume && volume < 20)
    {
        volume += 5;
    }
    if(volume < 5)
    {
        volume += 1;
    }
    mainVideoPlayer->setVolume(volume);
    volumeSlider->setSliderPosition(volume);
    label->setText(QString::number(volume));
}

void player::Volume_down()
{
    if(0 < volume && volume < 6)
    {
        volume -= 1;
    }
    if(5 < volume && volume < 21)
    {
        volume -= 5;
    }
    if(20 < volume)
    {
        volume -= 10;
    }
    mainVideoPlayer->setVolume(volume);
    volumeSlider->setSliderPosition(volume);
    label->setText(QString::number(volume));
}

void player::Scroll()
{
    int position = mainVideoPlayer->position();
    mainVideoPlayer->setPosition(position + 10000);
}

void player::Rewind()
{
    int position = mainVideoPlayer->position();
    mainVideoPlayer->setPosition(position - 10000);
}

void player::Play_stop()
{
    if(play == true)
    {
        play = false;
        mainVideoPlayer->pause();
        end = steady_clock::now();
        point = point + duration_cast<seconds>(end - start).count();
    }
    else
    {
        play = true;
        mainVideoPlayer->play();
        start = steady_clock::now();
    }
}

void player::Exit_player()
{
    timerChecker->stop();
    if(play == true)
    {
        end = steady_clock::now();
        point = point + duration_cast<seconds>(end - start).count();
    }
    point = point / 10;
    points[currentVideo + pageCounter] = point + points[currentVideo + pageCounter];
    QCursor::setPos(1915,100);
    int counter = 0;
    while(true)
    {
        if(currentVideo + pageCounter - counter == 0)
        {
            break;
        }
        if(points[currentVideo + pageCounter - counter] > points[currentVideo + pageCounter - counter - 1])
        {
            swap(all_videos[currentVideo + pageCounter - counter], all_videos[currentVideo + pageCounter - counter - 1]);
            swap(points[currentVideo + pageCounter - counter], points[currentVideo + pageCounter - counter - 1]);
            counter++;
        }
        else
        {
            break;
        }
    }
    ofstream file(dataFolderPath + "/videos.txt");
    for(int i = 0; i < files; i++)
    {
        file << "points: " << points[i] << "; name: " + all_videos[i] + "\n";
    }
    file.close();
    mainVideoPlayer->deleteLater();
    int allVideosSize = all_videos.size();
    for(int i = 0; i < rows * 3; i++)
    {
        if(allVideosSize > i)
        {
            videoPlayer[i]->setMedia(QUrl::fromLocalFile(QString::fromStdString(videoFolderPath + "/" + all_videos[i + pageCounter])));
        }
    }
    setRandom = true;
    scrollBar->setSliderPosition(0);
    disconnect(mainWindow, SIGNAL(Close_Event()), this, SLOT(Exit_player()));
    mainWindowExists = false;
}

void player::Auto_move_slider(qint64 position)
{
    mainSlider->setSliderPosition(position * 10000 / mainVideoPlayer->duration());
}

void player::Manual_move_slider(int position)
{
    mainVideoPlayer->setPosition(position * mainVideoPlayer->duration() / 10000);
}

void player::Slider_enter()
{
    smallWidget->show();
}

void player::Slider_left()
{
    smallWidget->hide();
}

void player::Slider_in()
{
    smallPlayer->play();
    QPoint position = mainSlider->mapFromGlobal(QCursor::pos());
    smallWidget->setGeometry(position.x() - 80, 850, 300, 200);

    ///if smallPlayer is faster than mainVideoPlayer on first half of the video -> increase 'shrink'
    ///if smallPlayer is slower than mainVideoPlayer on second half of the video -> increase 'shrink'
    ///if both are slower -> increase 'move'

    qint64 duration = smallPlayer->duration();
    qint64 shrink = 9.2;
    qint64 move = 1;
    qint64 shrinkBis = 1850 - shrink * 2;

    smallPlayer->setPosition(duration / shrinkBis * (position.x() - shrink) + duration / shrinkBis * move);
    smallPlayer->pause();
    milliseconds dura(50);
    this_thread::sleep_for(dura);
}

void player::Move_slider(int position)
{
    volume = position;
    mainVideoPlayer->setVolume(volume);
    label->setText(QString::number(volume));
}

void player::hide_all()
{
    if(mainWindowExists == true)
    {
        if(play == true)
        {
            Play_stop();
        }
        mainWindow->showMinimized();
    }
    showMinimized();
}
void player::timer_remid()
{
    system("xdotool click --repeat 1 2");
    timerChecker->start(250000);
}
