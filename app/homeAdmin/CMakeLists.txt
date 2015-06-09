project(homeAdmin)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)  
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(homeAdmin_HDRS
    adminToolItem.h
    adminWindow.h
)

set(homeAdmin_SRCS
    adminToolItem.cpp
    adminWindow.cpp
    main.cpp
)


include_directories(Hardware)
add_executable(homeAdmin ${homeAdmin_SRCS})
target_link_libraries(homeAdmin Qt5::Widgets)

