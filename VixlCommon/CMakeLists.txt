set(TARGET vixl_common)

set(INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    ../3rdParty/fmt/include
    )

set(SOURCES
    src/Error.cpp
    )

ADD_LIBRARY(${TARGET} STATIC ${SOURCES})

target_compile_features(${TARGET} PUBLIC cxx_std_17)

target_include_directories(${TARGET} PUBLIC ${INCLUDES})
target_link_libraries(${TARGET} ${LIBS})
target_compile_definitions(${TARGET} PRIVATE VERSION="${CMAKE_PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD_REQUIRED ON)
