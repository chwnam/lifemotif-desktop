#-------------------------------------------------
#
# Project created by QtCreator 2014-01-26T09:54:31
#
#-------------------------------------------------

QT       += core gui webkit webkitwidgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -Wno-deprecated-writable-strings -DBOOST_ALL_DYN_LINK

unix {
  DEFINES += HAVE_STDINT_H
}

macx {
  QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9
  CONFIG -= app_bundle
}

TARGET = lifemotif-desktop
TEMPLATE = app

SOURCES += \
    #python_wrapper/basepythonwrapper.cpp \
    #python_wrapper/googleoauth2wrapper.cpp \
    #python_wrapper/googleimapwrapper.cpp \
    #python_wrapper/imapwrapper.cpp \
    #python_wrapper/oauth2wrapper.cpp \
    #python_wrapper/localstructurewrapper.cpp \
    email_cache.cpp \
    loading_dialog.cpp \
    localstructure_extract.cpp \
    mailbox_list_extract.cpp \
    main.cpp \
    main_window.cpp \
    message_types.cpp \
    mime_raw_message_dialog.cpp \
    mime_structure_dialog.cpp \
    preference_window.cpp \
    web_browser_dialog.cpp \
    imap_tag.cpp \
    tag_reset_dialog.cpp \
    imap_console_dialog.cpp \
    config.cpp \
    settings.cpp \
    attachment.cpp \
    diary.cpp \
    calendar_widget.cpp \
    exceptions.cpp \
    google_client_info.cpp \
    google_oauth2.cpp \
    google_oauth2_credential.cpp \
    imap_manager.cpp \
    utils.cpp


HEADERS  += \
    #python_wrapper/basepythonwrapper.h \
    #python_wrapper/googleoauth2wrapper.h \
    #python_wrapper/googleimapwrapper.h \
    #python_wrapper/imapwrapper.h \
    #python_wrapper/oauth2wrapper.h \
    #python_wrapper/localstructurewrapper.h \
    #python_wrapper/python_wrapper.h \
    email_cache.h \
    localstructure_extract.h \
    main_window.h \
    message_types.h \
    mime_raw_message_dialog.h \
    mime_structure_dialog.h \
    preference_window.h \
    web_browser_dialog.h \
    loading_dialog.h \
    mailbox_list_extract.h \
    imap_tag.h \
    tag_reset_dialog.h \
    imap_console_dialog.h \
    config.h \
    settings.h \
    diary.h \
    attachment.h \
    calendar_widget.h \
    exceptions.h \
    google_client_info.h \
    google_oauth2.h \
    google_oauth2_credential.h \
    imap_manager.h \
    utils.h

FORMS    += \
    loading_dialog.ui \
    main_window.ui \
    mime_raw_message_dialog.ui \
    mime_structure_dialog.ui \
    preference_window.ui \
    web_browser_dialog.ui \
    tag_reset_dialog.ui \
    imap_console_dialog.ui

OTHER_FILES += \
    ../../python/base_imap_control.py \
    ../../python/base_oauth2_control.py \
    ../../python/email_parser.py \
    ../../python/google_imap_control.py \
    ../../python/google_oauth2_control.py \
    ../../python/imapUTF7.py \
    ../../python/local_structure_control.py \
    resource/lifemotif.ico \
    NOTE \
    TODO

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
    #-lboost_system-mt -lboost_filesystem-mt -lboost_python-mt  \
    -L/opt/local/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/config \
    -ldl -framework CoreFoundation
}

LIBS += \
    -lmimetic \
    -lpython2.7

RESOURCES += \
    lifemotif-desktop.qrc
