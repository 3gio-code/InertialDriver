cmake_minimum_required(VERSION 3.10)

project(InertialDriver)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Include globale (opzionale se usi target_include_directories correttamente, ma va bene qui)
include_directories(include)


# 1. Definiamo MyVector come libreria INTERFACE (Header-Only)
#    Significa: "Non compilare nulla, ma passa le proprietà a chi mi usa".
add_library(MyVector INTERFACE)


target_include_directories(MyVector INTERFACE include)


# InertialDriver ha un .cpp, quindi è una libreria normale (STATIC per default)
add_library(InertialDriver
    include/InertialDriver.h
    src/InertialDriver.cpp
)

add_executable(Test 
    src/main.cpp
)

# Linkiamo le librerie
target_link_libraries(Test
    MyVector
    InertialDriver
)