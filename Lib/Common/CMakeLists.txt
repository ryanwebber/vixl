set(TARGET vixl-common)

set(INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    ../../3rdParty/fmt/include
    )

set(SOURCES
        src/Error.cpp
        )

ADD_LIBRARY(${TARGET} STATIC ${SOURCES})

target_compile_features(${TARGET} PUBLIC cxx_std_17)

target_include_directories(${TARGET} PUBLIC ${INCLUDES})
target_compile_definitions(${TARGET} PRIVATE VERSION="${CMAKE_PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD_REQUIRED ON)

