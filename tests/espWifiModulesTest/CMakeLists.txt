include_directories(../sharedElements ../homeServer ../houseData)

set(espWifiModulesTest_HDRS
    espWifiModulesTest.h
)

set(espWifiModulesTest_UIS
    espWifiModulesTest.ui
)

set(espWifiModulesTest_SRCS
    espWifiModulesTest.cpp
    main.cpp
)

add_executable(espWifiModulesTest ${espWifiModulesTest_SRCS} ${espWifiModulesTest_HDRS} ${espWifiModulesTest_UIS})
target_link_libraries(espWifiModulesTest Qt5::Widgets homeServer sharedElements houseData)

