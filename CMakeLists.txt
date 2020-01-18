cmake_minimum_required(VERSION 3.10)
project(drawboard)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

if(MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
        -static-libgcc -static-libstdc++ -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive -Wl,-Bdynamic \
    ")
endif()

add_subdirectory(thirdparty/flatbuffers
    ${CMAKE_CURRENT_BINARY_DIR}/flatbuffers-build
    EXCLUDE_FROM_ALL
)

include_directories(
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(drawboard
    main.cpp
    client/client_app.cpp
    client/draw_client.cpp
    server/draw_server.cpp
    server/transmit_worker.cpp
    socket/read_write_buffer.cpp
    socket/tcp_listener.cpp
    socket/tcp_socket.cpp
    common/abstract_canvas.cpp
    common/log.cpp
    common/packet.cpp
    common/threaded_worker.cpp
)

if(WIN32)
    target_sources(drawboard
        PRIVATE
            DeclareDPIAware.manifest
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
        thirdparty/argh
        ${SDL2_INCLUDE_DIRS}
)

# The flatbuffers target carry header search path automatically if CMake > 2.8.11.
target_link_libraries(drawboard
    PRIVATE
        flatbuffers
)

if(MSVC)
    # SDL linkage for MSVC
    target_link_libraries(drawboard
        PRIVATE
            SDL2::SDL2
            SDL2::SDL2main
    )
elseif(MINGW)
    # SDL linkage for MinGW: Enable full static link
    target_link_libraries(drawboard
        PRIVATE
            -Wl,-Bstatic
            SDL2
            SDL2main
            -Wl,-Bdynamic
    )
else()
    # SDL linkage for other platforms: dynamic link by default
    target_link_libraries(drawboard
        PRIVATE
            ${SDL2_LIBRARIES}
    )
endif()


if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # pthread is needed for gcc
    target_link_libraries(drawboard
        PRIVATE
            pthread
    )
endif()


if(WIN32)
    target_link_libraries(drawboard
        PRIVATE
            Imm32
            Winmm
            Version
            SetupAPI
            Ws2_32
    )
endif()
