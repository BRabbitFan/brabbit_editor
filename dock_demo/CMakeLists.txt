cmake_minimum_required(VERSION 3.16)

project(brabbit_dock_demo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 REQUIRED COMPONENTS
  Core
  Widgets
)

file(GLOB_RECURSE QT_UI_FILES
  src/*.ui
)

file(GLOB_RECURSE CPP_HEADER_FILES
  src/*.h
  src/*.hh
  src/*.hpp
)

file(GLOB_RECURSE CPP_SOURCE_FILES
  src/*.c
  src/*.cc
  src/*.cpp
)

foreach(QT_UI_FILE ${QT_UI_FILES})
  message(STATUS "[brabbit] Generating ${QT_UI_FILE} as ${QT_UI_FILE}.hpp with uic")
  execute_process(COMMAND
      uic ${QT_UI_FILE}
      --output ${QT_UI_FILE}.hpp
      --include brabbit/util/translate_tool.hpp
      --translate brabbit::translate
      --idbased
      --no-protection)
endforeach()

foreach(QT_UI_FILE ${QT_UI_FILES})
  message(STATUS "[brabbit] Generating ${QT_UI_FILE} as ${QT_UI_FILE}.cpp with moc")
  execute_process(COMMAND
      moc ${QT_UI_FILE}
      --output ${QT_UI_FILE}.hpp
      --include brabbit/util/translate_tool.hpp
      --translate brabbit::translate
      --idbased
      --no-protection)
endforeach()

qt5_wrap_cpp(QT_MOC_SOURCE_FILES ${CPP_HEADER_FILES})

add_executable(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC
  src
  build
)

target_sources(${PROJECT_NAME} PRIVATE
  ${CPP_HEADER_FILES}
  ${CPP_SOURCE_FILES}
  ${QT_UI_SOURCE_FILES}
  ${QT_MOC_SOURCE_FILES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  Qt5::Core
  Qt5::Widgets
)

set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER xyz.brabbit.${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
  WIN32_EXECUTABLE TRUE

  ${BUNDLE_ID_OPTION}
  MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
  MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
  MACOSX_BUNDLE TRUE
)

# include(GNUInstallDirs)

# install(TARGETS brabbit_dock_demo
#   BUNDLE DESTINATION .
#   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#   RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
# )
