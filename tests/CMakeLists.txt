# CMakeLists.txt (tests)

add_executable("test")

set(TARGET_SOURCES
    "test_cfg_main.cc"
    "test_date.cc"
    "test_entry.cc"
    "test_entry_handler.cc"
    "test_entry_handler_io.cc"
    "test_entry_filter.cc"
    "test_entry_sorter.cc"
    "test_entry_base_builder.cc"
    "test_lexer.cc"
    "test_interpreter.cc"
)

#list(TRANSFORM TARGET_SOURCES PREPEND "${PROJECT_SOURCE_DIR}/test/")
target_sources("test" PUBLIC ${TARGET_SOURCES})

target_link_libraries("test" PUBLIC "mainsrc")
target_include_directories("test" PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories("test" PUBLIC "${PROJECT_SOURCE_DIR}/extern/include")
