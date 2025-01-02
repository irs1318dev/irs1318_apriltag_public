cmake_minimum_required(VERSION 3.1)
project(
    irs1318_apriltag_native
    VERSION 1.0.0
    LANGUAGES C CXX)

if(POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif()
option(BUILD_SHARED_LIBS "Build shared library" ON)

# Set a default build type if none was specified
set(default_build_type "Release")

# Set the C++ version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(DEBUG_POSTFIX "d")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE  STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS  "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

file(GLOB CPP_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB HEADER_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)
add_library(${PROJECT_NAME} ${CPP_SRCS} ${HEADER_SRCS})

if (MSVC)
    add_compile_definitions("_CRT_SECURE_NO_WARNINGS")
else()
    find_package(Threads REQUIRED)
    target_link_libraries(${PROJECT_NAME} PUBLIC Threads::Threads m)
endif()

set(_OpenCV_REQUIRED_COMPONENTS core imgproc videoio highgui)
find_package(OpenCV 4.10.0 EXACT COMPONENTS ${_OpenCV_REQUIRED_COMPONENTS} CONFIG REQUIRED)
find_package(apriltag CONFIG REQUIRED)
find_package(JNI REQUIRED)

find_package(PkgConfig)
pkg_check_modules(LIBCAMERA IMPORTED_TARGET libcamera)
pkg_check_modules(LIBEVENT IMPORTED_TARGET libevent_pthreads)

include_directories(${JNI_INCLUDE_DIRS})

if (LIBCAMERA_FOUND AND LIBEVENT_FOUND)
    include_directories(${LIBCAMERA_INCLUDE_DIRS} ${LIBEVENT_INCLUDE_DIRS})
    target_compile_definitions(${PROJECT_NAME} PUBLIC UseLibCamera)
    target_link_libraries(${PROJECT_NAME} PUBLIC apriltag PUBLIC ${OpenCV_LIBRARIES} PUBLIC PkgConfig::LIBCAMERA PUBLIC PkgConfig::LIBEVENT)
else()
    target_link_libraries(${PROJECT_NAME} PUBLIC apriltag PUBLIC ${OpenCV_LIBRARIES})
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 1 VERSION ${PROJECT_VERSION})

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
