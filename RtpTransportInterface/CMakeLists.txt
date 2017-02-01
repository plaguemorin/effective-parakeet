cmake_minimum_required(VERSION 3.6)
project(RtpTransportInterface)


add_library(RtpTransportInterface STATIC
        src/Transport.cpp include/RtpTransport/Transport.h
        include/RtpTransport/State.h)

target_include_directories(RtpTransportInterface PUBLIC include)
target_include_directories(RtpTransportInterface PRIVATE src)
set_property(TARGET RtpTransportInterface PROPERTY CXX_STANDARD 14)
