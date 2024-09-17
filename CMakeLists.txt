cmake_minimum_required(VERSION 3.5.0)
project(Rotom VERSION 0.0.1 LANGUAGES C CXX)

add_executable(Rotom src/main.cpp)
set_property (TARGET ${PROJECT_NAME}
  PROPERTY
    # Enable C++17 standard compliance
    CXX_STANDARD 17
)