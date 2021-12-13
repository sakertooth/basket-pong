cmake_minimum_required(VERSION 3.0.0)
project(basketpong VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(basketpong   src/Main.c 
                            src/Game.c
                            src/Audio.c
                            src/gui/Fonts.c
                            src/gui/Button.c 
                            src/gui/Label.c
                            src/objects/Player.c
                            src/objects/Ball.c
                            src/objects/Net.c
                            src/states/MainMenuState.c 
                            src/states/PlayState.c
                            src/states/GameOverState.c)

target_compile_options(basketpong PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Werror>
)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
find_package(SDL2 REQUIRED)
find_package(SDL2TTF REQUIRED)
find_package(SDL2_mixer REQUIRED)

target_include_directories(basketpong PRIVATE src ${SDL2_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIR})
target_link_libraries(basketpong ${SDL2_LIBRARIES} ${SDL2TTF_LIBRARY} ${SDL2_MIXER_LIBRARY})

add_custom_command(TARGET basketpong POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:basketpong>/assets)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
