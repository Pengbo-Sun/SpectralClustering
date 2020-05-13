cmake_minimum_required(VERSION 3.5)
project(SpectralClust)

set(CMAKE_CXX_STANDARD 11)

find_package(Threads REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(Armadillo REQUIRED)

add_executable(SpectralClust main.cpp
        src/scenegraph.cpp
)

target_link_libraries(SpectralClust
        PRIVATE
        Threads::Threads
	${ARMADILLO_LIBRARIES}
)

target_compile_features(SpectralClust
        PRIVATE
        cxx_range_for
        cxx_nullptr
        cxx_auto_type
        )

target_include_directories(SpectralClust
        PRIVATE
        include
        third_party/boost/libs/graph/include
	${EIGEN3_INCLUDE_DIR}
        ${ARMADILLO_INCLUDE_DIR}
        )
