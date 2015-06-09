project(homeAdmin)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)  
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(homeAdmin_HDRS
    adminWindow.h
    adminToolItem.h
)

set(homeAdmin_SRCS
    adminWindow.cpp
    main.cpp
)


include_directories(Hardware)
add_executable(homeAdmin ${homeAdmin_SRCS})
target_link_libraries(homeAdmin Qt5::Widgets)

