set(TARGET asset-pack)

set(LIBS)

set(SOURCES
        main.cpp
        )

add_executable(${TARGET} ${SOURCES})

target_link_libraries(${TARGET} PRIVATE nlohmann_json fmt vixl-argparse)
target_include_directories(${TARGET} PRIVATE "../../3rdParty/json/single_include")
