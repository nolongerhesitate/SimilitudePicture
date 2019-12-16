#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDateTime>
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

    connect(&mwt,&MainWindowThread::isSimilitudeSingal,this,&MainWindow::isSimilitudeSlot);
    connect(&mwt,&MainWindowThread::progressBarSingal,this,&MainWindow::progressBarSlot);

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

    QImage img;
    if(!img.load(imgPath)){
        QMessageBox::warning(this,"错误","要判断的图片不存在");
        return;
    }

    ui->selImgBtn->setEnabled(false);
    ui->selDirBtn->setEnabled(false);
    ui->imgLineEdit->setEnabled(false);
    ui->dirLineEdit->setEnabled(false);
    ui->textEdit->setEnabled(false);
    ui->pushButton->setEnabled(false);
    ui->cancelBtn->setEnabled(true);

    ui->progressBar->setVisible(true);
    ui->textEdit->clear();
    ui->progressBar->setValue(0);


    QDir dir(dirPath);
    dir.setFilter(QDir::Files);//除了文件，其他的过滤掉
    mwt.list = dir.entryInfoList();//获取文件信息列表
    mwt.img = img;
    ui->progressBar->setMaximum(mwt.list.size());  // 最大值
    mwt.start();
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
        if(!sph->isPicture(fileDialog->selectedFiles()[0])) {
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

void MainWindow::isSimilitudeSlot(const QString& filepath)
{
    ui->textEdit->append(filepath);
}

void MainWindow::progressBarSlot()
{
    ui->progressBar->setValue(ui->progressBar->value()+1);

    if(ui->progressBar->value() == ui->progressBar->maximum())
    {
        ui->selImgBtn->setEnabled(true);
        ui->selDirBtn->setEnabled(true);
        ui->imgLineEdit->setEnabled(true);
        ui->dirLineEdit->setEnabled(true);
        ui->textEdit->setEnabled(true);
        ui->pushButton->setEnabled(true);
        ui->cancelBtn->setEnabled(false);
    }
}

void MainWindow::on_cancelBtn_clicked()
{
    mwt.stop();
    ui->progressBar->setValue(0);
    ui->selImgBtn->setEnabled(true);
    ui->selDirBtn->setEnabled(true);
    ui->imgLineEdit->setEnabled(true);
    ui->dirLineEdit->setEnabled(true);
    ui->textEdit->setEnabled(true);
    ui->pushButton->setEnabled(true);
    ui->cancelBtn->setEnabled(false);
}

void MainWindow::on_imgLineEdit_editingFinished()
{
    imgPath = ui->imgLineEdit->text();
}
