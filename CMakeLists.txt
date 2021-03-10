cmake_minimum_required(VERSION 3.10)

# set the project name
project(blockchain)

set(CMAKE_CXX_STANDARD 17)

set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED )
include_directories( ${Boost_INCLUDE_DIR} )

# add the executable
add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} LINK_PUBLIC OpenSSL::Crypto)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${Boost_LIBRARIES} )


# add the executable
add_executable(example example.cpp)

target_link_libraries(example OpenSSL::Crypto)

