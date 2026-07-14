set(SDL_SHARED ON CACHE BOOL "Build SDL as a shared library" FORCE)
set(SDL_STATIC OFF CACHE BOOL "Build SDL as a static library" FORCE)

add_subdirectory(SDL)

add_library(external INTERFACE)
target_link_libraries(external INTERFACE SDL3::SDL3-shared)
