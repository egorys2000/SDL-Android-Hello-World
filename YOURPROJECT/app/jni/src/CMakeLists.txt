cmake_minimum_required(VERSION 3.6)

project(MY_APP)

find_library(SDL2 SDL2)

# Find SDL_Image library
find_library(SDL2_IMAGE SDL2_image)

add_library(main SHARED)

target_sources(main PRIVATE YourSourceHere.c)

# Link against SDL2 and SDL_Image
target_link_libraries(main SDL2)
target_link_libraries(main SDL2_IMAGE)
