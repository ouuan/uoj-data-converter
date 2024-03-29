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

configure_file(cmake/version.hpp.in ${CMAKE_BINARY_DIR}/generated/version.hpp)

add_executable(uoj-data-converter
  ${GUI_TYPE}

  src/main.cpp
  
  src/MainWindow.hpp
  src/MainWindow.cpp

  src/Models/ConvertedTestCaseModel.hpp
  src/Models/ExampleModel.hpp
  src/Models/OriginalTestCaseModel.hpp
  src/Models/ProblemConfModel.hpp
  src/Models/ResultModel.hpp
  src/Models/StdModel.hpp
  src/Models/SubtaskModel.hpp

  src/Pages/CommitOperationPage.cpp
  src/Pages/CommitOperationPage.hpp
  src/Pages/ExamplePage.cpp
  src/Pages/ExamplePage.hpp
  src/Pages/FinishPage.cpp
  src/Pages/FinishPage.hpp
  src/Pages/ProblemConfPage.cpp
  src/Pages/ProblemConfPage.hpp
  src/Pages/StdPage.cpp
  src/Pages/StdPage.hpp
  src/Pages/SubtaskPage.cpp
  src/Pages/SubtaskPage.hpp
  src/Pages/TestCaseChoosePage.cpp
  src/Pages/TestCaseChoosePage.hpp
  src/Pages/TestCaseConvertPage.cpp
  src/Pages/TestCaseConvertPage.hpp

  src/Widgets/AcceptsDropEdit.cpp
  src/Widgets/AcceptsDropEdit.hpp
  src/Widgets/ErrorLabel.cpp
  src/Widgets/ErrorLabel.hpp
  src/Widgets/ListWidget.cpp
  src/Widgets/ListWidget.hpp

  resources/resources.qrc
)

include_directories("generated/")
include_directories("src/")

if(MSVC)
    target_compile_options(uoj-data-converter PRIVATE "/utf-8")
endif(MSVC)

if(CMAKE_COMPILER_IS_GNUCXX)
  target_compile_options(uoj-data-converter PRIVATE
    -pedantic
    -Wall
    -Wextra
    -Wshadow
    -Woverloaded-virtual
    -Winit-self
    -Wunreachable-code
  )
endif(CMAKE_COMPILER_IS_GNUCXX)

target_link_libraries(uoj-data-converter PRIVATE Qt5::Widgets)

if(APPLE)
  set_target_properties(uoj-data-converter
    PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/cmake/Info.plist.in
  )
endif()

if(UNIX AND NOT APPLE)
  install(TARGETS uoj-data-converter
          RUNTIME DESTINATION bin
          LIBRARY DESTINATION lib)
  install(FILES .ci/linux/uoj-data-converter.desktop DESTINATION share/applications)
  install(FILES .ci/linux/icon.png DESTINATION share/icons/hicolor/256x256/apps RENAME uoj-data-converter.png)
endif()
