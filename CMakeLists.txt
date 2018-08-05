cmake_minimum_required(VERSION 3.10)

project(FrozenCore)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

list(APPEND CMAKE_CXX_FLAGS "-std=c++11")

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(SHADERS_DIR ${PROJECT_SOURCE_DIR}/shaders)
set(THIRD_PARTY_SRC_DIR ${PROJECT_SOURCE_DIR}/third_party)
set(THIRD_PARTY_BIN_DIR ${PROJECT_BINARY_DIR}/third_party)

add_subdirectory(${THIRD_PARTY_SRC_DIR}/glfw)
add_subdirectory(${THIRD_PARTY_SRC_DIR}/assimp)

set(THIRD_PARTY_HEADERS
	${THIRD_PARTY_SRC_DIR}/glm
	${THIRD_PARTY_SRC_DIR}/stb
	${THIRD_PARTY_SRC_DIR}/glfw/include
	${THIRD_PARTY_SRC_DIR}/glad/include
	${THIRD_PARTY_SRC_DIR}/glad/src
	${THIRD_PARTY_SRC_DIR}/assimp/include
	${PROJECT_BINARY_DIR}/third_party/assimp/include #generated config for assimp
	)
include_directories(${THIRD_PARTY_HEADERS})

find_package(OpenGL REQUIRED)

set(INCLUDES 
	${INCLUDE_DIR}/Camera.h
	${INCLUDE_DIR}/enums.h
	${INCLUDE_DIR}/HID.h
	${INCLUDE_DIR}/Keyboard.h
	${INCLUDE_DIR}/Mesh.h
	${INCLUDE_DIR}/Model.h
	${INCLUDE_DIR}/Shader.h
	)

set(SOURCES 
	${SRC_DIR}/Camera.cpp
	${SRC_DIR}/HID.cpp
	${SRC_DIR}/Keyboard.cpp
	${SRC_DIR}/main.cpp
	${SRC_DIR}/Mesh.cpp
	${SRC_DIR}/Model.cpp
	${SRC_DIR}/Shader.cpp
	)
include_directories(${INCLUDE_DIR})
add_executable(${PROJECT_NAME} ${INCLUDES} ${SOURCES})
target_link_libraries(${PROJECT_NAME}
				glfw ${GLFW_LIBRARIES}
				${OPENGL_LIBRARIES} 
				${OPENGL_gl_LIBRARY}						
				assimp
				)