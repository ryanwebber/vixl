add_subdirectory(entt)
add_subdirectory(fmt)
add_subdirectory(glm)
add_subdirectory(json)
add_subdirectory(sdl2)

set(BGFX_BUILD_TOOLS ON CACHE BOOL "Enable building bgfx tools" FORCE)
set(BGFX_BUILD_EXAMPLES OFF CACHE BOOL "Disable building bgfx examples" FORCE)
add_subdirectory(bgfx)

set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "Avoid statically linking fmt twice" FORCE)
add_subdirectory(spdlog)

# TODO: Uncomment when we bring in spirv-cross
#set(SPIRV_CROSS_EXCEPTIONS_TO_ASSERTIONS ON CACHE BOOL "Use assertions instead of c++ exceptions" FORCE)
#set(SPIRV_CROSS_STATIC ON CACHE BOOL "Build the static spirv-cross lib" FORCE)
#set(SPIRV_CROSS_ENABLE_TESTS OFF CACHE BOOL "Don't build the tests" FORCE)
#add_subdirectory(spirv-cross)

set(BUILD_TESTING OFF CACHE BOOL "Disable building uvw tests" FORCE)
set(BUILD_UVW_LIBS ON CACHE BOOL "Build a static library using the libuv cpp wrapper" FORCE)
add_subdirectory(uvw)
