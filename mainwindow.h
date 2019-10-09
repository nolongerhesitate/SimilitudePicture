#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mainwindowthread.h"
#include "similitudepicturehelper.h"

#include <QMainWindow>
#include <QDebug>
#include <QList>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QImage toGray(QImage image);
    double g_avg_pixel(QImage image); //计算平均灰度
    QString toHash(QImage image, const double& avg_pixel); //获取hash值
    int getHamming(const QString& str1, const QString& str2); //计算两个字符串的汉明距离
    bool isSimilitude(QImage img1, QImage img2, int weight); // 是否相似, 第三个参数权重，当汉明距离超过这个数返回false
    bool isPicture(const QString& filePath); // 是否为图片文件

private slots:
    void on_pushButton_clicked();

    void on_selImgBtn_clicked();

    void on_selDirBtn_clicked();

    void isSimilitudeSlot(const QString& isSimilitude);

    void progressBarSlot();

    void on_cancelBtn_clicked();

private:
    Ui::MainWindow *ui;
    QString imgPath;
    QString dirPath;
    SimilitudePictureHelper *sph;
    MainWindowThread mwt;
};
#endif // MAINWINDOW_H
