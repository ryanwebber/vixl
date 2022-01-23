
set(TARGET imgui)

set(LIBS
    )

set(INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui"
    )

set(SOURCES
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    )

add_library(${TARGET} STATIC ${SOURCES})
target_include_directories(${TARGET} PUBLIC ${INCLUDES})
