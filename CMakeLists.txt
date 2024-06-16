cmake_minimum_required(VERSION 3.28)

project(PrecisionDocMatch)


set(CMAKE_CXX_STANDARD 17)
find_package(Qt6 REQUIRED COMPONENTS
        Core
        Gui
        Widgets
)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


add_executable(PrecisionDocMatch main.cpp
)
find_package(rapidfuzz CONFIG REQUIRED)

target_link_libraries(PrecisionDocMatch
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        rapidfuzz::rapidfuzz
)