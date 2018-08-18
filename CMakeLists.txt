cmake_minimum_required(VERSION 3.6)

project(FreezingCore)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(SHADERS_DIR ${PROJECT_SOURCE_DIR}/shaders)
set(THIRD_PARTY_SRC_DIR ${PROJECT_SOURCE_DIR}/third_party)
set(THIRD_PARTY_BIN_DIR ${PROJECT_BINARY_DIR}/third_party)

add_subdirectory(${THIRD_PARTY_SRC_DIR}/glfw)
add_subdirectory(${THIRD_PARTY_SRC_DIR}/assimp)

set(INCLUDES 
	${INCLUDE_DIR}/Camera.h
	${INCLUDE_DIR}/enums.h
	${INCLUDE_DIR}/HID.h
	${INCLUDE_DIR}/Keyboard.h
	${INCLUDE_DIR}/Mesh.h
	${INCLUDE_DIR}/Shader.h
	${INCLUDE_DIR}/SceneGraph.h
	${PROJECT_SOURCE_DIR}/config.h.in
	)

set(SOURCES 
	${SRC_DIR}/Camera.cpp
	${SRC_DIR}/HID.cpp
	${SRC_DIR}/Keyboard.cpp
	${SRC_DIR}/main.cpp
	${SRC_DIR}/Mesh.cpp
	${SRC_DIR}/Shader.cpp
	${SRC_DIR}/SceneGraph.cpp
	)
set(SHADERS
	${SHADERS_DIR}/light.frag
	${SHADERS_DIR}/basic.vert
	${SHADERS_DIR}/phong.frag
	)
	
add_executable(${PROJECT_NAME} ${INCLUDES} ${SOURCES} ${SHADERS})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_11)
#target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
target_include_directories(${PROJECT_NAME}
	PUBLIC
		${INCLUDE_DIR}
	PRIVATE
		${PROJECT_BINARY_DIR}/third_party/assimp/include #generated config for assimp
		${THIRD_PARTY_SRC_DIR}/glm
		${THIRD_PARTY_SRC_DIR}/stb
		${THIRD_PARTY_SRC_DIR}/glfw/include
		${THIRD_PARTY_SRC_DIR}/glad/include
		${THIRD_PARTY_SRC_DIR}/glad/src
		${THIRD_PARTY_SRC_DIR}/assimp/include
		${CMAKE_BINARY_DIR}/generated
	)

find_package(OpenGL REQUIRED)

target_link_libraries(${PROJECT_NAME}
	PRIVATE
		glfw
		${GLFW_LIBRARIES}
		${OPENGL_LIBRARIES} 
		${OPENGL_gl_LIBRARY}						
		assimp
	)
	
configure_file(${PROJECT_SOURCE_DIR}/config.h.in ${CMAKE_BINARY_DIR}/generated/config.h)
if(MSVC)
	SET(CMAKE_DEFAULT_STARTUP_PROJECT ${PROJECT_NAME})
endif()