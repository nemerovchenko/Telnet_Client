cmake_minimum_required(VERSION 3.12)
project(telnet_client)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
add_executable(telnet_client
  src/TelnetClient.cpp
  src/main.cpp
)

target_link_libraries(telnet_client pthread)