#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->setVisible(false);
    ui->progressBar->setMinimum(0); // 最小值
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if(imgPath.isEmpty() || imgPath.isNull()){
        QMessageBox::warning(this,"错误","先选择图片");
        return;
    }
    if(dirPath.isEmpty() || dirPath.isNull()){
        QMessageBox::warning(this,"错误","先选择文件夹");
        return;
    }

    ui->selImgBtn->setEnabled(false);
    ui->selDirBtn->setEnabled(false);
    ui->imgLineEdit->setEnabled(false);
    ui->dirLineEdit->setEnabled(false);
    ui->textEdit->setEnabled(false);
    ui->pushButton->setEnabled(false);

    ui->progressBar->setVisible(true);
    /*
    qDebug()<<imgPath<<endl;
    qDebug()<<dirPath<<endl;
    */
    QImage img;
    img.load(imgPath);

    QDir dir(dirPath);
    dir.setFilter(QDir::Files);//除了文件，其他的过滤掉
    QFileInfoList list = dir.entryInfoList();//获取文件信息列表
    ui->progressBar->setMaximum(list.size());  // 最大值
    for(int i = 0; i< list.size();i++){
        ui->progressBar->setValue(i);
        if(!isPicture(list[i].filePath())) continue;
        QImage tmpImg;
        tmpImg.load(list[i].filePath());
        qDebug()<<i<<endl;
        if(isSimilitude(img,tmpImg,10)){
            ui->textEdit->append(list[i].filePath());
        }
    }

    ui->progressBar->setValue(list.size());

    ui->selImgBtn->setEnabled(true);
    ui->selDirBtn->setEnabled(true);
    ui->imgLineEdit->setEnabled(true);
    ui->dirLineEdit->setEnabled(true);
    ui->textEdit->setEnabled(true);
    ui->pushButton->setEnabled(true);
    /*
    if(isSimilitude(img1,img2,10)){
        QMessageBox::information(this,"相似","两张图片相似");
    }else
    {
        QMessageBox::information(this,"不同","两张图片不相同");
    }
    */
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

    for(int row=0;row<height; row++){
        for (int cols =0;cols<width;cols++) {
            QRgb rgb = image.pixel(cols,row);
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
    for(int row=0;row<height; row++){
        for (int cols =0;cols<width;cols++) {
            QRgb rgb = image.pixel(cols,row);
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

    QImage res1 = toGray(img1/*.scaled(800,800,Qt::KeepAspectRatio,Qt::FastTransformation)*/.scaled(10,10,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    double avg1 = g_avg_pixel(res1);
    QString hash1 = toHash(res1,avg1);
    QImage res2 = toGray(img2/*.scaled(800,800,Qt::KeepAspectRatio,Qt::FastTransformation)*/.scaled(10,10,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    double avg2 = g_avg_pixel(res2);
    QString hash2 = toHash(res2,avg2);
    return !(getHamming(hash1,hash2) > weight);
    //    bool success = res.save("C:\\Users\\Administrator\\Desktop\\s.png","PNG");
}

bool MainWindow::isPicture(const QString &filePath)
{
    QPixmap pixs;
    pixs.load(filePath);
    return !(pixs.isNull());
}


void MainWindow::on_selImgBtn_clicked()
{
    //定义文件对话框类
    QFileDialog *fileDialog = new QFileDialog(this);
    //定义文件对话框标题
    fileDialog->setWindowTitle(tr("打开图片"));
    //设置默认文件路径
//    fileDialog->setDirectory(".");
    //设置文件过滤器
    //    fileDialog->setNameFilter(tr("Images(*.png *.jpg *.jpeg *.bmp)"));
    //设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);
    if(fileDialog->exec())
    {
        if(!isPicture(fileDialog->selectedFiles()[0])) {
            QMessageBox::warning(this,"错误","请选择图片!");
            return;
        }
        imgPath = fileDialog->selectedFiles()[0];
        ui->imgLineEdit->setText(imgPath);
    }

}

void MainWindow::on_selDirBtn_clicked()
{
    dirPath =  QFileDialog::getExistingDirectory (this, tr("Open Directory"), "/home", QFileDialog::ShowDirsOnly );
    ui->dirLineEdit->setText(dirPath);
}
