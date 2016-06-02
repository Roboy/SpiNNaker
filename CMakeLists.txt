cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

find_package(gazebo REQUIRED)
include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})
list(APPEND CMAKE_CXX_FLAGS "${GAZEBO_CXX_FLAGS}")


add_library(spinnaker_plugin SHARED SpinnakerPlugin.cc spike_io_sender_receiver.cpp ConnectionListener.cpp DatabaseReader.cpp EIEIOMessage.cpp receiver_interface.cpp sender_interface_backward.cpp sender_interface_forward.cpp SpynnakerDatabaseConnection.cpp SpynnakerLiveSpikesConnection.cpp UDPConnection.cpp)
target_link_libraries(spinnaker_plugin ${GAZEBO_LIBRARIES} ${Boost_LIBRARIES})
