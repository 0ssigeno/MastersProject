cmake_minimum_required(VERSION 3.13)
project(Grafica)
#set(CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
#find_package(SDL2)
find_package(GLEW REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIR} )
include_directories(glm/include)

include_directories(${SDL2_INCLUDE_DIRS})



add_executable(game main.cpp HUD_Logger.h HUD_Logger.cpp)


target_link_libraries(game ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARY} ${SDL2_LIBRARIES})


#add_executable(OpenGLWorkbench main.cpp glcode.cpp)
#target_link_libraries(OpenGLWorkbench ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES} )