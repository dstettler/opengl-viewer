cmake_minimum_required(VERSION 3.16)
project(OpenGLViewer VERSION 1.0 LANGUAGES CXX)

message(PLATFORM=${CMAKE_SYSTEM_NAME})

set (CMAKE_DEBUG_POSTFIX d)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ADDITIONAL_CFLAGS}")

# We need this on all platforms
find_package(GLEW REQUIRED)

# Platform-specific packages for linking- I don't have access to a Mac
# to test what needs to be done on that platform
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    find_package(X11 REQUIRED)
    find_package(OpenGL REQUIRED)
endif()

# We'll build glfw ourselves regardless of platform
add_subdirectory(glfw)

add_subdirectory(glm)

find_package(glm CONFIG REQUIRED)

include_directories(include)
include_directories(imgui)
include_directories(imgui/backends)

# Add all source *and* header files here
set(PROJECT_SOURCES
    src/main.cpp

    imgui/imgui.h
    imgui/imconfig.h
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_internal.h
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui.cpp
    imgui/imstb_rectpack.h
    imgui/imstb_textedit.h
    imgui/imstb_truetype.h

    imgui/backends/imgui_impl_glfw.h
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.h
    imgui/backends/imgui_impl_opengl3.cpp
)

# Add all project source files
add_executable(OpenGLViewer ${PROJECT_SOURCES})

# Link libraries
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(OpenGLViewer GLEW::glew glfw glm::glm)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
target_link_libraries(OpenGLViewer ${X11_LIBRARIES} ${OPENGL_LIBRARIES} GLEW::glew glfw glm::glm)
endif()

# I do not know if the MACOSX properties are proper or not, as I cannot test them.
set_target_properties(OpenGLViewer PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER OpenGLViewer.dstet.me
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
)
