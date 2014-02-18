#-------------------------------------------------
#
# Project created by QtCreator 2014-01-26T09:54:31
#
#-------------------------------------------------

QT       += core gui webkit webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -Wno-deprecated-writable-strings -DBOOST_ALL_DYN_LINK

unix {
  DEFINES += HAVE_STDINT_H
}

macx {
  QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9
  CONFIG -= app_bundle
}

TARGET = LifeMotif
TEMPLATE = app

SOURCES += main.cpp\
    python_wrapper/basepythonwrapper.cpp \
    python_wrapper/googleoauth2wrapper.cpp \
    python_wrapper/googleimapwrapper.cpp \
    python_wrapper/imapwrapper.cpp \
    python_wrapper/oauth2wrapper.cpp \
    python_wrapper/localstructurewrapper.cpp \
    lifemotif_exceptions.cpp \
    localstructure_extract.cpp \
    message_types.cpp \
    email_cache.cpp \
    lifemotif_settings.cpp \
    lifemotif_utils.cpp \
    web_browser_dialog.cpp \
    lifemotif_diary.cpp \
    mime_raw_message_dialog.cpp \
    mime_structure_dialog.cpp \
    main_window.cpp \
    preference_window.cpp \
    lifemotif_calendar_widget.cpp


HEADERS  += \
    lifemotif_config.h \
    python_wrapper/basepythonwrapper.h \
    python_wrapper/googleoauth2wrapper.h \
    python_wrapper/googleimapwrapper.h \
    python_wrapper/imapwrapper.h \
    python_wrapper/oauth2wrapper.h \
    python_wrapper/localstructurewrapper.h \
    python_wrapper/python_wrapper.h \
    lifemotif_exceptions.h \
    lifemotif_settings.h \
    localstructure_extract.h \
    message_types.h \
    email_cache.h \
    lifemotif_utils.h \
    web_browser_dialog.h \
    lifemotif_diary.h \
    mime_raw_message_dialog.h \
    mime_structure_dialog.h \
    preference_window.h \
    main_window.h \
    lifemotif_calendar_widget.h

FORMS    += \
    web_browser_dialog.ui \
    mime_raw_message_dialog.ui \
    mime_structure_dialog.ui \
    main_window.ui \
    preference_window.ui

OTHER_FILES += \
    ../python/base_imap_control.py \
    ../python/base_oauth2_control.py \
    ../python/email_parser.py \
    ../python/google_imap_control.py \
    ../python/google_oauth2_control.py \
    ../python/imapUTF7.py \
    ../python/local_structure_control.py \
    TODO.txt \
    resource/lifemotif.ico

unix:!macx {
INCLUDEPATH += \
    /usr/include \
    /usr/include/python2.7 \
    /usr/local/include

LIBS += \
    -L/usr/lib \
    -L/usr/local/lib \
    -lboost_python-2.7-mt
}

macx {
INCLUDEPATH += \
    /opt/local/Library/Frameworks/Python.framework/Versions/2.7/include/python2.7 \
    /opt/local/include

LIBS += \
    -L/opt/local/lib \
    -lboost_system-mt -lboost_filesystem-mt -lboost_python-mt  \
    -L/opt/local/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/config \
    -ldl -framework CoreFoundation
}

LIBS += \
    -lmimetic \
    -lpython2.7
