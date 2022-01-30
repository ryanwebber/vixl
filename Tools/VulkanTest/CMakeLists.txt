set(TARGET vulkan-test)

set(LIBS)

set(SOURCES
        main.cpp
        )

add_executable(${TARGET} ${SOURCES})

target_link_libraries(${TARGET}
        PRIVATE
            vixl-graphics
            SDL2-static
        )

target_include_directories(${TARGET}
        PRIVATE
            ${PROJECT_SOURCE_DIR}/Lib/Graphics/include
            ${PROJECT_SOURCE_DIR}/3rdParty/sdl2/include
        )
