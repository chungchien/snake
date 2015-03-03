set(SNAKE_SRCS
  main.c
  show_help.c
  show_version.c
  run_game.c
  snake.c
  food.c
  wall.c
  )

add_executable(snake ${SNAKE_SRCS})
target_link_libraries(snake ${CURSES_LIBRARIES})
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")