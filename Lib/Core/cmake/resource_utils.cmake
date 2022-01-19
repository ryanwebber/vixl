function(add_builtins_asset_pack_dependency target manifest_file cwd)
    set(out_dir ${CMAKE_CURRENT_BINARY_DIR}/include/VX/Core/Generated/Assets)
    set(listing_file ${out_dir}/builtins.h)
    set(output_file  ${out_dir}/builtins.asset)

    message(STATUS "Target asset pack:\n\ttarget: ${target}_assets\n\tresource: ${output_file}\n")

    add_custom_command(
        OUTPUT ${output_file} ${listing_file}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${out_dir}
        COMMAND asset-pack
            -i ${manifest_file}
            -o ${output_file}
            -l ${listing_file}
            -c ${cwd}
            -n "VX::Generated::Assets"
        DEPENDS asset-pack ${manifest_file}
    )

    add_custom_target(${target}_assets DEPENDS ${output_file} ${listing_file})
    add_dependencies(${target} ${target}_assets)

    SET_PROPERTY(GLOBAL PROPERTY VIXL_BUILTINS_ASSET_PACK ${output_file})
    SET_PROPERTY(GLOBAL PROPERTY VIXL_BUILTINS_ASSET_PACK_TARGET ${target}_assets)
endfunction()
