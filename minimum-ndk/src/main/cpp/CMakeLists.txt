cmake_minimum_required(VERSION 3.4.1)

# build native_app_glue as a static lib
set(${CMAKE_C_FLAGS}, "${CMAKE_C_FLAGS}")

add_library(
  native_app_glue
  STATIC
  ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
)

# Export ANativeActivity_onCreate(),
# Refer to: https://github.com/android-ndk/ndk/issues/381.
set(
  CMAKE_SHARED_LINKER_FLAGS
  "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate"
)

add_library(
  MinimumNDK
  SHARED
  main.cpp
  Core.cpp
  Context.cpp
)

target_include_directories(
  MinimumNDK
  PRIVATE
  ${ANDROID_NDK}/sources/android/native_app_glue
)

# add lib dependencies
target_link_libraries(
  MinimumNDK
  PUBLIC
  android
  native_app_glue
  EGL
  GLESv1_CM
  log
)

target_compile_features(
  MinimumNDK
  PUBLIC
  cxx_constexpr
  cxx_auto_type
)

target_compile_options(
  MinimumNDK
  PUBLIC
  -Wall
  -Werror
)
