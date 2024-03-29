#ifndef QTCAMERACAPTURE_H
#define QTCAMERACAPTURE_H

#include <QObject>
#include <QAbstractVideoSurface>
#include <QDebug>

class QtCameraCapture : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    enum PixelFormat {
        Format_Invalid,
        Format_ARGB32,
        Format_ARGB32_Premultiplied,
        Format_RGB32,
        Format_RGB24,
        Format_RGB565,
        Format_RGB555,
        Format_ARGB8565_Premultiplied,
        Format_BGRA32,
        Format_BGRA32_Premultiplied,
        Format_BGR32,
        Format_BGR24,
        Format_BGR565,
        Format_BGR555,
        Format_BGRA5658_Premultiplied,

        Format_AYUV444,
        Format_AYUV444_Premultiplied,
        Format_YUV444,
        Format_YUV420P,
        Format_YV12,
        Format_UYVY,
        Format_YUYV,
        Format_NV12,
        Format_NV21,
        Format_IMC1,
        Format_IMC2,
        Format_IMC3,
        Format_IMC4,
        Format_Y8,
        Format_Y16,

        Format_Jpeg,

        Format_CameraRaw,
        Format_AdobeDng,

#ifndef Q_QDOC
        NPixelFormats,
#endif
        Format_User = 1000
    };

    Q_ENUM(PixelFormat)

    explicit QtCameraCapture(QObject *parent = 0);

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
            QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const override;

    bool present(const QVideoFrame &frame) override;

    static int CvtQVideoFrameYUYVToQImageRGB888(QImage *frameOut, QVideoFrame *frame);
    static QImage CvtQVideoFrameToQImage(QVideoFrame &frame);
    static void Cvt_yuv_to_rgb(unsigned char *yuv, unsigned char* rgb,  int image_width, int image_height);
signals:
    void frameAvailable(QVideoFrame &frame);

};

#endif // QTCAMERACAPTURE_H
