cmake_minimum_required(VERSION 3.20)
project(minesweeper)

set(CMAKE_CXX_STANDARD 17)

set(SFML_DIR "SFML-2.5.1/lib/cmake/SFML")
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_executable(minesweeper src/main.cpp src/Toolbox.cpp src/Toolbox.h src/Button.cpp src/Button.h src/Tile.cpp src/Tile.h src/GameState.cpp src/GameState.h src/minesweeper.h src/minesweeper.cpp)

target_link_libraries(minesweeper sfml-graphics sfml-audio)

