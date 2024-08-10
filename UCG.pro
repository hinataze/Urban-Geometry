
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT       += core gui
QT       += opengl
QT       += svg
QT       += widgets


CONFIG += c++17

# Uncomment to make code fail to compile if it with deprecated APIs.
# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    hs_execution_log.cpp \
    hs_triangulation.cpp \
    interactive_view.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    include/HS_CTriangulationGraphicsItem.h \
    include/HS_Delaunay_triangulation_2.h \
    include/HS_TriangulationGraphicsItem.h \
    include/HS_Triangulation_2.h \
    include/hs_triangulation.h \ 
    include/hs_execution_log.h \
    include/interactive_view.h \
    include/mainwindow.h

FORMS += \
    C:/dev/CGAL-5.6.1/include/CGAL/Qt/ImageInterface.ui \
    mainwindow.ui

INCLUDEPATH += \
          C:/dev/CGAL-5.6.1/include

LIBS += -LC:/Users/hermenegildo/99_PROJECTS/URBAN_GEOMETRY/UCG -lCGAL_Qt5_moc_and_resources


ICON = C:/Users/hermenegildo/99_PROJECTS/URBAN_GEOMETRY/UCG/icon_windows.ico

RESOURCES = C:/Users/hermenegildo/99_PROJECTS/URBAN_GEOMETRY/UCG/icon.qrc

