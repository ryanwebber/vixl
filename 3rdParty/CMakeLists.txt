add_subdirectory(entt)
add_subdirectory(fmt)
add_subdirectory(glfw)
add_subdirectory(glm)
add_subdirectory(json)
add_subdirectory(imgui-wrapper)
add_subdirectory(pixman-wrapper)

set(BGFX_BUILD_TOOLS ON CACHE BOOL "Enable building bgfx tools" FORCE)
set(BGFX_BUILD_EXAMPLES OFF CACHE BOOL "Disable building bgfx examples" FORCE)
add_subdirectory(bgfx)

set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "Avoid statically linking fmt twice" FORCE)
add_subdirectory(spdlog)

set(BUILD_TESTING OFF CACHE BOOL "Disable building uvw tests" FORCE)
set(BUILD_UVW_LIBS ON CACHE BOOL "Build a static library using the libuv cpp wrapper" FORCE)
add_subdirectory(uvw)
