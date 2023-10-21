project(Boot LANGUAGES C)

set(CMAKE_C_STANDARD_LIBRARIES "")
add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
    "Debug.c"
    "Debug.h"
    "Guids.c"
    "Loader.c"
    "Loader.h"
    "Main.c"
    "Memory.c"
    "Memory.h"
    "PhosBoot.h"
    "String.c"
    "String.h")

set_target_properties(${PROJECT_NAME} PROPERTIES
    SUFFIX ".efi"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Efi/Boot")

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE "../../thirdparty/edk2/MdePkg/Include" "../../thirdparty/edk2/MdePkg/Include/X64")
target_compile_options(${PROJECT_NAME} PRIVATE "-mno-red-zone" "-fno-stack-protector" "-fshort-wchar" "-ffreestanding")
target_link_options(${PROJECT_NAME} PRIVATE "-Wl,-subsystem:efi_application" "-Wl,-nodefaultlib" "-Wl,-entry:EfiMain")

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "Bootx64")
    target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE "../../thirdparty/edk2/MdePkg/Include/X64")
    target_compile_options(${PROJECT_NAME} PRIVATE "-target" "x86_64-pc-win32-coff")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(FATAL_ERROR "Phos should compile under x86_64.")

    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "Bootx86")
    target_compile_options(${PROJECT_NAME} PRIVATE "-target" "i386-pc-win32-coff")
endif()

if(PHOSBOOT_DEBUG STREQUAL "YES")
	add_compile_definitions(DEBUGGER)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_compile_definitions(DEBUG)
    target_compile_options(${PROJECT_NAME} PRIVATE "-g" "-glldb" "-gdwarf-4")
    target_link_options(${PROJECT_NAME} PRIVATE "-g" "-glldb" "-gdwarf-4")
endif()