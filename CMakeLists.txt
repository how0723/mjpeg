# set minimum cmake version
cmake_minimum_required(VERSION 3.0)
project(streamcapture-test)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

set (CHTX_STREAMCAPTURE_VERSION_MAJOR 1)
set (CHTX_STREAMCAPTURE_VERSION_MINOR 0)

# c++11 required
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_HOST_UNIX)
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -march=native -O3 -pthread" )
endif(CMAKE_HOST_UNIX)

option (USE_OPENCV "USE OPENCV LIB" ON) 

if (USE_OPENCV)
	if(CMAKE_HOST_WIN32)
		set(OPENCV_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/opencv/include)
		set(OPENCV_LIBS ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/opencv/lib/opencv_world341.lib )
		include_directories(${OPENCV_INCLUDE_DIRS})
		link_libraries(${OPENCV_LIBS})
	else()
		find_package(OpenCV REQUIRED)
		include_directories(${OpenCV_INCLUDE_DIRS})
		link_libraries(${OpenCV_LIBS})
	endif()	
endif (USE_OPENCV)

if(CMAKE_HOST_WIN32)
	ADD_DEFINITIONS(-DWIN32)
	ADD_DEFINITIONS(-D_WINDOWS)
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
endif(CMAKE_HOST_WIN32)	

include_directories(include)
include_directories(src)
include_directories(src/streamcapture)

set(STREAMCAPTURE_SOURCE_FILES 
	src/streamcapture/avilib.cpp 
	src/streamcapture/main.cpp 
	src/streamcapture/v4l2uvc.cpp
)
set(STREAMCAPTURE_HEADER_FILES
	src/streamcapture/avilib.h 
	src/streamcapture/uvcvideo.h 
	src/streamcapture/v4l2uvc.h 
)

if(CMAKE_HOST_WIN32)
	link_libraries(ws2_32)
endif(CMAKE_HOST_WIN32)	

add_executable("streamcapture-test" ${STREAMCAPTURE_SOURCE_FILES} ${STREAMCAPTURE_HEADER_FILES} )
target_link_libraries("streamcapture-test"   ${OpenCV_LIBS})
