include_directories(../sharedElements ../houseData)

set(homeServer_HDRS
    homeServer_global.h
    moduleServer.h
    tcpLineParser.h
    tcpServer.h
    userServer.h
)

set(homeServer_SRCS
    moduleServer.cpp
    tcpLineParser.cpp
    tcpServer.cpp
    userServer.cpp
)

add_library(homeServer SHARED ${homeServer_SRCS} ${homeServer_HDRS})
target_link_libraries(homeServer Qt5::Network Qt5::Xml sharedElements houseData)
