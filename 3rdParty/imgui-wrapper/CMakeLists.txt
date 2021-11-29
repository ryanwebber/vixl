
set(TARGET imgui)

set(LIBS
    )

set(INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui"
    "../GLFW/include"
    )

set(SOURCES
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
    )

add_library(${TARGET} STATIC ${SOURCES})
target_include_directories(${TARGET} PUBLIC ${INCLUDES})
