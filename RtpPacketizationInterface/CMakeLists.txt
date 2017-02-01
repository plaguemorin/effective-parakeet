cmake_minimum_required(VERSION 3.6)
project(RtpPacketizationInterface)


add_library(RtpPacketizationInterface STATIC
        include/RtpPacketization/EncodedFrame.h
        include/RtpPacketization/PacketizedData.h
        include/RtpPacketization/PayloadRegistry.h
        src/PayloadRegistry.cpp
        include/RtpPacketization/PayloadTypeFactory.h include/RtpPacketization/RtpDataReceiver.h src/RtpDataReceiver.cpp include/RtpPacketization/EncodedVideoFrame.h)

target_include_directories(RtpPacketizationInterface PUBLIC include)
target_include_directories(RtpPacketizationInterface PRIVATE src)
set_property(TARGET RtpPacketizationInterface PROPERTY CXX_STANDARD 14)
