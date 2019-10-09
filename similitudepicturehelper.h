#ifndef SIMILITUDEPICTUREHELPER_H
#define SIMILITUDEPICTUREHELPER_H

#include <QImage>
#include <QDebug>


class SimilitudePictureHelper
{
public:
    SimilitudePictureHelper();

    void stop();

    bool isSimilitude(QImage img1, QImage img2, int weight); // 是否相似, 第三个参数权重，当汉明距离超过这个数返回false
    bool isSimilitude(QString imgHash, QImage img2, int weight); // 是否相似, 根据一张图片的hash判断
    bool isPicture(const QString& filePath); // 是否为图片文件
    QString toHash(QImage image); // 获得一张图片的hash值

private:
    QImage toGray(QImage image);
    double g_avg_pixel(QImage image); //计算平均灰度
    QString toHash(QImage image, const double& avg_pixel); //获取hash值
    int getHamming(const QString& str1, const QString& str2); //计算两个字符串的汉明距离

public:
    const int scaled_w = 10;
    const int scaled_h = 10;

};

#endif // SIMILITUDEPICTUREHELPER_H
