cmake_minimum_required(VERSION 3.19)
project(PGE_Test)

# Set C++ Standards
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_WAYLAND "Build example programs with Wayland" OFF)
option(USE_STB "Use stb_image.h for image loading" OFF)
option(DISABLE_LIBDECOR "Disable libdecor usage in wayland builds" OFF)
option(FORCE_WAYLAND_LIBDECOR "Force wayland builds to only use libdecor" OFF)

######################################################################
file(GLOB SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_executable(Main)
target_sources(Main
  PRIVATE
    ${SOURCES}
)
target_include_directories(Main PRIVATE includes)

if(EMSCRIPTEN)
    target_link_options(Main PRIVATE -sASYNCIFY)
    target_link_options(Main PRIVATE -sALLOW_MEMORY_GROWTH=1)
    target_link_options(Main PRIVATE -sSTACK_SIZE=1048576)
    target_link_options(Main PRIVATE -sEXPORTED_RUNTIME_METHODS=HEAPF32)
    target_link_options(Main PRIVATE -sMAX_WEBGL_VERSION=2)
    target_link_options(Main PRIVATE -sMIN_WEBGL_VERSION=2)
    target_link_options(Main PRIVATE -sUSE_LIBPNG=1)
    target_link_options(Main PRIVATE -sLLD_REPORT_UNDEFINED)
    target_link_options(Main PRIVATE --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/assets@assets)
    # build Cache: libpng, zlib
    execute_process(COMMAND "${EMSCRIPTEN_ROOT_PATH}/embuilder${EMCC_SUFFIX}" build libpng zlib)
    set(CMAKE_EXECUTABLE_SUFFIX .html)
else()
    target_link_libraries(Main PRIVATE png)
    target_link_libraries(Main PRIVATE GL)
    target_link_libraries(Main PRIVATE pthread)
    target_link_libraries(Main PRIVATE X11)
    target_link_libraries(Main PRIVATE Xi)
endif()

foreach(source ${SOURCES})
    #get_filename_component(EXE_NAME ${source} NAME_WE)
    
    # if(USE_STB)
    #     target_compile_definitions(${EXE_NAME} PRIVATE OLC_USE_STB_IMAGE=1)
    # endif()

    if(NOT EMSCRIPTEN)
        # Copy assets when not Web platform
        # add_custom_command(
        #     TARGET ${EXE_NAME}
        #     POST_BUILD
        #     COMMAND ${CMAKE_COMMAND}
        #     ARGS -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/assets
        # )        
    endif()
endforeach()
