#include <QtGui/QApplication>

#include <logger.h>
#include <guiwriter.h>
#include <filewriter.h>

#include <QStyle>
#include <QDesktopWidget>

#include "mainwindow.h"
#include "tmpreferences.h"

int main(int argc, char *argv[])
{
    int  inRetVal = 0;

    QApplication obApp(argc, argv);

    tgLogger::instance().registerWriter( new cGUIWriter( cSeverity::ERROR ) );
    tgLogger::instance().registerWriter( new cFileWriter( cSeverity::DEBUG,
                                                          "vtitestmanager.log",
                                                          cFileWriter::APPEND ) );

    try
    {
        MainWindow mainWindow;

        mainWindow.setGeometry( QStyle::alignedRect( Qt::LeftToRight,
                                                     Qt::AlignCenter,
                                                     mainWindow.size(),
                                                     obApp.desktop()->availableGeometry() ));
        tgPrefs::instance().setScreenSize( obApp.desktop()->availableGeometry() );
        mainWindow.show();

        inRetVal = obApp.exec();
    }
    catch( cSevException &e )
    {
        tgLogger::instance() << cSeverity::ERROR
                             << "WHOA... Just caught an unhandled exception!"
                             << cLogMessage::EOM;
        tgLogger::instance() << e;

        inRetVal = 1;
    }

    return inRetVal;
}
