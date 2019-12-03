# add_subdirectory(sqlite-amalgamation-3260000)

# add_subdirectory(date)

set(HinnantDate_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/date")
set(ENABLE_TESTS off)
add_subdirectory(sqlpp11-connector-sqlite3)
