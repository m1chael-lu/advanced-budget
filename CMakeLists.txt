cmake_minimum_required(VERSION 3.10)
project(MyBoostBeastProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Specify the root directory for OpenSSL
set(OPENSSL_ROOT_DIR /opt/homebrew)

find_package(Boost 1.83.0 REQUIRED COMPONENTS system)
find_package(OpenSSL REQUIRED)

include_directories(${Boost_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})
include_directories(/usr/local/mysql-connector-c++-8.2.0/include)

link_directories(/usr/local/mysql-connector-c++-8.2.0/lib64)

# Add executable
add_executable(MyServer src/main.cpp src/server/server.cpp src/server/session.cpp src/server/router.cpp src/server/routes.cpp)
add_executable(DatabaseSetup src/database/setup.cpp)

# Link libraries
target_link_libraries(MyServer ${Boost_LIBRARIES} mysqlcppconn OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(DatabaseSetup mysqlcppconn ${Boost_LIBRARIES} OpenSSL::SSL OpenSSL::Crypto)
