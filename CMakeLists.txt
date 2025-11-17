cmake_minimum_required(VERSION 3.16)
project(PateGraphiqueThermalRun VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(BUILD_WITH_DISCORD "Link against Discord Game SDK" OFF)

add_subdirectory(third_party)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS
    src/*.cpp
)

add_executable(PateGraphique ${GAME_SOURCES})

target_include_directories(PateGraphique PRIVATE
    src
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
)

if (BUILD_WITH_DISCORD)
    target_compile_definitions(PateGraphique PRIVATE USE_DISCORD_SDK)
    target_include_directories(PateGraphique PRIVATE ${DISCORD_SDK_INCLUDE_DIR})
    target_link_libraries(PateGraphique PRIVATE ${DISCORD_SDK_LIB})
endif()

target_link_libraries(PateGraphique PRIVATE
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    nlohmann_json::nlohmann_json
)

install(TARGETS PateGraphique RUNTIME DESTINATION bin)
