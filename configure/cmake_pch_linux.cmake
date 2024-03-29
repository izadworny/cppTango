#
# Adds precompiled header (generated by the tango_pch target) to the ${target}.
#
function(tango_target_pch target)
    target_compile_options(${target} PRIVATE
        -Winvalid-pch
        -include "${CMAKE_BINARY_DIR}/cppapi/server/tango.h")
    add_dependencies(${target} tango_pch)
endfunction()

#
# Adds precompiled header (generated by the tango_test_pch target) to the ${target}.
#
function(tango_target_test_pch target)
    target_compile_options(${target} PRIVATE
        -Winvalid-pch
        -include "${CMAKE_BINARY_DIR}/tests/tango.h")
    add_dependencies(${target} tango_test_pch)
endfunction()

#
# Defines ${pch_target} target that precompiles tango.h file.
# PCH uses the same flags as defined on the ${ref_target}.
#
function(tango_add_pch pch_target ref_target pch_extra_cxx_flags)

    set(tango_h_file "${CMAKE_SOURCE_DIR}/cppapi/server/tango.h")
    set(tango_pch_file "${CMAKE_CURRENT_BINARY_DIR}/tango.h.gch")
    set(tango_pch_h_file "${CMAKE_CURRENT_BINARY_DIR}/tango.h")

    string(TOUPPER ${CMAKE_BUILD_TYPE} build_type_suffix)

    separate_arguments(
        pch_cxx_flags
        UNIX_COMMAND
        "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_${build_type_suffix}}")

    get_target_property(pch_compile_defs ${ref_target} COMPILE_DEFINITIONS)
    get_target_property(pch_include_dirs ${ref_target} INCLUDE_DIRECTORIES)
    get_target_property(pch_compile_opts ${ref_target} COMPILE_OPTIONS)

    get_directory_property(dir_compile_defs COMPILE_DEFINITIONS)
    list(APPEND pch_compile_defs ${dir_compile_defs})

    string(REPLACE ";" ";-D" pch_compile_defs "-D${pch_compile_defs}")
    string(REPLACE ";" ";-I" pch_include_dirs "-I${pch_include_dirs}")

    set(pch_compile_flags
        ${pch_cxx_flags}
        ${pch_compile_defs}
        ${pch_include_dirs}
        ${pch_compile_opts}
        ${pch_extra_cxx_flags}
        -x c++-header
        -std=c++${CMAKE_CXX_STANDARD}
        -c
        -o ${tango_pch_file})

    set_source_files_properties(${tango_pch_file} PROPERTIES GENERATED TRUE)

    add_custom_command(
        OUTPUT ${tango_pch_file}
        COMMAND ${CMAKE_CXX_COMPILER} ${pch_compile_flags} ${tango_h_file}
        COMMENT "Precompiling tango.h (${pch_target})"
        MAIN_DEPENDENCY ${tango_h_file}
        DEPENDS ${tango_pch_h_file})

    add_custom_command(
        OUTPUT ${tango_pch_h_file}
        COMMAND ${CMAKE_COMMAND} -E copy ${tango_h_file} ${tango_pch_h_file}
        COMMENT "Copying tango.h to PCH directory (${pch_target})"
        MAIN_DEPENDENCY ${tango_h_file})

    add_custom_target(${pch_target}
        DEPENDS ${tango_pch_h_file} ${tango_pch_file} idl_source)

endfunction()
