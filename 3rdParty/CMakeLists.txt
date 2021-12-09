add_subdirectory(fmt)
add_subdirectory(glfw)
add_subdirectory(glm)
add_subdirectory(imgui-wrapper)
add_subdirectory(pixman-wrapper)

set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "Avoid statically linking fmt twice" FORCE)
add_subdirectory(spdlog)

set(BUILD_UVW_LIBS ON CACHE BOOL "Build a static library using the libuv cpp wrapper" FORCE)
set(USE_LIBCPP OFF CACHE BOOL "Link libc++ in executable" FORCE)
add_subdirectory(uvw)
