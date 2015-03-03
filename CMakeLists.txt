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

