cmake_minimum_required(VERSION 3.10)

# set the project name
project(node)

add_compile_definitions(BOOST_LOG_DYN_LINK)

set(CMAKE_CXX_STANDARD 17)

set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)
find_package(Boost COMPONENTS program_options log log_setup REQUIRED )
include_directories( ${Boost_INCLUDE_DIR} )

set(SOURCE_FILES main.cpp loader.cpp block.cpp transaction.cpp chain.cpp blockchain.cpp peer.cpp peer_session.cpp message.cpp)

# add the executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} LINK_PUBLIC OpenSSL::Crypto)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${Boost_LIBRARIES} )


# add the executable
add_executable(example example.cpp)

target_link_libraries(example OpenSSL::Crypto)

