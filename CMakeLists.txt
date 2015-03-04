cmake_minimum_required(VERSION 2.8)

# 如果用戶沒有設置C/C++編譯器且當前用戶環境中找到了clang和clang++
# 則使用clang和clang++做爲默認的編譯器
if (NOT CMAKE_C_COMPILER AND NOT CMAKE_CXX_COMPILER)
  find_program(CLANG "clang")
  find_program(CLANG_CXX "clang++")
  if (CLANG AND CLANG_CXX)
    set(CMAKE_C_COMPILER ${CLANG})
    set(CMAKE_CXX_COMPILER ${CLANG_CXX})
  endif(CLANG AND CLANG_CXX)
endif (NOT CMAKE_C_COMPILER AND NOT CMAKE_CXX_COMPILER)

project(SNAKE)

set(SNAKE_MAJOR_VERSION 0)
set(SNAKE_MINOR_VERSION 1)
set(SNAKE_PATCH_VERSION 1)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING
    "Choose the type of build, options are: Non Debug Release RelWithDebInfo
MinSizeRel."
    FORCE)
endif ()

set(CMAKE_C_FLAGS_DEBUG "$ENV{CFLAGS} -Wall -g -ggdb")
set(CMAKE_C_FLAGS_RELEASE "$ENV{CFLAGS} -O2 -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2 -Wall")


set(PROJECT_NAME snake)

include(CheckFunctionExists)
check_function_exists(getopt_long HAVE_GETOPT_LONG)

# 檢查是否存在curses或是ncurses庫
include(FindCurses)
if (NOT CURSES_FOUND)
  message(FATAL_ERROR "need curses or ncurses")
endif (NOT CURSES_FOUND)

configure_file(config.h.in "${PROJECT_BINARY_DIR}/include/config.h")
include_directories("${PROJECT_BINARY_DIR}/include")

add_subdirectory(src)
