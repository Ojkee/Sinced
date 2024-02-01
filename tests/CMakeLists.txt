# CMakeLists.txt (tests)

add_executable("test")

set(TARGET_SOURCES
    "test_cfg_main.cc"
    "test_date.cc"
)

#list(TRANSFORM TARGET_SOURCES PREPEND "${PROJECT_SOURCE_DIR}/test/")
target_sources("test" PUBLIC ${TARGET_SOURCES})

target_link_libraries("test" PUBLIC "mainsrc")
target_include_directories("test" PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories("test" PUBLIC "${PROJECT_SOURCE_DIR}/extern/include")
