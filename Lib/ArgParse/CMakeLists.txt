set(TARGET vixl-argparse)

set(SOURCES
        src/ArgParse.cpp
        )

ADD_LIBRARY(${TARGET} STATIC ${SOURCES})

target_compile_features(${TARGET} PUBLIC cxx_std_17)

target_include_directories(${TARGET}
        PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        )

target_compile_definitions(${TARGET} PRIVATE VERSION="${CMAKE_PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD_REQUIRED ON)

