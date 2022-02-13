set(TARGET bin2cpp)

set(LIBS)

set(SOURCES
        main.cpp
        )

add_executable(${TARGET} ${SOURCES})
target_link_libraries(${TARGET} PRIVATE vixl-argparse)
