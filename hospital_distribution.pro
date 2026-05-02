QT += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hospital_distribution
TEMPLATE = app

CONFIG += c++17

SOURCES += main.cpp mainwindow.cpp voronoi/voronoi_calculator.cpp view/voronoi_scene.cpp view/voronoi_view.cpp widget/hospital_input_widget.cpp widget/result_panel.cpp utils/file_importer.cpp

HEADERS += mainwindow.h model/hospital.h model/evaluation_result.h model/voronoi_cell.h voronoi/voronoi_calculator.h view/voronoi_scene.h view/voronoi_view.h widget/hospital_input_widget.h widget/result_panel.h utils/file_importer.h

FORMS += mainwindow.ui
