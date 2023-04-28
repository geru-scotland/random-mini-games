cmake_minimum_required(VERSION 3.21)
project(pong_game)
set(CMAKE_CXX_STANDARD 23)

# ----- SETS ----- #

set(SDL2_INCLUDE_DIR /usr/local/include/)
#set(SDL2_LIBRARY /usr/local/lib/libSDL2main.a;/usr/local/lib/libSDL2.dylib;)
set(SDL2_LIBRARY /usr/local/lib/libSDL2.dylib;)

set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES
        Main.cpp
        src/GameSystem.cpp
        src/GameSystem.h)

# ----- END SETS ----- #

# ----- EXECUTABLES ----- #

add_executable(${PROJECT_NAME}
        ${SOURCE_FILES})

# ----- END EXECUTABLES ---- #

# ---- INCLUDES AND LIB LINK ---- #

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARY})

# ---- END INCLUDES AND LIB LINK ---- #