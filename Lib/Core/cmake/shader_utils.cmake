function(add_compiled_shader_dependency target type dir)
    get_filename_component(shader_basename ${dir} NAME)
    set(shader_target shader_${shader_basename}_${type})
    set(shader_output ${CMAKE_CURRENT_BINARY_DIR}/include/Core/Generated/Shader/${shader_basename}/${type}.h)
    set(shader_source ${CMAKE_CURRENT_SOURCE_DIR}/${dir}/${type}.sc)
    set(shader_def_source ${CMAKE_CURRENT_SOURCE_DIR}/${dir}/varying.def.sc)
    message(STATUS "Shader dependency:\n\ttype: ${type}\n\tname: ${shader_target}\n\ttarget: ${target}\n\tsource: ${shader_source}\n\tdest: ${shader_output}\n")

    add_custom_command(
            OUTPUT ${shader_output}
            COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/include/Core/Generated/Shader/${shader_basename}
            COMMAND shaderc
                -f ${shader_source}
                -o ${shader_output}
                --type ${type}
                --bin2c ${shader_basename}_${type}_shader
                --varyingdef ${shader_def_source}
                -i ${PROJECT_SOURCE_DIR}/3rdParty/bgfx/bgfx/src
                --platform osx
                -p metal
                --verbose
            DEPENDS shaderc ${shader_source} ${shader_def_source}
    )

    add_custom_target(${shader_target} DEPENDS ${shader_output})
    add_dependencies(${target} ${shader_target})
endfunction()
