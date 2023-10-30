project(Phoskrnl LANGUAGES C)

enable_language(ASM_NASM)

set(CMAKE_C_STANDARD_LIBRARIES "")
add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
	"../Shared/Debug/Protocol.c"
	"../Shared/Debug/Protocol.h"
	"../Shared/Intrinsics.h"
	"../Shared/Phosdef.h"
	"../Shared/Phoskinit.h"
	"Hal/Gdt.c" 
	"Hal/Gdtr.asm"
	"Hal/Hal.h"
	"Hal/Serial.c"
	"Kd/Kd.c"
	"Kd/Kd.h"
	"Ke/Ke.c"
	"Ke/Ke.h"
	"Rtl/Memory.c"
	"Rtl/Rtl.h"
	"Rtl/String.c"
    "Main.c"
    "Phos.h"
	"Phosbase.h")

set_target_properties(${PROJECT_NAME} PROPERTIES
    SUFFIX ""
	OUTPUT_NAME "Phoskrnl"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/System")

add_compile_definitions(PHOSKRNL)

target_include_directories(${PROJECT_NAME} PRIVATE "../Shared" ".")
target_compile_options(${PROJECT_NAME} PRIVATE
	$<$<COMPILE_LANGUAGE:C>:
		"-mno-red-zone"
		"-fno-stack-protector"
		"-fno-short-wchar"
		"-ffreestanding"
		"-fmsc-version=0"
		"-mno-stack-arg-probe">)
target_link_options(${PROJECT_NAME} PRIVATE "-Wl,-subsystem:native" "-Wl,-nodefaultlib" "-Wl,-entry:KiMain")

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    target_compile_options(${PROJECT_NAME} PRIVATE
		$<$<COMPILE_LANGUAGE:C>:
			"-target"
			"x86_64-pc-win32-coff">)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(FATAL_ERROR "Phos should compile under x86_64.")

	target_compile_options(${PROJECT_NAME} PRIVATE
		$<$<COMPILE_LANGUAGE:C>:
			"-target"
			"i386-pc-win32-coff">)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_compile_definitions(DEBUG)

    target_compile_options(${PROJECT_NAME} PRIVATE
		$<$<COMPILE_LANGUAGE:C>:
			"-g"
			"-glldb"
			"-gdwarf-4">)

    target_link_options(${PROJECT_NAME} PRIVATE "-g" "-glldb" "-gdwarf-4")
endif()