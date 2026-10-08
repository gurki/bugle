function( checked )
    execute_process( COMMAND ${ARGV} RESULT_VARIABLE result )
    if( NOT result EQUAL 0 )
        message( FATAL_ERROR "Package integration failed: ${ARGV}" )
    endif()
endfunction()

set( prefix "${BUGLE_TEST_DIR}/package-install" )
checked( "${CMAKE_COMMAND}" --install "${BUGLE_BUILD_DIR}" --config "${BUGLE_CONFIG}" --prefix "${prefix}" )
checked( "${CMAKE_COMMAND}" -S "${BUGLE_SOURCE_DIR}/package" -B "${BUGLE_TEST_DIR}/package-build"
    -G "${BUGLE_GENERATOR}" "-DCMAKE_PREFIX_PATH=${prefix}" "-DCMAKE_CXX_COMPILER=${BUGLE_COMPILER}"
    "-DCMAKE_BUILD_TYPE=${BUGLE_CONFIG}" )
checked( "${CMAKE_COMMAND}" --build "${BUGLE_TEST_DIR}/package-build" --config "${BUGLE_CONFIG}" )
checked( "${CMAKE_CTEST_COMMAND}" --test-dir "${BUGLE_TEST_DIR}/package-build" -C "${BUGLE_CONFIG}" --output-on-failure )
