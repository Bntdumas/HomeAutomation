#set CMAKE_PREFIX_PATH env to opt/qt

project(homeAutomation)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_AUTOMOC true)

find_package(Qt5 5.3.1 CONFIG REQUIRED Core Gui Widgets Network Sql)

add_subdirectory(app)
