#include "QtCameraCapture.h"
#include <QTime>

QtCameraCapture::QtCameraCapture(QObject *parent) : QAbstractVideoSurface(parent)
{

}

QList<QVideoFrame::PixelFormat> QtCameraCapture::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
{
    Q_UNUSED(handleType);
    return QList<QVideoFrame::PixelFormat>()
            << QVideoFrame::Format_ARGB32
            << QVideoFrame::Format_ARGB32_Premultiplied
            << QVideoFrame::Format_RGB32
            << QVideoFrame::Format_RGB24
            << QVideoFrame::Format_RGB565
            << QVideoFrame::Format_RGB555
            << QVideoFrame::Format_ARGB8565_Premultiplied
            << QVideoFrame::Format_BGRA32
            << QVideoFrame::Format_BGRA32_Premultiplied
            << QVideoFrame::Format_BGR32
            << QVideoFrame::Format_BGR24
            << QVideoFrame::Format_BGR565
            << QVideoFrame::Format_BGR555
            << QVideoFrame::Format_BGRA5658_Premultiplied
            << QVideoFrame::Format_AYUV444
            << QVideoFrame::Format_AYUV444_Premultiplied
            << QVideoFrame::Format_YUV444
            << QVideoFrame::Format_YUV420P
            << QVideoFrame::Format_YV12
            << QVideoFrame::Format_UYVY
            << QVideoFrame::Format_YUYV
            << QVideoFrame::Format_NV12
            << QVideoFrame::Format_NV21
            << QVideoFrame::Format_IMC1
            << QVideoFrame::Format_IMC2
            << QVideoFrame::Format_IMC3
            << QVideoFrame::Format_IMC4
            << QVideoFrame::Format_Y8
            << QVideoFrame::Format_Y16
            << QVideoFrame::Format_Jpeg
            << QVideoFrame::Format_CameraRaw
            << QVideoFrame::Format_AdobeDng;
}

bool QtCameraCapture::present(const QVideoFrame &frame)
{
    if (frame.isValid()) {
        //QTime timedbg;
        //timedbg.start();
        QVideoFrame cloneFrame(frame);
        emit frameAvailable(cloneFrame);
        //qDebug()<< "present = "<< timedbg.elapsed()<<"ms";
        //qDebug()<< "present" <<cloneFrame.pixelFormat();
        return true;
    }
    return false;
}

void QtCameraCapture::Cvt_yuv_to_rgb(unsigned char *yuv, unsigned char* rgb,  int image_width, int image_height)
{
    unsigned char* y0 = yuv + 0;
    unsigned char* u0 = yuv + 1;
    unsigned char* y1 = yuv + 2;
    unsigned char* v0 = yuv + 3;

    unsigned  char* r0 = rgb + 0;
    unsigned  char* g0 = rgb + 1;
    unsigned  char* b0 = rgb + 2;
    unsigned  char* r1 = rgb + 3;
    unsigned  char* g1 = rgb + 4;
    unsigned  char* b1 = rgb + 5;

    float rt0 = 0, gt0 = 0, bt0 = 0, rt1 = 0, gt1 = 0, bt1 = 0;

    for(int i = 0; i <= (image_width * image_height) / 2 ;i++)
    {
        bt0 = 1.164 * (*y0 - 16) + 2.018 * (*u0 - 128);
        gt0 = 1.164 * (*y0 - 16) - 0.813 * (*v0 - 128) - 0.394 * (*u0 - 128);
        rt0 = 1.164 * (*y0 - 16) + 1.596 * (*v0 - 128);

        bt1 = 1.164 * (*y1 - 16) + 2.018 * (*u0 - 128);
        gt1 = 1.164 * (*y1 - 16) - 0.813 * (*v0 - 128) - 0.394 * (*u0 - 128);
        rt1 = 1.164 * (*y1 - 16) + 1.596 * (*v0 - 128);

        if(rt0 > 250)   rt0 = 255;
        if(rt0 < 0)     rt0 = 0;

        if(gt0 > 250)  gt0 = 255;
        if(gt0 < 0) gt0 = 0;

        if(bt0 > 250) bt0 = 255;
        if(bt0 < 0) bt0 = 0;

        if(rt1 > 250) rt1 = 255;
        if(rt1 < 0) rt1 = 0;

        if(gt1 > 250) gt1 = 255;
        if(gt1 < 0) gt1 = 0;

        if(bt1 > 250) bt1 = 255;
        if(bt1 < 0) bt1 = 0;

        *r0 = (unsigned char)rt0;
        *g0 = (unsigned char)gt0;
        *b0 = (unsigned char)bt0;

        *r1 = (unsigned char)rt1;
        *g1 = (unsigned char)gt1;
        *b1 = (unsigned char)bt1;

        yuv = yuv + 4;
        rgb = rgb + 6;
        if(yuv == NULL)
          break;

        y0 = yuv;
        u0 = yuv + 1;
        y1 = yuv + 2;
        v0 = yuv + 3;

        r0 = rgb + 0;
        g0 = rgb + 1;
        b0 = rgb + 2;
        r1 = rgb + 3;
        g1 = rgb + 4;
        b1 = rgb + 5;
    }
    return;
}

QImage QtCameraCapture::CvtQVideoFrameToQImage(QVideoFrame &frame)
{
   // qDebug()<< "QVideoFrameCvtQImage" <<frame;
    frame.map(QAbstractVideoBuffer::ReadOnly);
    QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat());
    //qDebug()<< "QVideoFrameCvtQImage" <<imageFormat;
    QImage img;
    if (imageFormat != QImage::Format_Invalid) {
        img = QImage(frame.bits(),
                     frame.width(),
                     frame.height(),
                     // cloneFrame.bytesPerLine(),
                     imageFormat);
    }
    else {
        // e.g. JPEG
        int nbytes = frame.mappedBytes();
        img = QImage::fromData(frame.bits(), nbytes);
        //qDebug()<< "QVideoFrameCvtQImage" <<img;
    }
    frame.unmap();
    return img;
}

int QtCameraCapture::CvtQVideoFrameYUYVToQImageRGB888(QImage *frameOut, QVideoFrame *frame)
{
    if(frame->pixelFormat() != QVideoFrame::Format_YUYV) return -1;

    frame->map(QAbstractVideoBuffer::ReadOnly);
    QtCameraCapture::Cvt_yuv_to_rgb((unsigned char *)frame->bits(), (unsigned char *)frameOut->bits(), frame->width(), frame->height());
    frame->unmap();
    return 0;
}
