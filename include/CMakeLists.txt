add_subdirectory(SDL)
add_subdirectory(glm)

add_library(include INTERFACE)

target_include_directories(include INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(include INTERFACE
    SDL3::SDL3
    glm::glm
)
