cmake_minimum_required(VERSION 3.5)

project(Project VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "C:/msys64/mingw64")

# find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network Core)
# find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network Core)

find_package(Qt6 REQUIRED COMPONENTS Core Network Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

add_executable(Project
  main.cpp mainwindow.cpp
  mainwindow.h mainwindow.ui
  client.h client.cpp
  task.h task.cpp
  taskManager.h
)

# target_link_libraries(Project PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(Project PRIVATE Qt6::Core Qt6::Network Qt6::Widgets)


include(GNUInstallDirs)
install(TARGETS Project
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
