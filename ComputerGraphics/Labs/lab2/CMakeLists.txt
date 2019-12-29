cmake_minimum_required(VERSION 3.13)
project(Grafica)
#set(CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
#find_package(SDL2)
find_package(GLEW REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIR})
#include_directories(${SDL2_INCLUDE_DIRS})


add_executable(jump JumpingBall.cpp )
add_executable(particle  Particle.cpp Particle.h)
add_executable(gioco giocoVBO.cpp)

add_executable(bigGioco bigProj/Block.cpp bigProj/Game.cpp bigProj/GameObject.cpp bigProj/HUD_Logger.cpp bigProj/Input.cpp bigProj/main.cpp  bigProj/Block.h bigProj/Game.h bigProj/GameObject.h bigProj/HUD_Logger.h bigProj/Input.h )

add_executable(ark arkanoid.cpp)


target_link_libraries(jump ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARY})
target_link_libraries(particle ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARY})
target_link_libraries(gioco ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARY})

target_link_libraries(bigGioco ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARY})

target_link_libraries(ark ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARY})


#add_executable(OpenGLWorkbench main.cpp glcode.cpp)
#target_link_libraries(OpenGLWorkbench ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES} )