cmake_minimum_required(VERSION 3.20)
project(AetherBinary VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PROJECT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

configure_file(
  "${CMAKE_SOURCE_DIR}/include/Version.h.in"
  "${CMAKE_BINARY_DIR}/generated/Version.h"
  @ONLY
)

set(HEADER_FILES
  include/AetherArch.h
  include/AetherBinary.h
  include/AetherCommop.h
  include/AetherELF.h
  include/AetherFile.h
  include/AetherMachO.h
  include/AetherMeta.h
  include/AetherOther.h
  include/AetherPE.h
  include/AetherRegister.h
  include/CommDef.h
  include/Disassembler.h
  src/AetherBinaryPriv.hpp
)

set(SOURCE_FILES
  src/AetherArch.cpp
  src/AetherBinary.cpp
  src/AetherCommop.cpp
  src/AetherELF.cpp
  src/AetherMachO.cpp
  src/AetherMeta.cpp
  src/AetherOther.cpp
  src/AetherPE.cpp
  src/Assembler.cpp
  src/Disassembler.cpp
)

add_library(AetherBinary SHARED
  ${SOURCE_FILES}
  ${HEADER_FILES}
)

target_include_directories(AetherBinary PUBLIC
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/generated>
)

target_compile_features(AetherBinary PUBLIC cxx_std_20)

target_compile_definitions(AetherBinary PRIVATE
  PROJECT_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
  PROJECT_VERSION_MINOR=${PROJECT_VERSION_MINOR}
  PROJECT_VERSION_PATCH=${PROJECT_VERSION_PATCH}
  PROJECT_VERSION_STRING="${PROJECT_VERSION_STRING}"
)
