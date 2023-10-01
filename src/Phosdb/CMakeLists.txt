project(Phosdb LANGUAGES CXX)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
    "Main.cpp"
	"Window.cpp"
	"Window.h")

target_link_options(${PROJECT_NAME} PRIVATE "-Wl,-subsystem:windows")

add_definitions(-DUNICODE)
add_definitions(-D_UNICODE)

if((CMAKE_BUILD_TYPE STREQUAL "Debug") OR (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo"))
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")     
        add_definitions(-DDEBUG)
    endif()
    
    target_compile_options(${PROJECT_NAME} PRIVATE "-g" "-glldb" "-gdwarf-4")
    target_link_options(${PROJECT_NAME} PRIVATE "-g" "-glldb" "-gdwarf-4")
endif()