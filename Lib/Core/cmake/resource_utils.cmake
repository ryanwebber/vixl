# Runs asset-pack and creates a new target for the generated
# asset bundle. Will also add dependencies on the resources
# listed in the manifest file
#
# See the documentation for `asset-pack` (Tools/AssetPack)
# for more info on the required arguments
function(add_packed_asset)
    cmake_parse_arguments(
            ARG
            ""
            "TARGET;MANIFEST;OUTPUT;LISTING;NAMESPACE"
            "DEPS"
            ${ARGN}
    )

    get_filename_component(OUTPUT_DIR ${ARG_OUTPUT} DIRECTORY)
    get_filename_component(LISTING_DIR ${ARG_LISTING} DIRECTORY)
    get_filename_component(RESOURCE_DIR ${ARG_MANIFEST} DIRECTORY)

    # Record the manifest input files so we can use them as proper input
    file(READ ${ARG_MANIFEST} MANIFEST_CONTENTS)
    string(JSON ASSET_COUNT LENGTH ${MANIFEST_CONTENTS} "resources")
    math(EXPR ASSET_COUNT_MINUS_1 "${ASSET_COUNT}-1")
    foreach(IDX RANGE ${ASSET_COUNT_MINUS_1})
        string(JSON ASSET_PATH GET ${MANIFEST_CONTENTS} "resources" ${IDX} src)
        list(APPEND RESOURCE_LIST ${RESOURCE_DIR}/${ASSET_PATH})
    endforeach()

    add_custom_command(
            OUTPUT ${ARG_OUTPUT} ${ARG_LISTING}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_DIR}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${LISTING_DIR}
            COMMAND asset-pack
                -i ${ARG_MANIFEST}
                -o ${ARG_OUTPUT}
                -l ${ARG_LISTING}
                -n ${ARG_NAMESPACE}
                -c ${RESOURCE_DIR}
            DEPENDS asset-pack ${ARG_MANIFEST} ${RESOURCE_LIST}
    )

    add_custom_target(${ARG_TARGET} DEPENDS ${ARG_OUTPUT} ${ARG_LISTING})
    set_target_properties(${ARG_TARGET} PROPERTIES
            ASSET_BUNDLE ${ARG_OUTPUT}
            ASSET_LISTING ${ARG_LISTING})

    # Add a dependencies on this new target
    foreach (DEP IN LISTS ARG_DEPS)
        add_dependencies(${DEP} ${ARG_TARGET})
    endforeach()
endfunction()
