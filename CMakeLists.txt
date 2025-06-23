cmake_minimum_required(VERSION 3.30)
project(Lab5)

set(CMAKE_CXX_STANDARD 20)

if(WIN32)
    set(OPENGL_LIBRARIES opengl32 glu32)
    set(GLUT_LIBRARIES freeglut)
endif()

add_executable(Lab5 main.cpp DynamicArray.h Linal.h MatPoint.h RigidBody.h Vector.h)

target_link_libraries(Lab5
        ${OPENGL_LIBRARIES}
        ${GLUT_LIBRARIES}
)
