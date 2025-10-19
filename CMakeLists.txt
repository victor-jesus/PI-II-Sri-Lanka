cmake_minimum_required(VERSION 3.15)
project(Game LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(SRC
    src/main.c
    src/game.c
    src/player.c
    src/entity.c
    src/enemy.c
    src/battle.c
    src/dice.c
    src/sprite.c
    src/map.c
    src/door.c
    src/physics.c
)

include_directories(
    headers
)

add_executable(game ${SRC})

if(WIN32)
    set(ALLEGRO_INCLUDE_DIR "C:/Allegro/include")
    set(ALLEGRO_LIB_DIR "C:/Allegro/lib")

    include_directories(${ALLEGRO_INCLUDE_DIR})
    link_directories(${ALLEGRO_LIB_DIR})

    target_link_libraries(game 
        "C:/Allegro/lib/liballegro.dll.a"
        "C:/Allegro/lib/liballegro_primitives.dll.a"
        "C:/Allegro/lib/liballegro_font.dll.a"
        "C:/Allegro/lib/liballegro_ttf.dll.a"
        "C:/Allegro/lib/liballegro_image.dll.a"
        "C:/Allegro/lib/liballegro_main.dll.a"
    )

else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(ALLEGRO REQUIRED allegro-5 allegro_primitives-5 allegro_font-5 allegro_ttf-5 allegro_image-5 allegro_main-5)

    include_directories(${ALLEGRO_INCLUDE_DIRS})
    link_directories(${ALLEGRO_LIBRARY_DIRS})
    target_link_libraries(game ${ALLEGRO_LIBRARIES})
endif()

if(MSVC)
    target_compile_options(game PRIVATE /W4 /WX)
else()
    target_compile_options(game PRIVATE -Wall -Wextra -pedantic -g)
endif()

add_custom_target(run
    COMMAND $<TARGET_FILE:game>
    DEPENDS game
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} 
)
