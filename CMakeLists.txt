cmake_minimum_required(VERSION 3.15)
project(POC LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Arquivos fonte
set(SRC
    src/main.c
    src/game.c
    src/player.c
    src/entity.c
    src/enemy.c
    src/battle.c
    src/dice.c
)

# Cria o executável
add_executable(${PROJECT_NAME} ${SRC})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/headers)

# ==========================
# Linux (usa pkg-config)
# ==========================
if(UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(ALLEGRO REQUIRED allegro-5 allegro_font-5 allegro_primitives-5)

    target_include_directories(${PROJECT_NAME} PRIVATE ${ALLEGRO_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${ALLEGRO_LIBRARIES})
endif()

# ==========================
# Windows (paths manuais)
# ==========================
if(WIN32)
    set(ALLEGRO_ROOT "C:/Allegro")

    target_include_directories(${PROJECT_NAME} PRIVATE ${ALLEGRO_ROOT}/include)
    target_link_directories(${PROJECT_NAME} PRIVATE ${ALLEGRO_ROOT}/lib)

    target_link_libraries(${PROJECT_NAME}
        allegro-5
        allegro_font-5
        allegro_primitives-5
    )
endif()
