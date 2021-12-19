#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCamera>
#include <QCameraViewfinder>
#include "QtCameraCapture.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QCamera *camera;
    QCameraViewfinder *viewfind;

    QCamera *m_camera;
    QtCameraCapture *m_cameraCapture;
private slots:
    void testCallback();
    void grabImage(QVideoFrame &frame);
};
#endif // MAINWINDOW_H
