
#include <QDebug>
#include <QtGui/QPainter>
#include <QTimer>
#include <libyuv.h>
#include "MainWindow.h"

#define vpx_dx_interface (vpx_codec_vp8_dx())

namespace {
    void printFmt(vpx_img_fmt_t fmt) {
      switch (fmt) {
        case VPX_IMG_FMT_NONE: qDebug() << "Format: IMG_FMT_NONE"; break;
        case VPX_IMG_FMT_RGB24: qDebug() << "Format: IMG_FMT_RGB24"; break;
        case VPX_IMG_FMT_RGB32: qDebug() << "Format: IMG_FMT_RGB32"; break;
        case VPX_IMG_FMT_RGB565: qDebug() << "Format: IMG_FMT_RGB565"; break;
        case VPX_IMG_FMT_RGB555: qDebug() << "Format: IMG_FMT_RGB555"; break;
        case VPX_IMG_FMT_UYVY: qDebug() << "Format: IMG_FMT_UYVY"; break;
        case VPX_IMG_FMT_YUY2: qDebug() << "Format: IMG_FMT_YUY2"; break;
        case VPX_IMG_FMT_YVYU: qDebug() << "Format: IMG_FMT_YVYU"; break;
        case VPX_IMG_FMT_BGR24: qDebug() << "Format: IMG_FMT_BGR24"; break;
        case VPX_IMG_FMT_RGB32_LE: qDebug() << "Format: IMG_FMT_RGB32_LE"; break;
        case VPX_IMG_FMT_ARGB: qDebug() << "Format: IMG_FMT_ARGB"; break;
        case VPX_IMG_FMT_ARGB_LE: qDebug() << "Format: IMG_FMT_ARGB_LE"; break;
        case VPX_IMG_FMT_RGB565_LE: qDebug() << "Format: IMG_FMT_RGB565_LE"; break;
        case VPX_IMG_FMT_RGB555_LE: qDebug() << "Format: IMG_FMT_RGB555_LE"; break;
        case VPX_IMG_FMT_YV12: qDebug() << "Format: IMG_FMT_YV12"; break;
        case VPX_IMG_FMT_I420: qDebug() << "Format: IMG_FMT_I420"; break;
        case VPX_IMG_FMT_VPXYV12: qDebug() << "Format: IMG_FMT_VPXYV12"; break;
        case VPX_IMG_FMT_VPXI420: qDebug() << "Format: IMG_FMT_VPXI420"; break;
        case VPX_IMG_FMT_I422: qDebug() << "Format: IMG_FMT_I422"; break;
        case VPX_IMG_FMT_I444: qDebug() << "Format: IMG_FMT_I444"; break;
        case VPX_IMG_FMT_I440: qDebug() << "Format: IMG_FMT_I440"; break;
        case VPX_IMG_FMT_444A: qDebug() << "Format: IMG_FMT_444A"; break;
        case VPX_IMG_FMT_I42016: qDebug() << "Format: IMG_FMT_I42016"; break;
        case VPX_IMG_FMT_I42216: qDebug() << "Format: IMG_FMT_I42216"; break;
        case VPX_IMG_FMT_I44416: qDebug() << "Format: IMG_FMT_I44416"; break;
        case VPX_IMG_FMT_I44016: qDebug() << "Format: IMG_FMT_I44016"; break;
      }
    }
}

MainWindow::MainWindow(QWidget *parent, const Qt::WindowFlags &flags)
    : QMainWindow(parent, flags), refresh_timer(new QTimer(this)) {
  vpx_codec_dec_cfg_t cfg;

  // Setting number of threads to a constant value (1)
  cfg.threads = 8;
  cfg.h = cfg.w = 0;  // set after decode

  auto err = vpx_codec_dec_init(&ctx, vpx_dx_interface, &cfg, VPX_CODEC_USE_FRAME_THREADING);

  setWindowTitle(QStringLiteral("Demo RTP"));
  resize(480, 275);

  refresh_timer->connect(refresh_timer, SIGNAL(timeout()), this, SLOT(repaint()));
  refresh_timer->setTimerType(Qt::PreciseTimer);
  refresh_timer->start(33);
}

MainWindow::~MainWindow() {
  vpx_codec_destroy(&ctx);
}

void MainWindow::NewSsrc(const uint32_t unknownSsrc) {

}

bool MainWindow::showVp8Frame(const std::unique_ptr<rtp::packetization::EncodedVideoFrame> &&frame) {
  auto err_decode = vpx_codec_decode(&ctx, frame->payload.data(), frame->payload.size(), NULL, 0);
  if (err_decode) {
    printf("Unable to decode frame!\n%s\n", vpx_codec_err_to_string(err_decode));
    return false;
  }

  /* An encoded image might contain more than one frame */
  vpx_codec_iter_t iter = nullptr;
  while (auto img = vpx_codec_get_frame(&ctx, &iter)) {
    if (image.width() != img->d_w || image.height() != img->d_h || image.format() != QImage::Format_RGB16) {
      qDebug() << "Recreating frame with size " << img->d_w << "x" << img->d_h;
      printFmt(img->fmt);
      image = QImage(img->d_w, img->d_h, QImage::Format_RGB16);
    }

    ConvertFromI420(
        img->planes[VPX_PLANE_Y], img->stride[VPX_PLANE_Y],
        img->planes[VPX_PLANE_U], img->stride[VPX_PLANE_U],
        img->planes[VPX_PLANE_V], img->stride[VPX_PLANE_V],
        image.bits(), img->d_w * 2,
        img->d_w, img->d_h,
        libyuv::FOURCC_RGBP
    );
  }

  return true;
}

void MainWindow::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.drawImage(0, 0, image);
}
