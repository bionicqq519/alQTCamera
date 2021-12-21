#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCameraInfo>
#include <QDebug>
#include <qmessagebox.h>

#include <QTime>

#include "DllCall.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    foreach(const QCameraInfo &cameraInfo, cameras) {
        qDebug() << "CameraInfo:" << cameraInfo;
    }
    ui->testButton->setDisabled(false);
    connect(ui->testButton, SIGNAL(clicked()), this, SLOT(testCallback()));

    // 開啟camera並設定QtCameraCapture類別，不一定要用Qcamera也可以用像是QMediaPlayer來開啟
    m_camera = new QCamera();

    // 對camera進行設定
    QCameraViewfinderSettings VfSettings;
    //VfSettings.setResolution(preferred_resolution);
    VfSettings.setPixelFormat(QVideoFrame::Format_YUYV);//QVideoFrame::Format_YUYV);
    //VfSettings.setMaximumFrameRate(15);
    m_camera->setViewfinderSettings(VfSettings);//設定當前相機的取景器

    // 設定m_cameraCapture來取得影像
    m_cameraCapture = new QtCameraCapture;
    m_camera->setViewfinder(m_cameraCapture);
    connect(m_cameraCapture, SIGNAL(frameAvailable(QVideoFrame&)), this, SLOT(grabImage(QVideoFrame&)));
    m_camera->start();
//    camera = new QCamera(this);
//    viewfind = new QCameraViewfinder();
//    viewfind->show();
//    camera->setViewfinder(viewfind);
//    camera->start();

    dllCall();
    qDebug()<<"MainWindow";
}

MainWindow::~MainWindow()
{
    m_camera->stop();
    delete m_camera;
    delete m_cameraCapture;
    delete ui;
}

void MainWindow::testCallback()
{
    QMessageBox::information(this, tr("Title"),"message");
    return;
}

void MainWindow::grabImage(QVideoFrame &frame)
{
    // qDebug()<<"image"<<frame ;
    QTime timedbg;
    timedbg.start();
    //ui->imageView->setPixmap(pm.scaled(ui->imageView->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    QImage *img = new QImage(frame.width(), frame.height(), QImage::Format_RGB888);
    //QImage img = QtCameraCapture::CvtQVideoFrameToQImage(frame);
    if(QtCameraCapture::CvtQVideoFrameYUYVToQImageRGB888(img, &frame) != 0)
    {
        qDebug()<<"Error to Cvt";
    }
    QPixmap pm = QPixmap::fromImage(*img);
    ui->imageView->setPixmap(pm);
    qDebug()<< "grabImage = "<< timedbg.elapsed()<<"ms";
    delete img;
    return;
}

