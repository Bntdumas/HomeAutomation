include_directories(../sharedElements ../homeServer ../houseData)

set(homeAdmin_HDRS
    adminWindow.h
    adminToolItem.h
)

set(homeAdmin_SRCS
    adminWindow.cpp
    main.cpp
)

add_executable(homeAdmin ${homeAdmin_SRCS} ${homeAdmin_HDRS})
target_link_libraries(homeAdmin Qt5::Widgets homeServer sharedElements houseData)

