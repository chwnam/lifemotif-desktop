#-------------------------------------------------
#
# Project created by QtCreator 2014-01-26T09:54:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -Wno-deprecated-writable-strings -DBOOST_ALL_DYN_LINK

TARGET = LifeMotif
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    python_wrapper/basepythonwrapper.cpp \
    python_wrapper/googleoauth2wrapper.cpp \
    python_wrapper/googleimapwrapper.cpp \
    python_wrapper/imapwrapper.cpp \
    python_wrapper/oauth2wrapper.cpp \
    python_wrapper/localstructurewrapper.cpp \
    preferencewindow.cpp \
    lifemotif_exceptions.cpp \
    lifemotif_settings.cpp \
    localstructure_extract.cpp \
    lifemotif_path_helper.cpp \
    message_types.cpp \
    email_cache.cpp


HEADERS  += mainwindow.h \
    lifemotif_config.h \
    python_wrapper/basepythonwrapper.h \
    python_wrapper/googleoauth2wrapper.h \
    python_wrapper/googleimapwrapper.h \
    python_wrapper/imapwrapper.h \
    python_wrapper/oauth2wrapper.h \
    python_wrapper/localstructurewrapper.h \
    preferencewindow.h \
    python_wrapper/python_wrapper.h \
    lifemotif_exceptions.h \
    lifemotif_settings.h \
    localstructure_extract.h \
    lifemotif_path_helper.h \
    message_types.h \
    email_cache.h

FORMS    += mainwindow.ui \
    preferencewindow.ui

OTHER_FILES += \
    ../python/base_imap_control.py \
    ../python/base_oauth2_control.py \
    ../python/google_imap_control.py \
    ../python/google_oauth2_control.py \
    ../python/imapUTF7.py \
    ../python/local_structure_control.py \
    TODO.txt

INCLUDEPATH += \
    /opt/local/Library/Frameworks/Python.framework/Versions/2.7/include/python2.7 \
    /opt/local/include

LIBS += \
    -L/opt/local/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/config \
    -L/opt/local/lib \
    -lboost_system-mt \
    -lboost_filesystem-mt \
    -lboost_python-mt  \
    -ldl -framework CoreFoundation -lpython2.7
