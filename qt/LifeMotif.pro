#-------------------------------------------------
#
# Project created by QtCreator 2014-01-26T09:54:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LifeMotif
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    python_wrapper/basepythonwrapper.cpp \
    python_wrapper/googleoauth2wrapper.cpp


HEADERS  += mainwindow.h \
    python_wrapper/basepythonwrapper.h \
    python_wrapper/googleoauth2wrapper.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    ../python/base_imap_control.py \
    ../python/base_oauth2_control.py \
    ../python/google_imap_control.py \
    ../python/google_oauth2_control.py \
    ../python/imapUTF7.py

INCLUDEPATH += \
    /opt/local/Library/Frameworks/Python.framework/Versions/2.7/include/python2.7 \
    /opt/local/include

LIBS += \
    -L/opt/local/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/config \
    -L /opt/local/lib -lboost_python-mt \
    -ldl -framework CoreFoundation -lpython2.7
