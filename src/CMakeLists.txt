set(SNAKE_SRCS main.c show_help.c show_version.c run_game.c)
add_executable(snake ${SNAKE_SRCS})
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")