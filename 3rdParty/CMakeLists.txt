# Ignore warnings for our 3rd party libs (SDL in particular here)
set(EXTRA_CFLAGS "${EXTRA_CFLAGS} -w")

add_subdirectory(entt)
add_subdirectory(fmt)
add_subdirectory(glm)
add_subdirectory(json)
add_subdirectory(sdl2)

set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "Avoid statically linking fmt twice" FORCE)
add_subdirectory(spdlog)

set(BUILD_TESTING OFF CACHE BOOL "Disable building uvw tests" FORCE)
set(BUILD_UVW_LIBS ON CACHE BOOL "Build a static library using the libuv cpp wrapper" FORCE)
add_subdirectory(uvw)
