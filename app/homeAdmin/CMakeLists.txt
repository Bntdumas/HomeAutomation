project(homeAdmin)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)  
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Sql REQUIRED)

set(homeAdmin_HDRS
    adminToolItem.h
    adminWindow.h
    databaseAdmin.h
    databaseTools.h
    Hardware/moduleConfigurator.h
    Hardware/moduleWizard.h
    Hardware/moduleWizardPages.h
)

set(homeAdmin_SRCS
    adminToolItem.cpp
    adminWindow.cpp
    databaseAdmin.cpp
    databaseTools.cpp
    main.cpp
    Hardware/moduleConfigurator.cpp
    Hardware/moduleWizard.cpp
    Hardware/moduleWizardPages.cpp

)


include_directories(Hardware)
add_executable(homeAdmin ${homeAdmin_SRCS})
target_link_libraries(homeAdmin Qt5::Widgets Qt5::Sql)

