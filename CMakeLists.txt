cmake_minimum_required(VERSION 3.15)
project(streamcapture)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
	MESSAGE(STATUS "current platform: Linux ")
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -march=native -O2 -pthread" )

    # include_directories(/home/matrix/workspace/opencv-4.2.0/my_install/include/opencv4)
    # link_directories(/home/matrix/workspace/opencv-4.2.0/my_install/lib)
    # link_directories(/home/matrix/workspace/opencv-4.2.0/my_install/lib/opencv4/3rdparty)
	# set(OpenCV_LIBS opencv_world ade IlmImf ittnotify libjasper libjpeg-turbo libpng libprotobuf libtiff libwebp quirc zlib)
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    MESSAGE(STATUS "current platform: Windows")
    set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} D:\\tools\\vcpkg\\installed\\x64-windows\\share\\opencv)
    FIND_PACKAGE(OpenCV CONFIG REQUIRED)
    include_directories(${OpenCV_INCLUDE_DIRS})
ELSE ()
    MESSAGE(STATUS "other platform: ${CMAKE_SYSTEM_NAME}")
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")

include_directories(src)
include_directories(src/streamcapture)

file(GLOB_RECURSE SRC_FILES src/*.cpp *.h)

add_executable("streamcapture" ${SRC_FILES} )
target_link_libraries("streamcapture"  
#  ${OpenCV_LIBS}
 )
