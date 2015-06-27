
set(sharedElements_HDRS
    utils.h
    sharedElements_global.h
    debugCenter.h
)

set(sharedElements_SRCS
    debugCenter.cpp
)

add_library(sharedElements SHARED ${sharedElements_SRCS} ${sharedElements_HDRS})
target_link_libraries(sharedElements Qt5::Core)
