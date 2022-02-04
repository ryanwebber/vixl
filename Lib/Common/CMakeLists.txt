set(TARGET vixl-common)

set(SOURCES
        src/Error.cpp
        )

ADD_LIBRARY(${TARGET} STATIC ${SOURCES})

target_compile_features(${TARGET} PUBLIC cxx_std_17)

target_include_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${TARGET}
        SYSTEM
        PUBLIC
            ${PROJECT_SOURCE_DIR}/3rdParty/fmt/include
        SYSTEM
        INTERFACE
            ${PROJECT_SOURCE_DIR}/3rdParty/expected/include
        )

target_compile_definitions(${TARGET} PRIVATE VERSION="${CMAKE_PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD_REQUIRED ON)

