cmake_minimum_required(VERSION 3.10)
project(MyBoostBeastProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set this to your Boost installation path if needed
# set(BOOST_ROOT "path/to/your/boost_1_84_0")

find_package(Boost 1.83.0 REQUIRED COMPONENTS system)

include_directories(${Boost_INCLUDE_DIRS})

# Add executable
add_executable(MyServer src/main.cpp src/server/server.cpp src/server/session.cpp src/server/router.cpp src/server/routes.cpp)

# Link libraries
target_link_libraries(MyServer ${Boost_LIBRARIES})
