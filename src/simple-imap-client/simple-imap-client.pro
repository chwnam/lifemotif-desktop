QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = simple-imap-client
TEMPLATE = app

FORMS += \
    main_window.ui \
    connect_dialog.ui

HEADERS += \
    main_window.h \
    connect_dialog.h

SOURCES += \
    main_window.cpp \
    main.cpp \
    connect_dialog.cpp

