set(SFML_LIB sfml-graphics sfml-system sfml-window)

cmake_minimum_required(VERSION 3.0.0)
project(
  brick_game
  VERSION 0.1.0
  LANGUAGES C CXX)

include(CTest)
enable_testing()

link_libraries(${SFML_LIB})
add_executable(game src/main.cpp)

FILE(COPY fonts DESTINATION .)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
