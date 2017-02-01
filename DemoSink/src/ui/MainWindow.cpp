
#include <QDebug>
#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent, const Qt::WindowFlags &flags)
    : QMainWindow(parent, flags) {
}

void MainWindow::NewSsrc(const uint32_t unknownSsrc) {

}

void MainWindow::showVp8Frame(const std::unique_ptr<rtp::packetization::EncodedVideoFrame> &&frame) {
  qDebug() << "Got a video frame";
}
