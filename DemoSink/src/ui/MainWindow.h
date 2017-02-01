

#ifndef EFFECTIVE_PARAKEET_MAINWINDOW_H
#define EFFECTIVE_PARAKEET_MAINWINDOW_H

#include <QMainWindow>
#include <QListView>
#include <QtCore/QStringListModel>

#include <memory>

#include <RtpPacketization/EncodedVideoFrame.h>

#define VPX_CODEC_DISABLE_COMPAT 1
#include <vpx/vpx_decoder.h>
#include <vpx/vp8dx.h>

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, const Qt::WindowFlags &flags = Qt::WindowFlags());

    virtual ~MainWindow();

    void NewSsrc(const uint32_t unknownSsrc);

    bool showVp8Frame(const std::unique_ptr<rtp::packetization::EncodedVideoFrame> &&frame);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    vpx_codec_ctx_t ctx;
    QImage image;
    QTimer *refresh_timer;

    void DisplayYUV(int size_y, unsigned char *buffer_y, int size_u, unsigned char *buffer_u, int size_v, unsigned char *buffer_v, unsigned int width, unsigned int height, int stride_y, int stride_u, int stride_v, vpx_img_fmt_t fmt);
};


#endif //EFFECTIVE_PARAKEET_MAINWINDOW_H
