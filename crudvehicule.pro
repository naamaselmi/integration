QT       += core gui sql charts widgets svg network  # Add svg module for QSvgRenderer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# Your project source files
SOURCES += \
    connection.cpp \
    main.cpp \
    mainwindow.cpp \
    technicien.cpp \
    vehicule.cpp \
    libs/qrcodegen/qrcodegen.cpp \
    libs/QrCodeGenerator.cpp  # Ensure qrcodegen.cpp is included here

HEADERS += \
    connection.h \
    mainwindow.h \
    technicien.h \
    vehicule.h \
    libs/QrCodeGenerator.h

INCLUDEPATH += C:/Users/zussl/Documents/crudvehicule/libs/qrcodegen



FORMS += \
    mainwindow.ui

RESOURCES += \
    image.qrc
