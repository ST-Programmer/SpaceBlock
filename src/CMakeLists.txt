add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
        main.cpp
        game.cpp
        entity.cpp
        player.cpp
        tile.cpp
        plant.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        include
)
