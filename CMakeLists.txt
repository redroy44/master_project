#Specify the version being used as well as the language
cmake_minimum_required(VERSION 2.8)
#Name your project here
project(master_project)

MESSAGE( STATUS "CMAKE_SOURCE_DIR:         " ${CMAKE_SOURCE_DIR} )

# export compiler commands for autocompletion
set( CMAKE_EXPORT_COMPILE_COMMANDS 1 )

#Sends the -std=c++11 flag to the gcc compiler
#add_definitions(-g -Wall -std=c++11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11 -fopenmp")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native")

set(SOURCES
    Wave.cpp
    WaveProcessor.cpp
    NoiseEstimator.cpp
    LsaEstimator.cpp
)

set(HEADERS
    Wave.h
    WaveProcessor.h
    NoiseEstimator.h
    LsaEstimator.h

)
MESSAGE( SOURCES ": " ${SOURCES} )
MESSAGE( HEADERS ": " ${HEADERS} )

include_directories(${CMAKE_SOURCE_DIR})

add_executable(main main.cpp ${SOURCES} ${HEADERS})

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost COMPONENTS program_options filesystem system)
find_package(Armadillo)

target_link_libraries( main armadillo sndfile boost_program_options boost_filesystem boost_system)


