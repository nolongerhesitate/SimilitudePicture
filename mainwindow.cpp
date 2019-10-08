#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QImage img1;
    img1.load("C:\\Users\\Administrator\\Desktop\\49.png");
    QImage img2;
    img2.load("C:\\Users\\Administrator\\Desktop\\21.png");

    if(isSimilitude(img1,img2,10)){
        QMessageBox::information(this,"相似","两张图片相似");
    }else
    {
        QMessageBox::information(this,"不同","两张图片不相同");
    }
}



QImage MainWindow::toGray( QImage image )
{
    int height = image.height();
    int width = image.width();
    QImage ret(width, height, QImage::Format_Indexed8);
    ret.setColorCount(256);
    for(int i = 0; i < 256; i++)
    {
        ret.setColor(i, qRgb(i, i, i));
    }
    switch(image.format())
    {
    case QImage::Format_Indexed8:
        for(int i = 0; i < height; i ++)
        {
            const uchar *pSrc = (uchar *)image.constScanLine(i);
            uchar *pDest = (uchar *)ret.scanLine(i);
            memcpy(pDest, pSrc, width);
        }
        break;
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
        for(int i = 0; i < height; i ++)
        {
            const QRgb *pSrc = (QRgb *)image.constScanLine(i);
            uchar *pDest = (uchar *)ret.scanLine(i);

            for( int j = 0; j < width; j ++)
            {
                pDest[j] = qGray(pSrc[j]);
            }
        }
        break;
    }
    return ret;
}

double MainWindow::g_avg_pixel(QImage image)
{
    double avg_pixel=0;
    int height = image.height();
    int width = image.width();

    for(int row=0;row<height; row+=1){
        for (int cols =0;cols<width;cols+=1) {
           QRgb rgb = image.pixel(row,cols);
           qDebug()<<"rgb === "<<rgb<<endl;
           avg_pixel += rgb;
        }
    }
    return avg_pixel/(height*width);
}

QString MainWindow::toHash(QImage image, const double& avg_pixel)
{
    QString hash = "";
    int height = image.height();
    int width = image.width();
    for(int row=0;row<height; row+=1){
        for (int cols =0;cols<width;cols+=1) {
           QRgb rgb = image.pixel(row,cols);
           hash += rgb>=avg_pixel?"1":"0";
        }
    }
    return hash;
}

int MainWindow::getHamming(const QString& str1, const QString& str2)
{
    int length = str1.size()<= str2.size()?str1.size():str2.size();
    int hamming = qAbs(str1.size()-str2.size());

    for(int i = 0; i<length;i++)
    {
        if(str1[i] != str2[i])
            hamming++;
    }
    return hamming;
}

bool MainWindow::isSimilitude(QImage img1, QImage img2, int weight)
{
    //QImage res = toGray(img/*.scaled(800,800,Qt::KeepAspectRatio,Qt::FastTransformation)*/.scaled(10,10,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    /*
    double avg = g_avg_pixel(res);
    QString hash = toHash(res,avg);
    ui->lineEdit_2->setText(hash);
    ui->lineEdit->setText(QString::number(avg,'f', 30));
    ui->label->setText(QString::number(hamm));
    int hamm = getHamming("1111000000111100000011110000001111000000",hash);
    bool success = res.save("C:\\Users\\Administrator\\Desktop\\s.png","PNG");
    */

    QImage res1 = toGray(img1.scaled(800,800,Qt::KeepAspectRatio,Qt::FastTransformation).scaled(10,10,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    double avg1 = g_avg_pixel(res1);
    QString hash1 = toHash(res1,avg1);
    QImage res2 = toGray(img2.scaled(800,800,Qt::KeepAspectRatio,Qt::FastTransformation).scaled(10,10,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    double avg2 = g_avg_pixel(res2);
    QString hash2 = toHash(res2,avg2);
    return !(getHamming(hash1,hash2) > weight);
//    bool success = res.save("C:\\Users\\Administrator\\Desktop\\s.png","PNG");
}

