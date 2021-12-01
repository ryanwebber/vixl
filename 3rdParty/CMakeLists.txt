add_subdirectory(fmt)
add_subdirectory(glfw)
add_subdirectory(glm)
add_subdirectory(imgui-wrapper)

set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "Avoid statically linking fmt twice" FORCE)
add_subdirectory(spdlog)
