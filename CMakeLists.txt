cmake_minimum_required(VERSION 3.5)

project(uoj-data-converter LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
  SET(GUI_TYPE WIN32)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

# https://medium.com/@alasher/colored-c-compiler-output-with-ninja-clang-gcc-10bfe7f2b949
option(FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." On)
if(${FORCE_COLORED_OUTPUT})
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(-fdiagnostics-color=always)
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options(-fcolor-diagnostics)
  endif()
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/.git)
  find_package(Git)
  if(GIT_FOUND)
    execute_process(
      COMMAND ${GIT_EXECUTABLE} describe --long --tags
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
      OUTPUT_VARIABLE "GIT_DESCRIBE"
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    if(NOT "${GIT_DESCRIBE}" EQUAL "")
      string(REGEX REPLACE "(.+)-(.+)-(.+)" "\\2" "COMMITS_SINCE_LAST_RELEASE" "${GIT_DESCRIBE}")
      if(NOT "${COMMITS_SINCE_LAST_RELEASE}" EQUAL "0")
        string(REGEX REPLACE "(.+)-(.+)-(.+)" "\\1.r\\2.\\3" "DISPLAY_VERSION" "${GIT_DESCRIBE}")
      else()
        string(REGEX REPLACE "(.+)-(.+)-(.+)" "\\1" "DISPLAY_VERSION" "${GIT_DESCRIBE}")
      endif()
    endif()
  endif()
endif()
message(STATUS "Display version: ${DISPLAY_VERSION}")

if(APPLE)
	set(CMAKE_OSX_DEPLOYMENT_TARGET "10.14" CACHE STRING "Minimum OS X deployment version")
endif()

configure_file(src/version.hpp.in ${CMAKE_BINARY_DIR}/generated/version.hpp)

add_executable(uoj-data-converter
  src/main.cpp
)

include_directories("generated/")

if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(uoj-data-converter PRIVATE
		-ansi
		-pedantic
		-Wall
		-Wextra
		-Weffc++
		-Woverloaded-virtual
		-Winit-self
		-Wunreachable-code
	)
endif(CMAKE_COMPILER_IS_GNUCXX)

target_link_libraries(uoj-data-converter PRIVATE Qt5::Widgets)