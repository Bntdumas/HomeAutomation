include_directories(../app/commonFiles ../app/clientSimulator ../app/homeServer ../app/houseDataStructure)

macro(add_unittest _name _source)
  set(_test ${_source})
  add_executable(${_name} ${_source})
  add_test(NAME ${_name} COMMAND ${_name})
  target_link_libraries(${_name} Qt5::Core Qt5::Test homeServer clientSimulator houseDataStructure)
endmacro()

add_unittest(networkModuleSimulation tst_networkModuleSimulation.cpp)
add_unittest(networkModuleWithHardware tst_networkModuleWithHardware.cpp)
add_unittest(dataStructure tst_dataStructure.cpp)

