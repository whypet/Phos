project(Kernel LANGUAGES C)

set(CMAKE_C_STANDARD_LIBRARIES "")
add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
    "Main.c"
    "Phos.h"
	"PhosBase.h")

set_target_properties(${PROJECT_NAME} PROPERTIES
    SUFFIX ""
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
	OUTPUT_NAME "Phoskrnl")

target_compile_options(${PROJECT_NAME} PRIVATE "-mno-red-zone" "-fno-stack-protector" "-fshort-wchar" "-ffreestanding")
target_link_options(${PROJECT_NAME} PRIVATE "-Wl,-subsystem:native" "-Wl,-nodefaultlib" "-Wl,-entry:KiMain")

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    target_compile_options(${PROJECT_NAME} PRIVATE "-target" "x86_64-pc-win32-coff")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(FATAL_ERROR "Phos should compile under x86_64.")

	target_compile_options(${PROJECT_NAME} PRIVATE "-target" "i386-pc-win32-coff")
endif()

if((CMAKE_BUILD_TYPE STREQUAL "Debug") OR (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo"))
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")     
        add_definitions(-DDEBUG)
    endif()
    
    target_compile_options(${PROJECT_NAME} PRIVATE "-g" "-glldb" "-gdwarf-4")
    target_link_options(${PROJECT_NAME} PRIVATE "-g" "-glldb" "-gdwarf-4")
endif()