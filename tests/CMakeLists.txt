include_directories(../app/sharedElements ../app/clientSimulator ../app/homeServer ../app/houseData)

enable_testing()

macro(add_unittest _name _source)
  set(_test ${_source})
  add_executable(${_name} ${_source})
  add_test(NAME ${_name} COMMAND ${_name})
  target_link_libraries(${_name} Qt5::Core Qt5::Test homeServer clientSimulator houseData sharedElements)
endmacro()

add_unittest(networkModuleSimulation tst_networkModuleSimulation.cpp)
add_unittest(networkModuleWithHardware tst_networkModuleWithHardware.cpp)
add_unittest(dataStructure tst_dataStructure.cpp)
add_unittest(devices tst_devices.cpp)


