#Specify the version being used aswell as the language
cmake_minimum_required(VERSION 2.6)
#Name your project here
project(master_project)

#Sends the -std=c++11 flag to the gcc compiler
add_definitions(-std=c++11)
#This tells CMake to fib.c and name it fibonacci
add_executable(main main.cpp)

set(Boost_INCLUDE_DIRS /usr/include/boost)
set(Boost_LIBRARY_DIR /usr/lib/x86_64-linux-gnu)
set(Armadillo_INCLUDE_DIRS /usr/include)
set(Armadillo_LIBRARY_DIRS "/usr/lib")

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


