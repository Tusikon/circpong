cmake_minimum_required(VERSION 3.5)
project(cirpong)

set(CMAKE_CXX_STANDARD 14)

include_directories(
        .
        C:/amsys32/mingw64/include/SDL
        /usr/local/include/SDL2
)

link_directories(
        .
        C:/msys32/mingw64/lib
)

IF(APPLE)
    FIND_LIBRARY(OPENGL OpenGL)
    MARK_AS_ADVANCED (OPENGL)
    SET(EXTRA_LIBS ${OPENGL})

    add_definitions(-DAPPLE)

ELSEIF(WIN32)
    SET(EXTRA_LIBS opengl32 glew32)
ELSE()
    SET(EXTRA_LIBS GL GLEW)
ENDIF ()

IF(UNIX AND NOT APPLE AND NOT WIN32) # Use powermates controls, not on MAC OR WINDOWS :/
    SET(SRC_CONTROL src/control.cpp)
ELSE()
    SET(SRC_CONTROL src/control_non.cpp)
ENDIF()

set(SOURCE_FILES
        src/main.cpp
        src/scenes/text_drawer.cpp
        src/scenes/utils.cpp
        src/scenes/arena.cpp
        src/scenes/game_start.cpp

        src/state.cpp
        src/environment.cpp
        src/player.cpp
        src/ball.cpp
        src/game.cpp
        ${SRC_CONTROL}
        src/input.h)

add_executable(cirpong ${SOURCE_FILES})
target_link_libraries(cirpong ${EXTRA_LIBS} SDL2 SDL2_ttf )
