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
        src/RtpVideoSink.cpp src/RtpVideoSink.h
        include/RtpRtcp/SinkStream.h src/SinkStream.cpp
        include/RtpRtcp/rtcp/Payload.h src/rtcp/Payload.cpp include/RtpRtcp/rtcp/SenderReport.h src/rtcp/SenderReport.cpp include/RtpRtcp/rtcp/SourceDescription.h src/rtcp/SourceDescription.cpp)

target_include_directories(RtpRtcp PUBLIC include)
target_include_directories(RtpRtcp PRIVATE src)
target_link_libraries(RtpRtcp PUBLIC RtpTransportInterface RtpPacketizationInterface ${CMAKE_THREAD_LIBS_INIT})
set_property(TARGET RtpRtcp PROPERTY CXX_STANDARD 14)
