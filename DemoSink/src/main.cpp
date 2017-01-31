#include <RtpRtcp/Session.h>
#include <QtWidgets/QApplication>
#include "ui/MainWindow.h"

#include "SimpleUdpTransport.h"

int main(int argc, char *argv[]) {
  QApplication qApplication(argc, argv);

  // Build our RTP Payload -> Codecs registry

  // Build our session
  rtp::Session theSession;

  // Construct our transport and connect everything up
  SimpleUdpTransport transport;
  transport.RtcpReceiveCallback([&](const std::vector<uint8_t> &&data) { theSession.ParseRtcpPacket(std::move(data)); });
  transport.RtpReceiveCallback([&](const std::vector<uint8_t> &&data) { theSession.ParseRtpPacket(std::move(data)); });
  transport.StateChangedCallback([&](rtp::Transport::State newState) { theSession.TransportStateChanged(newState); });
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
      theSession.CreateSinkStream(config);

      mainWindow.NewSsrc(ssrc);
  });

  // Show and wait until the user closes the main window
  mainWindow.show();

  return qApplication.exec();
}

