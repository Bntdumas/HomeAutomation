include_directories(../commonFiles)

set(houseData_HDRS
    houseData_global.h
    houseDataHandler.h
    houseDataStructure.h
)

set(houseData_SRCS
    houseDataHandler.cpp
    houseDataStructure.cpp
)

add_library(houseData SHARED ${houseData_SRCS})
target_link_libraries(houseData Qt5::Core)
