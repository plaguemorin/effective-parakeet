cmake_minimum_required(VERSION 3.6)
project(DemoSink)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Widgets finds its own dependencies (QtGui and QtCore).
find_package(Qt5Widgets REQUIRED)

add_executable(DemoSink src/main.cpp src/SimpleUdpTransport.cpp src/SimpleUdpTransport.h src/ui/MainWindow.cpp src/ui/MainWindow.h src/vp8/VP8PacketCodecFactory.cpp src/vp8/VP8PacketCodecFactory.h src/vp8/VP8PacketReconstructor.cpp src/vp8/VP8PacketReconstructor.h)
target_link_libraries(DemoSink PUBLIC RtpRtcp Qt5::Widgets vpx yuv)
