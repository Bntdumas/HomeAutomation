
set(clientSimulator_HDRS
    clientSimulator.h
    clientSimulator_global.h
)

set(clientSimulator_SRCS
    clientSimulator.cpp
)

add_library(clientSimulator SHARED ${clientSimulator_SRCS} ${clientSimulator_HDRS})
target_link_libraries(clientSimulator Qt5::Network)
