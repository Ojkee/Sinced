# CMakeLists.txt (src)

add_library(mainsrc SHARED
    date/base_date.cc
    date/format_date.cc
    entry/entry_handler.cc
    entry/entry_base.cc
    entry/entry_sorter.cc
    entry/entry_filter.cc
    parser/parser.cc
)

target_include_directories(mainsrc PUBLIC "${PROJECT_SOURCE_DIR}/include")

add_executable(main
    main.cc
)
target_link_libraries(main PUBLIC mainsrc)
target_include_directories(main PUBLIC "${PROJECT_SOURCE_DIR}/include")
