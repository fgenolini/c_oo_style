cmake_minimum_required(VERSION 3.16)

# set(CMAKE_VERBOSE_MAKEFILE ON)

set(APP_LOW_NAME c_oo_style)

project(${APP_LOW_NAME} VERSION 1.0.1)

include(CTest)
enable_testing()

# C standard required = C 11
# Not yet available for Microsoft Visual C 2019
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CommonSourcesList animal.c big_animal.c
  )

add_executable(${APP_LOW_NAME} ${APP_LOW_NAME}.c ${CommonSourcesList})
target_compile_options(${APP_LOW_NAME} PRIVATE
  $<$<OR:$<C_COMPILER_ID:Clang>,$<C_COMPILER_ID:AppleClang>,$<C_COMPILER_ID:GNU>>:-Wall -Wextra>
  $<$<C_COMPILER_ID:MSVC>:/Wall /w45038 /wd4201 /wd4365 /wd4626 /wd4710 /wd4711 /wd4820>)

add_test(${APP_LOW_NAME} ${APP_LOW_NAME})
