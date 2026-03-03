cmake_minimum_required(VERSION 3.16)
project(simple_gl_demo LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Prefer out-of-source build
if("${CMAKE_BINARY_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
  message(FATAL_ERROR "Please build out-of-source (mkdir build && cd build && cmake ..)")
endif()

include(FetchContent)

# --- GLAD (robust handling) ---
FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG v0.1.36
)
FetchContent_MakeAvailable(glad)

# Try to detect an exported glad target; otherwise create a local one
set(GLAD_TARGET "")
if (TARGET glad::glad)
  set(GLAD_TARGET glad::glad)
elseif (TARGET glad)
  set(GLAD_TARGET glad)
else()
  if (DEFINED glad_SOURCE_DIR)
    add_library(glad STATIC ${glad_SOURCE_DIR}/src/glad.c)
    target_include_directories(glad PUBLIC ${glad_SOURCE_DIR}/include)
    set(GLAD_TARGET glad)
  else()
    message(FATAL_ERROR "glad source dir not available and no glad target exported")
  endif()
endif()

message(STATUS "Resolved GLAD target: ${GLAD_TARGET}")

# --- GLFW ---
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.3.8
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# --- glm (header-only) ---
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 0.9.9.8
)
FetchContent_MakeAvailable(glm)

# Executable
add_executable(simple_gl src/main.cpp)

# include glad headers if downloaded
if (DEFINED glad_SOURCE_DIR)
  target_include_directories(simple_gl PRIVATE ${glad_SOURCE_DIR}/include)
endif()

# Link libraries using resolved GLAD target
target_link_libraries(simple_gl PRIVATE glfw ${GLAD_TARGET} glm)

# Platform-specific OpenGL linking
if (WIN32)
  target_link_libraries(simple_gl PRIVATE opengl32)
elseif(APPLE)
  find_library(COCOA Cocoa REQUIRED)
  find_library(IOKIT IOKit REQUIRED)
  find_library(COREVIDEO CoreVideo REQUIRED)
  find_library(OPENGL OpenGL REQUIRED)
  target_link_libraries(simple_gl PRIVATE ${COCOA} ${IOKIT} ${COREVIDEO} ${OPENGL})
else()
  find_package(OpenGL REQUIRED)
  target_link_libraries(simple_gl PRIVATE OpenGL::GL dl)
endif()
