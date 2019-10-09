#include "similitudepicturehelper.h"

#include <QPixmap>

SimilitudePictureHelper::SimilitudePictureHelper()
{

}

QImage SimilitudePictureHelper::toGray( QImage image )
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

double SimilitudePictureHelper::g_avg_pixel(QImage image)
{
    double avg_pixel=0;
    int height = image.height();
    int width = image.width();

    for(int row=0;row<height; row++){
        for (int cols =0;cols<width;cols++) {
            QRgb rgb = image.pixel(cols,row);
            avg_pixel += rgb;
        }
    }
    return avg_pixel/(height*width);
}

QString SimilitudePictureHelper::toHash(QImage image, const double& avg_pixel)
{
    QString hash = "";
    int height = image.height();
    int width = image.width();
    for(int row=0;row<height; row++){
        for (int cols =0;cols<width;cols++) {
            QRgb rgb = image.pixel(cols,row);
            hash += rgb>=avg_pixel?"1":"0";
        }
    }
    return hash;
}

int SimilitudePictureHelper::getHamming(const QString& str1, const QString& str2)
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

bool SimilitudePictureHelper::isSimilitude(QImage img1, QImage img2, int weight)
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

    QImage res1 = toGray(img1/*.scaled(800,800,Qt::KeepAspectRatio,Qt::SmoothTransformation)*/.scaled(scaled_w,scaled_h,Qt::KeepAspectRatio,Qt::FastTransformation));
    double avg1 = g_avg_pixel(res1);
    QString hash1 = toHash(res1,avg1);
    QImage res2 = toGray(img2/*.scaled(800,800,Qt::KeepAspectRatio,Qt::SmoothTransformation)*/.scaled(scaled_w,scaled_h,Qt::KeepAspectRatio,Qt::FastTransformation));
    double avg2 = g_avg_pixel(res2);
    QString hash2 = toHash(res2,avg2);
    return !(getHamming(hash1,hash2) > weight);
    //    bool success = res.save("C:\\Users\\Administrator\\Desktop\\s.png","PNG");
}

bool SimilitudePictureHelper::isPicture(const QString &filePath)
{
    QPixmap pixs;
    pixs.load(filePath);
    return !(pixs.isNull());
}

QString SimilitudePictureHelper::toHash(QImage image)
{
    QImage res = toGray(image.scaled(scaled_w,scaled_h,Qt::KeepAspectRatio,Qt::FastTransformation));
    double avg = g_avg_pixel(res);
    return toHash(res,avg);
}

bool SimilitudePictureHelper::isSimilitude(QString imgHash, QImage img2, int weight)
{
    return !(getHamming(imgHash,toHash(img2))>weight);
}

