# Compiles a shader into a platform-compatible binary shader
# and writes it to a generated c header file.
#
# See the documentation for `shaderc` for details on the
# arguments this function takes
function(add_compiled_shader)
    cmake_parse_arguments(
            ARG
            ""
            "TARGET;TYPE;PLATFORM;PROFILE;DEFINITION;SRC;OUTPUT;NAME"
            "INCLUDES;DEPS"
            ${ARGN}
    )

    get_filename_component(SHADER_DIR ${ARG_OUTPUT} DIRECTORY)
    list(TRANSFORM ARG_INCLUDES PREPEND "-i;")

    set(hello shaderc
            -f ${ARG_SRC}
            -o ${ARG_OUTPUT}
            --type ${ARG_TYPE}
            --bin2c ${ARG_NAME}
            --varyingdef ${ARG_DEFINITION}
            ${ARG_INCLUDES}
            --platform ${ARG_PLATFORM}
            --profile ${ARG_PROFILE})

    add_custom_command(
            OUTPUT ${ARG_OUTPUT}
            COMMAND mkdir -p ${SHADER_DIR}
            COMMAND shaderc
                -f ${ARG_SRC}
                -o ${ARG_OUTPUT}
                --type ${ARG_TYPE}
                --bin2c ${ARG_NAME}
                --varyingdef ${ARG_DEFINITION}
                ${ARG_INCLUDES}
                --platform ${ARG_PLATFORM}
                --profile ${ARG_PROFILE}
            DEPENDS shaderc ${ARG_SRC} ${ARG_DEFINITION}
    )

    # Create a target for the newly generated thing
    add_custom_target(${ARG_TARGET} DEPENDS ${ARG_OUTPUT})

    # Add a dependencies on this new target
    foreach (DEP IN LISTS ARG_DEPS)
        add_dependencies(${DEP} ${ARG_TARGET})
    endforeach()
endfunction()
