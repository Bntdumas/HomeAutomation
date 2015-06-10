include_directories(../commonFiles)

set(houseDataStructure_HDRS
    houseDataHandler.h
    houseDataHandler_global.h
    houseDataStructure.h
)

set(houseDataStructure_SRCS
    houseDataHandler.cpp
    houseDataStructure.cpp
)

add_library(houseDataStructure SHARED ${houseDataStructure_SRCS})
target_link_libraries(houseDataStructure Qt5::Network)
