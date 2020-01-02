cmake_minimum_required(VERSION 2.8.3)
project(rosrobot_control)

find_package(catkin REQUIRED COMPONENTS roscpp std_msgs)

## Declare a catkin package
catkin_package()

include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(rosrobot_teleop_key src/rosrobot_teleop_key.cpp)
target_link_libraries(rosrobot_teleop_key ${catkin_LIBRARIES})
