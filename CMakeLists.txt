cmake_minimum_required(VERSION 3.28.3)
project(wave)

find_package(SDL2 REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_executable(wave src/main.cpp
                    src/controller/Controller.cpp
                    src/model/Model.cpp
                    src/view/View.cpp
)

target_include_directories(wave PRIVATE
    ${SDL2_INCLUDE_DIRS}
    src/controller/
    src/model/
    src/view/
    src/utils
)

target_link_libraries(wave
    ${SDL2_LIBRARIES}
)