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
    python_wrapper/basepythonwrapper.cpp \
    python_wrapper/googleoauth2wrapper.cpp \
    python_wrapper/googleimapwrapper.cpp \
    python_wrapper/imapwrapper.cpp \
    python_wrapper/oauth2wrapper.cpp \
    python_wrapper/localstructurewrapper.cpp \
    email_cache.cpp \
    lifemotif_calendar_widget.cpp \
    lifemotif_diary.cpp \
    lifemotif_exceptions.cpp \
    lifemotif_google_client_info.cpp \
    lifemotif_google_oauth2.cpp \
    lifemotif_google_oauth2_credential.cpp \
    lifemotif_settings.cpp \
    lifemotif_utils.cpp \
    loading_dialog.cpp \
    localstructure_extract.cpp \
    mailbox_list_extract.cpp \
    main.cpp \
    main_window.cpp \
    message_types.cpp \
    mime_raw_message_dialog.cpp \
    mime_structure_dialog.cpp \
    preference_window.cpp \
    runnable_helper.cpp \
    web_browser_dialog.cpp \
    imap_tag.cpp \
    tag_reset_dialog.cpp \
    base_imap.cpp \
    imap_console_dialog.cpp


HEADERS  += \
    python_wrapper/basepythonwrapper.h \
    python_wrapper/googleoauth2wrapper.h \
    python_wrapper/googleimapwrapper.h \
    python_wrapper/imapwrapper.h \
    python_wrapper/oauth2wrapper.h \
    python_wrapper/localstructurewrapper.h \
    python_wrapper/python_wrapper.h \
    email_cache.h \
    lifemotif_calendar_widget.h \
    lifemotif_config.h \
    lifemotif_diary.h \
    lifemotif_exceptions.h \
    lifemotif_google_client_info.h \
    lifemotif_google_oauth2.h \
    lifemotif_google_oauth2_credential.h \
    lifemotif_settings.h \
    lifemotif_utils.h \
    localstructure_extract.h \
    main_window.h \
    message_types.h \
    mime_raw_message_dialog.h \
    mime_structure_dialog.h \
    preference_window.h \
    web_browser_dialog.h \
    loading_dialog.h \
    runnable_helper.h \
    mailbox_list_extract.h \
    imap_tag.h \
    tag_reset_dialog.h \
    base_imap.h \
    imap_console_dialog.h

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
    ../python/base_imap_control.py \
    ../python/base_oauth2_control.py \
    ../python/email_parser.py \
    ../python/google_imap_control.py \
    ../python/google_oauth2_control.py \
    ../python/imapUTF7.py \
    ../python/local_structure_control.py \
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
