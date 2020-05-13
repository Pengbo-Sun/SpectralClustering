cmake_minimum_required(VERSION 3.2)
project(pcl_with_geolib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)



find_package(PCL 1.7 REQUIRED)
find_package(Threads REQUIRED)

find_package(OpenCV REQUIRED)
find_package(Matlab REQUIRED)

add_executable(${PROJECT_NAME} main.cpp src/setlabels.cpp src/matengine.cpp)
add_definitions(${PCL_DEFINITIONS})

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Threads::Threads
        ${PCL_LIBRARIES}
        ${OpenCV_LIBS}
        ${Matlab_ROOT_DIR}/extern/bin/glnxa64/libMatlabEngine.so
        ${Matlab_ROOT_DIR}/extern/bin/glnxa64/libMatlabDataArray.so

)
target_compile_features(${PROJECT_NAME}
    PRIVATE
        cxx_range_for
        cxx_nullptr
        cxx_auto_type
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        include
        third_party
        ${PCL_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS}
        ${Matlab_ROOT_DIR}/extern/include 
)