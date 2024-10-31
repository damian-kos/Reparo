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
find_package(PostgreSQL REQUIRED)

# Define the executable and add source files first
add_executable(${PROJECT_NAME} 
    main.cpp
    vendor/imgui/imgui.cpp
    vendor/imgui/imgui_demo.cpp
    vendor/imgui/imgui_draw.cpp
    vendor/imgui/imgui_tables.cpp
    vendor/imgui/imgui_widgets.cpp
    vendor/imgui/imgui_impl_sdl2.cpp
    vendor/imgui/imgui_impl_opengl3.cpp
    vendor/imgui/imconfig.h
    vendor/imgui/imgui.h
    vendor/imgui/imgui_impl_opengl3.h
    vendor/imgui/imgui_impl_opengl3_loader.h
    vendor/imgui/imgui_impl_sdl2.h
    vendor/imgui/imgui_internal.h
    vendor/imgui/imstb_rectpack.h
    vendor/imgui/imstb_textedit.h
    vendor/imgui/imstb_truetype.h
    vendor/imgui/imgui_stdlib.h
    vendor/imgui/imgui_stdlib.cpp
    vendor/nlohmann/json.hpp
    vendor/nlohmann/json_fwd.hpp
    src/RoImage.h
    src/RoImage.cpp
    src/RoLocalization.h
    src/RoLocalization.cpp
    src/RoBarcode.h
    src/RoBarcode.cpp
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

    target_compile_definitions(${PROJECT_NAME} PRIVATE -DPLATFORM_WINDOWS)
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
	PostgreSQL::PostgreSQL
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