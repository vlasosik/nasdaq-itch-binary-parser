add_subdirectory(entities)
add_subdirectory(exceptions)
add_subdirectory(parser)
add_subdirectory(reader)

add_executable(exe main.cpp)

target_link_libraries(exe PRIVATE entites parcer reader quill::quill fmt::fmt ZLIB::ZLIB)