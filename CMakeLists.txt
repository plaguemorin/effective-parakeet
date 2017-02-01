cmake_minimum_required(VERSION 3.6)
project(effective-parakeet)

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
        "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(warnings "-Wall -Wextra -Werror")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(warnings "/W4 /wd4512 /WX /EHsc")
    # Disabled Warnings:
    #   4512 "assignment operator could not be generated"
    #        This warning provides no useful information and will occur in
    #        well formed programs.
    #        <http://msdn.microsoft.com/en-us/library/hsyx7kbz.aspx>
endif ()

add_subdirectory(Vendor/libyuv)

add_subdirectory(RtpTransportInterface)
add_subdirectory(RtpPacketizationInterface)
add_subdirectory(RtpRtcp)
add_subdirectory(DemoSink)

