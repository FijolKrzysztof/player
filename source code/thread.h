#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QMediaPlayer>

class Thread : public QThread
{
    Q_OBJECT

public:
    explicit Thread(QObject* parent = 0, bool isStopped = false);

    void run();
    bool Stop;

signals:
    void valueChanged(int);
};

extern int minimalFrame;

#endif // THREAD_H
