

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

};


#endif //EFFECTIVE_PARAKEET_MAINWINDOW_H
