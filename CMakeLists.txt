cmake_minimum_required(VERSION 3.10)
project(drawboard)

set(CMAKE_CXX_STANDARD 14)

find_package(SDL2 REQUIRED)

add_executable(drawboard
    main.cpp
    client/client.cpp
    server/server.cpp
    socket/tcp_listener_posix.cpp
    socket/tcp_socket_posix.cpp
)

target_include_directories(drawboard
    PRIVATE
        socket
        ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(drawboard
    PRIVATE
        pthread
        ${SDL2_LIBRARIES}
)