#set CMAKE_PREFIX_PATH env to opt/qt

project(homeAutomation)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_AUTOMOC true)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)  
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_definitions( -DQT_STRICT_ITERATORS )
add_definitions( -DQT_NO_CAST_FROM_ASCII )
add_definitions( -DQT_NO_CAST_TO_ASCII )
add_definitions( -DQT_NO_CAST_FROM_BYTEARRAY )
add_definitions( -DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)

find_package(Qt5 5.5.0 CONFIG REQUIRED Core Gui Widgets Network Test)

add_subdirectory(app)
add_subdirectory(tests)
