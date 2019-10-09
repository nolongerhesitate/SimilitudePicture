#include "mainwindowthread.h"

#include <QDateTime>
#include <QDir>

MainWindowThread::MainWindowThread()
{
    stopped = false;
}

void MainWindowThread::_check(QImage tmpImg,  QString filePath)
{
    if(sph.isSimilitude(imgHash,tmpImg,weight))
        emit isSimilitudeSingal(filePath);
}

void MainWindowThread::run()
{
//    qint64 startTime= QDateTime::currentDateTime().toMSecsSinceEpoch();
    imgHash = sph.toHash(img);
    for(int i = 0; i< list.size();i++)
    {
        if(stopped) break;
        if(!sph.isPicture(list[i].filePath())) {
            emit progressBarSingal();
        }else {
            QImage tmpImg;
            tmpImg.load(list[i].filePath());
            QtConcurrent::run(this,&MainWindowThread::_check, tmpImg, list[i].filePath());
            emit progressBarSingal();
        }
    }

//    qDebug()<< "=== " << (QDateTime::currentDateTime().toMSecsSinceEpoch()- startTime)<<endl;
}

void MainWindowThread::stop()
{
    stopped = true;
}

