#-------------------------------------------------
#
# Project created by QtCreator 2012-04-10T14:09:15
#
#-------------------------------------------------

QT          += core gui xml
TARGET       = VTIAppManager
TEMPLATE     = app
INCLUDEPATH += framework

SOURCES     += framework/filewriter.cpp \
               framework/guiwriter.cpp \
               framework/logger.cpp \
               framework/logmessage.cpp \
               framework/preferences.cpp \
               framework/tracer.cpp \
               main.cpp\
               mainwindow.cpp \
               dlgtestinfraregistry.cpp \
               dlgconfigeditor.cpp \
               dlgpreferences.cpp \
               tmpreferences.cpp \
               dlgmanagereleases.cpp \
               dlgstartat.cpp \
               dlgbuildpackage.cpp \
               dlgprogress.cpp \
               dlgmanageautotests.cpp \
               dlgprocesscmd.cpp \
               dlgwarningbox.cpp

HEADERS     += framework/filewriter.h \
               framework/guiwriter.h \
               framework/logger.h \
               framework/logmessage.h \
               framework/logwriter.h \
               framework/preferences.h \
               framework/severity.h \
               framework/sevexception.h \
               framework/singleton.h \
               framework/tracer.h \
               mainwindow.h \
               dlgtestinfraregistry.h \
               dlgconfigeditor.h \
               dlgpreferences.h \
               tmpreferences.h \
               dlgmanagereleases.h \
               dlgstartat.h \
               dlgbuildpackage.h \
               dlgprogress.h \
               dlgmanageautotests.h \
               dlgprocesscmd.h \
               dlgwarningbox.h

FORMS       += mainwindow.ui \
               dlgtestinfraregistry.ui \
               dlgconfigeditor.ui \
               dlgpreferences.ui \
               dlgmanagereleases.ui \
               dlgstartat.ui \
               dlgbuildpackage.ui \
               dlgprogress.ui \
               dlgmanageautotests.ui \
               dlgprocesscmd.ui \
    dlgwarningbox.ui

RESOURCES   += vtitestmanager.qrc
RC_FILE      = vtitestmanager.rc



























