add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE main.cc)

target_link_libraries(${PROJECT_NAME} PRIVATE vendor)
target_link_libraries(${PROJECT_NAME} PRIVATE Lib)

set(DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../data")