set(LOG4TANGO_TEST_SOURCES test_log4tango.cpp)
set(BENCH_TEST_SOURCES clock.cpp clock.hh test_bench.cpp)

if(WIN32)
    add_executable(test_log4tango ${LOG4TANGO_TEST_SOURCES} $<TARGET_OBJECTS:log4tango_objects>)
    add_executable(test_bench ${BENCH_TEST_SOURCES} $<TARGET_OBJECTS:log4tango_objects>)
    target_compile_definitions(test_log4tango PRIVATE "${static_defs}")
    target_compile_definitions(test_bench PRIVATE "${static_defs}")

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(test_log4tango PRIVATE "/MTd")
        target_compile_options(test_bench PRIVATE "/MTd")
    else()
        target_compile_options(test_log4tango PRIVATE "/MT")
        target_compile_options(test_bench PRIVATE "/MT")
    endif()

    add_test("log4tango_test"       test_log4tango)
#    add_test("log4tango_benchmark"  test_bench)

else()
    add_executable(test_log4tango ${LOG4TANGO_TEST_SOURCES} $<TARGET_OBJECTS:log4tango_objects>)
    add_executable(test_bench ${BENCH_TEST_SOURCES} $<TARGET_OBJECTS:log4tango_objects>)

    add_test("log4tango_test"       test_log4tango)
#    add_test("log4tango_benchmark"  test_bench)
endif()
