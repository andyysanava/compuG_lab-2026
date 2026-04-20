cmake_minimum_required(VERSION 3.10)
project(OpenGL_Animacion_Basica)
# --- 1. SETTINGS & PATHS ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Use quotes because of the space in "External Libraries"
set(EXT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/External Libraries")
# --- 2. SHARED INCLUDES (Linux & Windows) ---
include_directories(
    "include"
    "${EXT_DIR}/glm"
    "${EXT_DIR}"
)
# --- 3. PLATFORM LOGIC ---
if(WIN32)
    # WINDOWS (Professor's Setup)
    # Assimp headers come from the local External Libraries folder
    include_directories("${EXT_DIR}/assimp/include")

    set(USER_LIBS 
        "${EXT_DIR}/assimp/lib/assimp-vc142-mt.lib" 
        opengl32.lib 
        glu32.lib 
        glew32.lib 
        glfw3.lib
    )
    
    # Automatically copy Assimp DLLs to the build folder so the .exe runs
    file(GLOB DLL_FILES "${EXT_DIR}/assimp/lib/*.dll")
    if(DLL_FILES)
        file(COPY ${DLL_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    endif()
else()
    # LINUX (Your Setup)
    find_package(OpenGL REQUIRED)
    find_package(GLEW REQUIRED)
    find_package(assimp REQUIRED)
    find_package(Threads REQUIRED)

    # Force the correct assimp headers first — avoids picking up mismatched versions
    get_target_property(ASSIMP_INCLUDE_DIRS assimp::assimp INTERFACE_INCLUDE_DIRECTORIES)
    include_directories(BEFORE ${ASSIMP_INCLUDE_DIRS})

    # Try to find GLFW3
    find_package(glfw3 QUIET)
    if(NOT glfw3_FOUND)
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(GLFW REQUIRED glfw3)
    endif()
    include_directories(${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS})
    
    set(USER_LIBS 
        ${OPENGL_LIBRARIES} 
        GLEW::GLEW 
        assimp::assimp 
        glfw 
        ${CMAKE_DL_LIBS} 
        Threads::Threads
    )
endif()
# --- 4. SOURCE FILES ---
# Compiling SOIL2 from the local source files in your directory
file(GLOB SOIL2_SOURCES "${EXT_DIR}/SOIL2/*.c")
# Updated to use Iluminacion.cpp
add_executable(Animacion_Basica 
    Animacion_Basica.cpp
    Camera.h 
    Mesh.h 
    Model.h 
    Shader.h 
    stb_image.h 
    ${SOIL2_SOURCES}
)
target_link_libraries(Animacion_Basica PUBLIC ${USER_LIBS})
# --- 5. ASSET MANAGEMENT ---
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/Shader" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/images" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/Models" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
