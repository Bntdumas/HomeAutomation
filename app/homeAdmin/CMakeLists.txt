include_directories(../commonFiles ../homeServer)

set(homeAdmin_HDRS
    adminWindow.h
    adminToolItem.h
)

set(homeAdmin_SRCS
    adminWindow.cpp
    main.cpp
)

add_executable(homeAdmin ${homeAdmin_SRCS})
target_link_libraries(homeAdmin Qt5::Widgets homeServer)

