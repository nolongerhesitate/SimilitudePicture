#ifndef MAINWINDOWTHREAD_H
#define MAINWINDOWTHREAD_H

#include "similitudepicturehelper.h"

#include <QThread>
#include <QDebug>
#include <QImage>
#include <QFileInfoList>
#include <QtConcurrent>



class MainWindowThread : public QThread
{
    Q_OBJECT
public:
    MainWindowThread();
    void stop();

private:
    void _check(QImage tmpImg,  QString filePath);

protected:
    void run() override;


signals:
    void isSimilitudeSingal(const QString& isSimilitude);
    void progressBarSingal();

public:
    QImage img;
    QString imgHash;
    int weight = 4;
    QFileInfoList list;

private:
    volatile bool stopped;
    SimilitudePictureHelper sph;
};

#endif // MAINWINDOWTHREAD_H
