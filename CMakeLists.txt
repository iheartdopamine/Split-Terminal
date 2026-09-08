cmake_minimum_required(VERSION 3.16)
project(SplitTerminal VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

include(FetchContent)
FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v5.0.0
)
FetchContent_MakeAvailable(ftxui)

include_directories(include)

add_executable(split_terminal
    src/main.cpp
    src/core/System.cpp
    src/core/BufferManager.cpp
)

target_link_libraries(split_terminal PRIVATE
    ftxui::screen
    ftxui::dom
    ftxui::component
)