project(Boot LANGUAGES C)

set(CMAKE_C_STANDARD_LIBRARIES "")
add_executable(${PROJECT_NAME} ${UEFI_SOURCE})

target_sources(${PROJECT_NAME} PRIVATE
    Main.c
    PhosBoot.h
    Debug.c
    Debug.h
    Guids.c
    Memory.c
    Memory.h
    String.c
    String.h)

set_target_properties(${PROJECT_NAME} PROPERTIES
    SUFFIX ".efi"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Efi/Boot")

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE "../../thirdparty/edk2/MdePkg/Include" "../../thirdparty/edk2/MdePkg/Include/X64")
target_compile_options(${PROJECT_NAME} PRIVATE "-mno-red-zone" "-fno-stack-protector" "-fshort-wchar")
target_link_options(${PROJECT_NAME} PRIVATE "-Wl,-subsystem:efi_application" "-Wl,-nodefaultlib" "-Wl,-entry:EfiMain")

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "Bootx64")
    target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE "../../thirdparty/edk2/MdePkg/Include/X64")
    target_compile_options(${PROJECT_NAME} PRIVATE "-target" "x86_64-pc-win32-coff")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "Bootx86")
    target_compile_options(${PROJECT_NAME} PRIVATE "-target" "i386-pc-win32-coff")
endif()

if((CMAKE_BUILD_TYPE STREQUAL "Debug") OR (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo"))
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")     
        add_definitions(-DDEBUG)
    endif()
    
    target_compile_options(${PROJECT_NAME} PRIVATE "-g" "-glldb" "-gdwarf-4")
    target_link_options(${PROJECT_NAME} PRIVATE "-g" "-glldb" "-gdwarf-4")
endif()