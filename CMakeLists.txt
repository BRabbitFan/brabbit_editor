cmake_minimum_required(VERSION 3.14)

project(brabbit_editor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(
  QT
  NAMES
  Qt6
  REQUIRED
  OMPONENTS
  Core
  Quick
  LinguistTools
)
find_package(
  Qt${QT_VERSION_MAJOR}
  REQUIRED
  COMPONENTS
  Core
  Quick
  LinguistTools
)

set(PROJECT_SOURCES
  main.cpp
  translate.hpp
  LanguageManager.hpp LanguageManager.cpp
  UiKernal.hpp UiKernal.cpp
  DockListModel.hpp DockListModel.cpp
  qml.qrc
  ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(brabbit_editor
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
  )
# Define target properties for Android with Qt 6 as:
#   set_property(TARGET brabbit_editor APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
  if(ANDROID)
    add_library(brabbit_editor SHARED
      ${PROJECT_SOURCES}
    )
# Define properties for Android with Qt 5 after find_package() calls as:
#   set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
  else()
    add_executable(brabbit_editor
      ${PROJECT_SOURCES}
    )
  endif()
endif()

message(STATUS "translate started")
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/json/)
execute_process(COMMAND python ${CMAKE_SOURCE_DIR}/translate.py -i true -ip ${CMAKE_SOURCE_DIR}/ini/ -q true -c true)
message(STATUS ${EXECUTABLE_OUTPUT_PATH})
message(STATUS "translate finished")


target_link_libraries(brabbit_editor
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick)

set_target_properties(brabbit_editor PROPERTIES
  MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
  MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
  MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
  MACOSX_BUNDLE TRUE
  WIN32_EXECUTABLE TRUE
)

install(TARGETS brabbit_editor
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
  qt_import_qml_plugins(brabbit_editor)
  qt_finalize_executable(brabbit_editor)
endif()
