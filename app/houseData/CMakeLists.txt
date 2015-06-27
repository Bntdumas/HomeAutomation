include_directories(../sharedElements)

set(houseData_HDRS
    devices.h
    houseData_global.h
    houseDataHandler.h
    houseDataStructure.h
)

set(houseData_SRCS
    houseDataHandler.cpp
    houseDataStructure.cpp
)

add_library(houseData SHARED ${houseData_SRCS} ${houseData_HDRS})
target_link_libraries(houseData Qt5::Core sharedElements)
