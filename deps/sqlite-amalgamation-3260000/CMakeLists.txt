target_link_libraries(sqlite -lpthreads)

add_library (sqlite "${CMAKE_CURRENT_SOURCE_DIR}/sqlite.c")

target_include_directories (sqlite STATIC ${CMAKE_CURRENT_SOURCE_DIR})
