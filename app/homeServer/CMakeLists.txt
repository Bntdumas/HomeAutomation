include_directories(../commonFiles)

set(homeServer_HDRS
    homeServer.h
    homeServer_global.h
    TCPLineParser.h
)

set(homeServer_SRCS
    homeServer.cpp
    TCPLineParser.cpp
)

add_library(homeServer SHARED ${homeServer_SRCS})
target_link_libraries(homeServer Qt5::Network)
