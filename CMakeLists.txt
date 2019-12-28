cmake_minimum_required(VERSION 3.10)
project(drawboard)

set(CMAKE_CXX_STANDARD 14)

find_package(SDL2 REQUIRED)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

if(MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
        -static-libgcc -static-libstdc++ -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive -Wl,-Bdynamic \
    ")
endif()

add_executable(drawboard
    main.cpp
    client/client_app.cpp
    server/server.cpp
    socket/buffer.cpp
    socket/tcp_listener.cpp
    socket/tcp_socket.cpp
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
        ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(drawboard
    PRIVATE
        ${SDL2_LIBRARIES}
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries(drawboard
        PRIVATE
            pthread
    )
endif ()

if(WIN32)
    target_link_libraries(drawboard
        PRIVATE
            Ws2_32
    )
endif()
