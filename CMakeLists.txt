cmake_minimum_required(VERSION 3.15)
project(Reparo VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ensure static linking
set(BUILD_SHARED_LIBS OFF)

if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# Find common required packages
find_package(SDL2 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenCV CONFIG REQUIRED)
find_package(Gettext REQUIRED)
find_package(Iconv REQUIRED)
find_package(ZXing CONFIG REQUIRED)

# Define the source directories
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(VENDOR_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor")

# Automatically collect source files
file(GLOB_RECURSE SRC_FILES 
    "${SOURCE_DIR}/*.cpp" 
    "${SOURCE_DIR}/*.h"
)

# Manually list vendor files since they are specific
set(VENDOR_FILES 
    ${VENDOR_DIR}/imgui/imgui.cpp
    ${VENDOR_DIR}/imgui/imgui_demo.cpp
    ${VENDOR_DIR}/imgui/imgui_draw.cpp
    ${VENDOR_DIR}/imgui/imgui_tables.cpp
    ${VENDOR_DIR}/imgui/imgui_widgets.cpp
    ${VENDOR_DIR}/imgui/imgui_impl_sdl2.cpp
    ${VENDOR_DIR}/imgui/imgui_impl_opengl3.cpp
    ${VENDOR_DIR}/imgui/imconfig.h
    ${VENDOR_DIR}/imgui/imgui.h
    ${VENDOR_DIR}/imgui/imgui_impl_opengl3.h
    ${VENDOR_DIR}/imgui/imgui_impl_sdl2.h
    ${VENDOR_DIR}/imgui/imgui_internal.h
    ${VENDOR_DIR}/imgui/imstb_rectpack.h
    ${VENDOR_DIR}/imgui/imstb_textedit.h
    ${VENDOR_DIR}/imgui/imstb_truetype.h
    ${VENDOR_DIR}/imgui/imgui_stdlib.h
    ${VENDOR_DIR}/imgui/imgui_stdlib.cpp
    ${VENDOR_DIR}/nlohmann/json.hpp
    ${VENDOR_DIR}/nlohmann/json_fwd.hpp
    ${VENDOR_DIR}/sqlite3/sqlite3.c
    ${VENDOR_DIR}/sqlite3/sqlite3.h
)

# Define the executable and add source files
add_executable(${PROJECT_NAME} 
    main.cpp
    ${VENDOR_FILES} 
    ${SRC_FILES}
)

# Platform-specific configurations
if(WIN32)
    set(VCPKG_TARGET_TRIPLET "x64-windows-static")
    
    # Find Windows-specific libraries
    find_library(INTL_LIBRARY
        NAMES intl libintl
        PATHS "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib"
        REQUIRED
    )
    find_library(ICONV_LIBRARY
        NAMES iconv libiconv
        PATHS "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib"
        REQUIRED
    )
    find_library(CHARSET_LIBRARY
        NAMES charset libcharset
        PATHS "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib"
        REQUIRED
    )

    # Create imported targets for Windows
    if(NOT TARGET Gettext::Lib)
        add_library(Gettext::Lib STATIC IMPORTED)
        set_target_properties(Gettext::Lib PROPERTIES
            IMPORTED_LOCATION "${INTL_LIBRARY}"
        )
    endif()
    if(NOT TARGET Iconv::Iconv)
        add_library(Iconv::Iconv STATIC IMPORTED)
        set_target_properties(Iconv::Iconv PROPERTIES
            IMPORTED_LOCATION "${ICONV_LIBRARY}"
        )
    endif()
    if(NOT TARGET Charset::Charset)
        add_library(Charset::Charset STATIC IMPORTED)
        set_target_properties(Charset::Charset PROPERTIES
            IMPORTED_LOCATION "${CHARSET_LIBRARY}"
        )
    endif()

    target_compile_definitions(${PROJECT_NAME} PRIVATE -DPLATFORM_WINDOWS $<$<CONFIG:Debug>:DEBUG>)
elseif(APPLE)
    set(VCPKG_TARGET_TRIPLET "x64-osx-static")
    
    # Find macOS gettext installation
    execute_process(
        COMMAND brew --prefix gettext
        OUTPUT_VARIABLE GETTEXT_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    
    # Find macOS-specific libraries
    find_library(INTL_LIBRARY
        NAMES intl libintl
        PATHS ${GETTEXT_PREFIX}/lib
        REQUIRED
    )

    target_compile_definitions(${PROJECT_NAME} PRIVATE -DPLATFORM_MAC)
    
    # Set RPATH for bundled libraries
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INSTALL_RPATH "@executable_path/../Frameworks"
    )
endif()

# Set include directories
target_include_directories(${PROJECT_NAME} 
    PRIVATE 
        ${SDL2_INCLUDE_DIRS} 
        ${OPENGL_INCLUDE_DIR}
        ${CMAKE_SOURCE_DIR}/vendor/imgui
	${CMAKE_SOURCE_DIR}/vendor/nlohmann
	${CMAKE_SOURCE_DIR}/vendor/sqlite3
        ${OpenCV_INCLUDE_DIRS}
)

# Add platform-specific include directories
if(APPLE)
    target_include_directories(${PROJECT_NAME} PRIVATE
        ${GETTEXT_PREFIX}/include
        ${GETTEXT_PREFIX}/include/gettext
    )
endif()

# Common libraries for both platforms
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        SDL2::SDL2-static
        SDL2::SDL2main 
        OpenGL::GL 
        ${OpenCV_LIBS}
        ZXing::Core
        ZXing::ZXing
)

# Platform-specific libraries
if(WIN32)
    target_link_libraries(${PROJECT_NAME} 
        PRIVATE 
            Gettext::Lib
            Iconv::Iconv
            Charset::Charset
    )
elseif(APPLE)
    target_link_libraries(${PROJECT_NAME} 
        PRIVATE 
            ${INTL_LIBRARY}
            iconv
    )
endif()

# Set up translations directory
set(LOCALE_INSTALL_DIR "${CMAKE_BINARY_DIR}/locale")
file(MAKE_DIRECTORY ${LOCALE_INSTALL_DIR})

# Add compile definitions for locale directory
target_compile_definitions(${PROJECT_NAME} 
    PRIVATE 
        LOCALE_DIR="${LOCALE_INSTALL_DIR}"
)

# Set up resource directory
set(RESOURCE_INSTALL_DIR "${CMAKE_BINARY_DIR}/resource")
file(MAKE_DIRECTORY ${RESOURCE_INSTALL_DIR})

# Add compile definitions for resource directory
target_compile_definitions(${PROJECT_NAME} 
    PRIVATE 
        RESOURCE_DIR="${RESOURCE_INSTALL_DIR}"
)
