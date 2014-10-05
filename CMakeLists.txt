#Specify the version being used aswell as the language
cmake_minimum_required(VERSION 2.8)
#Name your project here
project(master_project)

MESSAGE( STATUS "CMAKE_SOURCE_DIR:         " ${CMAKE_SOURCE_DIR} )

#Sends the -std=c++11 flag to the gcc compiler
add_definitions(-g -Wall -std=c++11)

set(SOURCES 
	Wave.cpp WaveProcessor.cpp NoiseEstimator.cpp
)

set(HEADERS
	Wave.h 
	WaveProcessor.h 
	NoiseEstimator.h
)
MESSAGE( SOURCES ": " ${SOURCES} )
MESSAGE( HEADERS ": " ${HEADERS} )

include_directories(${CMAKE_SOURCE_DIR})
link_directories(${CMAKE_SOURCE_DIR})

add_executable(main main.cpp ${SOURCES} ${HEADERS})

set(Boost_INCLUDE_DIRS /usr/include/boost)
set(Boost_LIBRARY_DIR /usr/lib/x86_64-linux-gnu)
set(Armadillo_INCLUDE_DIRS /usr/include)
set(Armadillo_LIBRARY_DIRS "/usr/lib/libarmadillo.so")
set(LibSndFile_LIBRARY_DIRS "/usr/local/lib/libsndfile.so")


set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.54.0 COMPONENTS program_options) 

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIR})
    target_link_libraries( main ${Boost_LIBRARIES} )                                                                                                                                                                                                                            
endif()

include_directories(${Armadillo_INCLUDE_DIRS})


target_link_libraries( main ${Armadillo_LIBRARY_DIRS} )
target_link_libraries( main ${LibSndFile_LIBRARY_DIRS} )

