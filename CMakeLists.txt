cmake_minimum_required(VERSION 4.3)
project(lab2_task17)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        sfml
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG        2.6.1
)
FetchContent_MakeAvailable(sfml)

add_executable(lab2_task17 main.cpp)

target_link_libraries(lab2_task17 PRIVATE sfml-graphics sfml-window sfml-system)