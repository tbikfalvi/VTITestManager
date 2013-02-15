//============================================================================================================
//
//  VTI Test Manager application
//
//  File            : dlgtestinfraregistry.cpp
//  File version    : 1.0
//
//============================================================================================================

#include <QMessageBox>
#include <QCloseEvent>
#include <QDir>
#include <QFileDialog>
#include <QProcess>

//============================================================================================================

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dlgtestinfraregistry.h"
#include "dlgpreferences.h"
#include "tmpreferences.h"
#include "dlgconfigeditor.h"
#include "dlgmanagereleases.h"
#include "dlgstartat.h"
#include "dlgbuildpackage.h"
#include "dlgmanageautotests.h"
#include "dlgprocesscmd.h"

//#include <logger.h>
//tgLogger::instance() << cSeverity::DEBUG << QString("Process %1").arg(i) << cLogMessage::EOM;

//============================================================================================================
// MainWindow constructor
//------------------------------------------------------------------------------------------------------------
// Initializing user interface pointer
//============================================================================================================
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    tgPrefs::instance().load();

    setWindowTitle( QString("VTI Test Manager - Version: %1").arg(tgPrefs::instance().qsAppVersion()) );

    while( tgPrefs::instance().actionNeeded() )
    {
        if( tgPrefs::instance().actionNeeded( cTMPreferences::ACT_RUN_ENV ) )
        {
            if( QMessageBox::question( this, tr("Question"),
                                       tr("Is this application is running in cell or minisystem environment?"),
                                       QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
            {
                tgPrefs::instance().setRunningOnDevelopmentPC( false );
            }
            else
            {
                tgPrefs::instance().setRunningOnDevelopmentPC( true );
            }
            tgPrefs::instance().actionProcessed( cTMPreferences::ACT_RUN_ENV );
        }
    }

    ui->action_BuildVTIPackage->setEnabled( tgPrefs::instance().isRunningOnDevelopmentPC() );
}
//============================================================================================================
// MainWindow destructor
//------------------------------------------------------------------------------------------------------------
// Destroying user interface pointer
//============================================================================================================
MainWindow::~MainWindow()
{
    delete ui;
}
//============================================================================================================
// closeEvent
//------------------------------------------------------------------------------------------------------------
// Event occures when main window of application is begin to close
//============================================================================================================
void MainWindow::closeEvent( QCloseEvent *p_poEvent )
{
    if( QMessageBox::question( this, tr("Question"),
                               tr("Are you sure you want to close the application?"),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        p_poEvent->accept();
    }
    else
    {
        p_poEvent->ignore();
    }
}
//============================================================================================================
// on_action_ExitApplication_triggered
//------------------------------------------------------------------------------------------------------------
// Called when user activate 'Exit application'
//============================================================================================================
void MainWindow::on_action_ExitApplication_triggered()
{
    close();
}
//============================================================================================================
// on_action_Preferences_triggered
//------------------------------------------------------------------------------------------------------------
// Called when user activate 'Preferences' dialog
//============================================================================================================
void MainWindow::on_action_Preferences_triggered()
{
    dlgPreferences  obDlgPreferences( this );

    obDlgPreferences.exec();

    ui->action_BuildVTIPackage->setEnabled( tgPrefs::instance().isRunningOnDevelopmentPC() );
}
//============================================================================================================
// on_action_Manage_registry_triggered
//------------------------------------------------------------------------------------------------------------
// Called when user activate 'Manage registry' dialog
//============================================================================================================
void MainWindow::on_action_Manage_registry_triggered()
{
    dlgTestinfraRegistry    obDlgTestinfraRegistry( this );

    obDlgTestinfraRegistry.exec();
}
//============================================================================================================
// on_action_AIF_TestConfiguration_triggered
//------------------------------------------------------------------------------------------------------------
// Called when user activate 'AIF Test Configuration' dialog
//============================================================================================================
void MainWindow::on_action_AIF_TestConfiguration_triggered()
{
    dlgConfigEditor obDlgConfigEditor( this, "AIF", tgPrefs::instance().dirAIF(), tgPrefs::instance().dirAIFConf() );

    obDlgConfigEditor.exec();
}
//============================================================================================================
// on_action_Cycler_TestConfiguration_triggered
//------------------------------------------------------------------------------------------------------------
// Called when user activate 'Cycler Test Configuration' dialog
//============================================================================================================
void MainWindow::on_action_Cycler_TestConfiguration_triggered()
{
    QString qsCyclerConfDir = tgPrefs::instance().dirCyclerConf();

    if( tgPrefs::instance().dirCyclerConf().contains("/<CYCLER_VERSION>/") )
    {
        QString qsDirectory = QFileDialog::getExistingDirectory(this, tr("Select the desired Cycler release"), tgPrefs::instance().dirCycler() );

        if (!qsDirectory.isEmpty())
        {
            qsCyclerConfDir = qsDirectory + QString("/test/engine");
        }
    }

    dlgConfigEditor obDlgConfigEditor( this, "Cycler", tgPrefs::instance().dirCycler(), qsCyclerConfDir );

    obDlgConfigEditor.exec();
}
//============================================================================================================
// on_action_ManageReleases_triggered
//------------------------------------------------------------------------------------------------------------
// Called when user activate 'Manage Releases' dialog
//============================================================================================================
void MainWindow::on_action_ManageReleases_triggered()
{
    dlgManageReleases   obDlgManageReleases( this );

    obDlgManageReleases.exec();
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void MainWindow::on_action_BuildVTIPackage_triggered()
{
    dlgBuildPackage obDlgBuildPackage( this );

    obDlgBuildPackage.exec();
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void MainWindow::on_action_ManageATP_AIF_triggered()
{
    dlgManageAutoTests   obDlgManageAutoTests( this, "AIF", tgPrefs::instance().dirAIF(), tgPrefs::instance().dirAIFConf() );

    obDlgManageAutoTests.exec();
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void MainWindow::on_action_ManageATP_Cycler_triggered()
{
    QString qsCyclerConfDir = tgPrefs::instance().dirCyclerConf();

    if( tgPrefs::instance().dirCyclerConf().contains("/<CYCLER_VERSION>/") )
    {
        QString qsDirectory = QFileDialog::getExistingDirectory(this, tr("Select the desired Cycler release"), tgPrefs::instance().dirCycler() );

        if (!qsDirectory.isEmpty())
        {
            qsCyclerConfDir = qsDirectory + QString("/test/engine");
        }
    }

    dlgManageAutoTests   obDlgManageAutoTests( this, "Cycler", tgPrefs::instance().dirCycler(), qsCyclerConfDir );

    obDlgManageAutoTests.exec();
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void MainWindow::on_action_Process_Commands_triggered()
{
    dlgProcessCmd   obDlgProcessCmd( this );

    obDlgProcessCmd.exec();
}
