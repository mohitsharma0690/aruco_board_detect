cmake_minimum_required(VERSION 2.8.3)
project(aruco_board)

#Enable C++ 11
add_compile_options(-std=c++11)

#Packages
find_package(catkin REQUIRED COMPONENTS	cv_bridge roscpp std_msgs message_generation image_transport)
find_package(OpenCV REQUIRED)

#Messages
add_message_files(FILES Marker.msg MarkerImageInfo.msg)
generate_messages(DEPENDENCIES std_msgs)

#Catkin dependencies
catkin_package(CATKIN_DEPENDS message_runtime roscpp std_msgs)

#Aruco ROS node
add_executable(aruco_board src/ros/ArucoNode.cpp)
add_dependencies(aruco_board aruco_generate_messages_cpp ${catkin_EXPORTED_TARGETS})
target_link_libraries(aruco_board ${catkin_LIBRARIES} ${OpenCV_LIBS})

#Include directories
include_directories(include ${catkin_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})

#Install
install(TARGETS aruco_board
	ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
	LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
	RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
