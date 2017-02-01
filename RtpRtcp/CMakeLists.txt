cmake_minimum_required(VERSION 3.6)
project(RtpRtcp)

find_package (Threads)

add_library(RtpRtcp STATIC
        include/RtpRtcp/Session.h src/Session.cpp
        include/RtpRtcp/Stream.h src/Stream.cpp
        include/RtpRtcp/Extension.h src/Extension.cpp
        include/RtpRtcp/RtpPacket.h src/RtpPacket.cpp
        include/RtpRtcp/RtcpPacket.h src/RtcpPacket.cpp
        src/BufferNetworkHelper.h
        include/RtpRtcp/PayloadRegistry.h src/PayloadRegistry.cpp
        src/RtpVideoSink.cpp src/RtpVideoSink.h
        include/RtpRtcp/Frame.h
        include/RtpRtcp/SinkStream.h src/SinkStream.cpp
        include/RtpRtcp/PacketizedData.h)

target_include_directories(RtpRtcp PUBLIC include)
target_include_directories(RtpRtcp PRIVATE src)
target_link_libraries(RtpRtcp PUBLIC  RtpTransportInterface ${CMAKE_THREAD_LIBS_INIT})
set_property(TARGET RtpRtcp PROPERTY CXX_STANDARD 14)
