cmake_minimum_required(VERSION 3.5)

add_executable ( snake_game main.cpp )

target_include_directories( snake_game PUBLIC ${PROJECT_SOURCE_DIR}/include )

target_link_libraries(snake_game PUBLIC ncurses )