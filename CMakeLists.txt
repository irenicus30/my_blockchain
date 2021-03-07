cmake_minimum_required(VERSION 3.10)

# set the project name
project(blockchain)

set(CMAKE_CXX_STANDARD 17)

set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)

# add the executable
add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} OpenSSL::Crypto)


# add the executable
add_executable(example example.cpp)

target_link_libraries(example OpenSSL::Crypto)

