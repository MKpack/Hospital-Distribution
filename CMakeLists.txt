cmake_minimum_required(VERSION 3.16)

project(hospital_distribution VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Gui)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        model/hospital.h
        model/evaluation_result.h
        model/voronoi_cell.h
        voronoi/voronoi_calculator.h
        voronoi/voronoi_calculator.cpp
        view/voronoi_scene.h
        view/voronoi_scene.cpp
        view/voronoi_view.h
        view/voronoi_view.cpp
        widget/hospital_input_widget.h
        widget/hospital_input_widget.cpp
        widget/result_panel.h
        widget/result_panel.cpp
        utils/file_importer.h
        utils/file_importer.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(hospital_distribution
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(hospital_distribution SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(hospital_distribution
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(hospital_distribution PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Gui)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.hospital_distribution)
endif()
set_target_properties(hospital_distribution PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS hospital_distribution
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(hospital_distribution)
endif()
