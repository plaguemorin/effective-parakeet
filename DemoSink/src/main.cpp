#include <QtWidgets/QApplication>

#include <RtpRtcp/Session.h>

#include "ui/MainWindow.h"
#include "SimpleUdpTransport.h"
#include "vp8/VP8PacketCodecFactory.h"

int main(int argc, char *argv[]) {
  QApplication qApplication(argc, argv);

  // Build our session
  rtp::Session theSession;

  // Build our RTP Payload -> Codecs registry
  theSession.RegisterRtpPayload(96, std::make_shared<VP8PacketCodecFactory>());

  // Construct our transport and connect everything up
  SimpleUdpTransport transport;
  transport.RtcpReceiveCallback([&](const std::vector<uint8_t> &&data) { theSession.ParseRtcpPacket(std::move(data)); });
  transport.RtpReceiveCallback([&](const std::vector<uint8_t> &&data) { theSession.ParseRtpPacket(std::move(data)); });
  transport.StateChangedCallback([&](rtp::transport::State newState) { theSession.TransportStateChanged(newState); });
  theSession.SendRtcpDataCallback([&](const std::vector<uint8_t> &&data) { return transport.SendRtcp(std::move(data)); });
  theSession.SendRtpDataCallback([&](const std::vector<uint8_t> &&data) { return transport.SendRtp(std::move(data)); });

  // Listen on our UDP socket
  transport.ListenOn(5004);

  // Start the GUI
  MainWindow mainWindow;

  // When we get an unknown ssrc, we assume it's our stream since we don't have any signaling
  theSession.UnknownSsrcCallback([&](const uint32_t ssrc) {
      rtp::Stream::Config config;
      config.rtp.ssrc = ssrc;
      config.type = rtp::Stream::VIDEO;
      auto ptr = theSession.CreateSinkStream(config);
      if (auto stream = ptr.lock()) {
        stream->CompleteEncodedFrameCallback([&] (std::unique_ptr<rtp::packetization::EncodedFrame>&& frame) {
            mainWindow.showVp8Frame(
                std::unique_ptr<rtp::packetization::EncodedVideoFrame>(reinterpret_cast<rtp::packetization::EncodedVideoFrame*>(frame.release() )
            ));
        });
      }

      mainWindow.NewSsrc(ssrc);
  });

  // Show and wait until the user closes the main window
  mainWindow.show();

  return qApplication.exec();
}

