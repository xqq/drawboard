cmake_minimum_required(VERSION 3.10)
project(drawboard)

set(CMAKE_CXX_STANDARD 14)

find_package(SDL2 REQUIRED)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_executable(drawboard
    main.cpp
    client/client.cpp
    server/server.cpp
    socket/buffer.cpp
)

if(WIN32)
    target_sources(drawboard
        PRIVATE
            socket/tcp_listener_winsock.cpp
            socket/tcp_socket_winsock.cpp
    )
else()
    target_sources(drawboard
        PRIVATE
            socket/tcp_listener_posix.cpp
            socket/tcp_socket_posix.cpp
    )
endif()

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
