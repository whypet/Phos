project(Phosdb LANGUAGES C CXX)

option(WINDOWS_SUBSYSTEM "Compiles using the Windows subsystem.")

add_subdirectory("../../thirdparty/DirectXMath" "DirectXMath")

set(FT_DISABLE_ZLIB TRUE)
set(FT_DISABLE_BZIP2 TRUE)
set(FT_DISABLE_PNG TRUE)
set(FT_DISABLE_HARFBUZZ TRUE)
set(FT_DISABLE_BROTLI TRUE)

add_subdirectory("../../thirdparty/freetype" "freetype")

add_library(ImGui)

target_sources(ImGui PRIVATE
	"../../thirdparty/imgui/imgui_demo.cpp"
	"../../thirdparty/imgui/imgui_draw.cpp"
	"../../thirdparty/imgui/imgui_internal.h"
	"../../thirdparty/imgui/imgui_tables.cpp"
	"../../thirdparty/imgui/imgui_widgets.cpp"
	"../../thirdparty/imgui/imgui.cpp"
	"../../thirdparty/imgui/imgui.h"
	"../../thirdparty/imgui/backends/imgui_impl_dx11.cpp"
	"../../thirdparty/imgui/backends/imgui_impl_dx11.h"
	"../../thirdparty/imgui/backends/imgui_impl_win32.cpp"
	"../../thirdparty/imgui/backends/imgui_impl_win32.h"
	"../../thirdparty/imgui/misc/cpp/imgui_stdlib.cpp"
	"../../thirdparty/imgui/misc/cpp/imgui_stdlib.h"
	"../../thirdparty/imgui/misc/freetype/imgui_freetype.cpp"
	"../../thirdparty/imgui/misc/freetype/imgui_freetype.h")

target_include_directories(ImGui SYSTEM PUBLIC "../../thirdparty/freetype/include")
target_include_directories(ImGui SYSTEM PUBLIC "../../thirdparty/imgui")
target_include_directories(ImGui SYSTEM PUBLIC "../../thirdparty/imgui/backends")
target_include_directories(ImGui SYSTEM PUBLIC "../../thirdparty/imgui/misc/cpp")
target_include_directories(ImGui SYSTEM PUBLIC "../../thirdparty/imgui/misc/freetype")

target_compile_options(ImGui PUBLIC "-DIMGUI_USER_CONFIG=\"${CMAKE_CURRENT_SOURCE_DIR}/ImConfig.hh\"")

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
	"../Shared/Debug/Protocol.c"
	"../Shared/Debug/Protocol.h"
	"Clients/SerialClient.cc"
	"Clients/SerialClient.hh"
	"Clients/TcpClient.cc"
	"Clients/TcpClient.hh"
	"GUI/SerialWindow.cc"
	"GUI/SerialWindow.hh"
	"GUI/Window.hh"
	"Direct3D.cc"
	"Direct3D.hh"
	"ImConfig.hh"
	"Logger.cc"
	"Logger.h"
	"Logger.hh"
	"Main.cc"
	"Phosdb.hh"
	"Util.cc"
	"Util.hh"
	"Window.cc"
	"Window.hh")

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

add_compile_definitions(PHOSDB)
add_compile_definitions(UNICODE)
add_compile_definitions(_UNICODE)

target_include_directories(${PROJECT_NAME} PRIVATE "../Shared" ".")
target_compile_options(${PROJECT_NAME} PRIVATE "-Wall" "-fshort-wchar")
target_link_libraries(${PROJECT_NAME} PRIVATE ImGui freetype "d3d11.lib" "ws2_32.lib" "ntdll.lib")

if (WINDOWS_SUBSYSTEM)
	add_compile_definitions(WINMAIN)
	target_link_options(${PROJECT_NAME} PRIVATE "-Wl,-subsystem:windows")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_compile_definitions(DEBUG)
    target_compile_options(${PROJECT_NAME} PRIVATE "-g" "-ggdb" "-gdwarf-4")
    target_link_options(${PROJECT_NAME} PRIVATE "-g" "-ggdb" "-gdwarf-4")
endif()

cmake_path(SET    FONTS_PATH "$ENV{windir}")
cmake_path(APPEND FONTS_PATH "Fonts")

cmake_path(SET    TAHOMA_FONT ${FONTS_PATH})
cmake_path(APPEND TAHOMA_FONT "tahoma.ttf")

if(EXISTS ${TAHOMA_FONT})
	configure_file(${TAHOMA_FONT} "Res/tahoma.ttf" COPYONLY)
else()
	message("Could not find Tahoma font. Add 'Res/tahoma.ttf' in the build directory manually.")
endif()