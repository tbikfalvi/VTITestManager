//============================================================================================================
//
//  VTI Test Manager application
//
//  File            : dlgmanagereleases.cpp
//  File version    : 1.0
//
//============================================================================================================

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QStringList>
#include <QFile>
#include <QTime>
#include <QThread>

//============================================================================================================

#include "dlgmanagereleases.h"
#include "ui_dlgmanagereleases.h"
#include "tmpreferences.h"
#include <logger.h>

//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
class sleeper : public QThread
{
public:
    static void msleep(unsigned long msecs)
    {
        QThread::msleep(msecs);
    }
};
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
dlgManageReleases::dlgManageReleases(QWidget *parent) : QDialog(parent), ui(new Ui::dlgManageReleases)
{
    ui->setupUi(this);

    setWindowTitle( QString("Manage SAT Releases") );

    ui->pbAIFInstall->setEnabled( false );
    ui->pbCyclerInstall->setEnabled( false );
    ui->pbCSSInstall->setEnabled( false );

    m_qsLocalDirRelease = tgPrefs::instance().dirLocalReleases();

    ui->ledCurrentDir->setText( m_qsLocalDirRelease );
    ui->ledCurrentDir->setToolTip( m_qsLocalDirRelease );

    _loadRegistrySettings();

    ui->ledRegAIFVersion->setText( m_qsRegAIFVersion );
    ui->ledRegInnovaPrg->setText( m_qsRegInnovaPrg );
    ui->ledRegFPProgram->setText( m_qsRegFPPrg );
    ui->ledRegIPEng->setText( m_qsRegEngPC_IP );
    ui->ledRegIPAW->setText( m_qsRegAWHost );
    ui->ledFootSwitchType->setText( m_qsRegFootSwitchType );
    ui->ledITUVersion->setText( m_qsRegITUVersion );

    _fillListAIFReleases();
    _fillListCyclerReleases();
    _fillListCSSReleases();

    QSettings   iniFile( "VTIAppManager.ini", QSettings::IniFormat );
    int         dlgWidth    = iniFile.value( "Dialogs/ManageReleases_width", 702 ).toInt();
    int         dlgHeight   = iniFile.value( "Dialogs/ManageReleases_height", 634 ).toInt();
    QPoint      qpDlgSize   = QPoint( dlgWidth, dlgHeight );

    resize( qpDlgSize.x(), qpDlgSize.y() );
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
dlgManageReleases::~dlgManageReleases()
{
    QSettings   iniFile( "VTIAppManager.ini", QSettings::IniFormat );

    iniFile.setValue( "Dialogs/ManageReleases_width", width() );
    iniFile.setValue( "Dialogs/ManageReleases_height", height() );
    delete ui;
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgManageReleases::on_pbAIFChangeDir_clicked()
{
    QString qsDirectory = QFileDialog::getExistingDirectory(this, tr("Select the desired directory"), m_qsLocalDirRelease );

    if (!qsDirectory.isEmpty())
    {
        m_qsLocalDirRelease = qsDirectory;
        _fillListAIFReleases();
        _fillListCyclerReleases();
        _fillListCSSReleases();

        ui->ledCurrentDir->setText( m_qsLocalDirRelease );
        ui->ledCurrentDir->setToolTip( m_qsLocalDirRelease );
    }
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgManageReleases::on_pbAIFInstall_clicked()
{
    if( ui->listAIFReleases->currentRow() < 0 )
    {
        QMessageBox::warning( this, tr("Warning"), tr("No valid item selected!") );
        ui->pbAIFInstall->setEnabled( false );
        return;
    }

    m_qsAIFFileRelease = ui->listAIFReleases->item(ui->listAIFReleases->currentRow())->text();

    QString qsRelease = QString( "%1/%2" ).arg(m_qsLocalDirRelease).arg(m_qsAIFFileRelease);
    qsRelease.replace( "/", "\\");

    if( QMessageBox::question( this, tr("Question"),
                               tr("Are you sure you want to install the following release:\n\n%1").arg(qsRelease),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        QProcess *qpAIFInstall = new QProcess(this);

        if( !qpAIFInstall->startDetached( QString("msiexec.exe /i \"%1\"").arg(qsRelease) ) )
        {
            QMessageBox::warning( this, tr("Warning"),
                                  tr("Error occured when starting process:\n\nmsiexec.exe /i %1\n\nError code: %2\n"
                                     "0 > The process failed to start.\n"
                                     "1 > The process crashed some time after starting successfully.\n"
                                     "2 > The last waitFor...() function timed out.\n"
                                     "4 > An error occurred when attempting to write to the process.\n"
                                     "3 > An error occurred when attempting to read from the process.\n"
                                     "5 > An unknown error occurred.").arg(qsRelease).arg(qpAIFInstall->error()) );
        }
        else
        {
            if( QMessageBox::question( this, tr("Question"),
                                       tr("Do you want to restore important registry settings after install?\n\n"
                                          "Please click Yes only after AIF installation finished."),
                                       QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
            {
                _loadBackupSettings();
                _restoreRegistrySettings();
            }
        }
    }
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgManageReleases::on_pbAIFUninstall_clicked()
{
    if( QMessageBox::question( this, tr("Question"),
                               tr("Do you want to save important registry settings before uninstall?"),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        _loadRegistrySettings();
        _backupRegistrySettings();
    }

    QString     qsProcess   = "MsiExec.exe /passive /x {434699A4-2838-4A36-91A8-21AD8DC5908A}";
/*
    QSettings   obReg( QString("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"), QSettings::NativeFormat );
    QStringList qslFolders  = obReg.childGroups();

    for( int i=0; i<qslFolders.size(); i++ )
    {
        QSettings   obRegTemp( QString("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\%1").arg(qslFolders.at(i)), QSettings::NativeFormat );
        QString     qsValue = obRegTemp.value( QString("DisplayName"), "" ).toString();

        if( qsValue.compare( "AIF" ) == 0 )
        {
            qsProcess = obRegTemp.value( QString("UninstallString"), "" ).toString();
            break;
        }
    }

    if( qsProcess.length() < 1 )
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr( "Unable to locate the uninstall application for AIF release.\n"
                                  "Please uninstall AIF manually.") );
        return;
    }
    tgLogger::instance() << cSeverity::DEBUG
                         << "Uninstall process: \""
                         << qsProcess
                         << "\""
                         << cLogMessage::EOM;

*/
    QProcess *qpAIFInstall = new QProcess(this);

    int nRet = qpAIFInstall->execute( QString("%1").arg( qsProcess ) );

    if( nRet < 0 )
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("Error occured when starting process:\n\n%1\n\nError code: %2\n"
                                 "-2 > Process cannot be started\n"
                                 "-1 > Process crashed").arg(qsProcess).arg(nRet) );
    }
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgManageReleases::on_pbCyclerInstall_clicked()
{
    QString qsCurrentPath = QDir::currentPath();

    if( ui->listCyclerReleases->currentRow() < 0 )
    {
        QMessageBox::warning( this, tr("Warning"), tr("No valid item selected!") );
        ui->pbCyclerInstall->setEnabled( false );
        return;
    }

    m_qsCyclerFileRelease = ui->listCyclerReleases->item(ui->listCyclerReleases->currentRow())->text();

    QString qsRelease = QString( "%1/%2" ).arg(m_qsLocalDirRelease).arg(m_qsCyclerFileRelease);
    qsRelease.replace( "/", "\\");

    QString qsCyclerDir = m_qsCyclerFileRelease.remove(".zip");
    QString qsDestDir = QString("%1\\%2").arg(tgPrefs::instance().dirCycler()).arg(qsCyclerDir);
    QDir    qdDest( qsDestDir );

    if( !qdDest.exists() )
    {
        qdDest.mkpath( qsDestDir );
    }
    if( QDir::setCurrent( qsDestDir ) )
    {
        QString     qsProcess = QString( "\"%1\\pkzip.exe\" -extract -directories=relative -overwrite=all \"%2\"" ).arg(qsCurrentPath).arg(qsRelease);
        QProcess   *qpCyclerInstall = new QProcess(this);

        int nRet = qpCyclerInstall->execute( qsProcess );

        if( nRet < 0 )
        {
            QMessageBox::warning( this, tr("Warning"),
                                  tr("Error occured when starting process:\n\n%1\n\nError code: %2\n"
                                     "-2 > Process cannot be started\n"
                                     "-1 > Process crashed").arg(qsProcess).arg(nRet) );
        }
        else if( !m_bSilentInstall )
        {
            QMessageBox::information( this, tr("Information"),
                                      tr("Installing selected cycler package finished."));
        }
    }

    QDir::setCurrent( qsCurrentPath );
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgManageReleases::on_pbCSSInstall_clicked()
{
    QString qsCurrentPath = QDir::currentPath();

    if( ui->listCSSReleases->currentRow() < 0 )
    {
        QMessageBox::warning( this, tr("Warning"), tr("No valid item selected!") );
        ui->pbCSSInstall->setEnabled( false );
        return;
    }

    m_qsCSSFileRelease = ui->listCSSReleases->item(ui->listCSSReleases->currentRow())->text();

    QString qsRelease = QString( "%1/%2" ).arg(m_qsLocalDirRelease).arg(m_qsCSSFileRelease);
    qsRelease.replace( "/", "\\");

    QString qsDestDir = QString("%1\\CellStatusScenario").arg(tgPrefs::instance().dirCycler());

    if( QDir::setCurrent( qsDestDir ) )
    {
        QString     qsProcess = QString( "\"%1\\pkzip.exe\" -extract -directories=relative -overwrite=all \"%2\"" ).arg(qsCurrentPath).arg(qsRelease);
        QProcess   *qpCSSInstall = new QProcess(this);

        int nRet = qpCSSInstall->execute( qsProcess );

        if( nRet < 0 )
        {
            QMessageBox::warning( this, tr("Warning"),
                                  tr("Error occured when starting process:\n\n%1\n\nError code: %2\n"
                                     "-2 > Process cannot be started\n"
                                     "-1 > Process crashed").arg(qsProcess).arg(nRet) );
        }
        else if( !m_bSilentInstall )
        {
            QMessageBox::information( this, tr("Information"),
                                      tr("Installing selected 5 Min. Check package finished."));
        }
    }

    QDir::setCurrent( qsCurrentPath );
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgManageReleases::on_pbExit_clicked()
{
    QDialog::close();
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgManageReleases::_fillListAIFReleases()
{
    ui->listAIFReleases->clear();

    QDir currentDir = QDir( m_qsLocalDirRelease );

    QStringList files;
    QStringList releases;

    releases << "*.msi";

    files = currentDir.entryList( releases, QDir::Files | QDir::NoSymLinks);

    for (int i = 0; i < files.size(); ++i)
    {
        ui->listAIFReleases->addItem( files.at(i) );
    }
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgManageReleases::_fillListCyclerReleases()
{
    ui->listCyclerReleases->clear();

    QDir currentDir = QDir( m_qsLocalDirRelease );

    QStringList files;
    QStringList releases;

    releases << "lby_*.zip";

    files = currentDir.entryList( releases, QDir::Files | QDir::NoSymLinks);

    for (int i = 0; i < files.size(); ++i)
    {
        ui->listCyclerReleases->addItem( files.at(i) );
    }
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgManageReleases::_fillListCSSReleases()
{
    ui->listCSSReleases->clear();

    QDir currentDir = QDir( m_qsLocalDirRelease );

    QStringList files;
    QStringList releases;

    releases << "css_*.zip";

    files = currentDir.entryList( releases, QDir::Files | QDir::NoSymLinks);

    for (int i = 0; i < files.size(); ++i)
    {
        ui->listCSSReleases->addItem( files.at(i) );
    }
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgManageReleases::on_listAIFReleases_clicked(const QModelIndex &index)
{
    if( ui->listAIFReleases->currentRow() > -1 )
    {
        ui->pbAIFInstall->setEnabled( true );
    }
    else
    {
        ui->pbAIFInstall->setEnabled( false );
    }
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgManageReleases::on_listCyclerReleases_clicked(const QModelIndex &index)
{
    if( ui->listCyclerReleases->currentRow() > -1 )
    {
        ui->pbCyclerInstall->setEnabled( true );
    }
    else
    {
        ui->pbCyclerInstall->setEnabled( false );
    }

}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgManageReleases::on_listCSSReleases_clicked(const QModelIndex &index)
{
    if( ui->listCSSReleases->currentRow() > -1 )
    {
        ui->pbCSSInstall->setEnabled( true );
    }
    else
    {
        ui->pbCSSInstall->setEnabled( false );
    }
}
//============================================================================================================
void dlgManageReleases::on_pbInstallAll_clicked()
{
    m_bSilentInstall = true;

    if( ui->pbCSSInstall->isEnabled() )
    {
        on_pbCSSInstall_clicked();
    }

    if( ui->pbCyclerInstall->isEnabled() )
    {
        on_pbCyclerInstall_clicked();
    }

    if( ui->pbAIFInstall->isEnabled() )
    {
        if( QMessageBox::question( this, tr("Question"),
                                   tr("Do you want to uninstall current AIF release?"),
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
        {
            on_pbAIFUninstall_clicked();
        }
        on_pbAIFInstall_clicked();
    }

    m_bSilentInstall = true;
}

void dlgManageReleases::_loadRegistrySettings()
{
    QSettings   regHKLM( "HKEY_LOCAL_MACHINE\\SOFTWARE\\GE Medical Systems\\Testinfra", QSettings::NativeFormat );

    m_qsRegAIFVersion = regHKLM.value( "AIF/Version", tr("<key not found>") ).toString();
    m_qsRegInnovaPrg = regHKLM.value( "AIF/InnovaPrg", tr("<key not found>") ).toString();
    m_qsRegFPPrg = regHKLM.value( "AIF/FPPrg", tr("<key not found>") ).toString();

    m_qsRegEngPC_IP = regHKLM.value( "AIF/engPC_IP", tr("<key not found>") ).toString();
    m_qsRegDL_IP = regHKLM.value( "AIF/DL_IP", tr("<key not found>") ).toString();

    m_qsRegAWHost = regHKLM.value( "AIF/AW/host", tr("<key not found>") ).toString();

    m_qsRegKeypadPort = regHKLM.value( "AIF/Keypad/KeypadPort", tr("<key not found>") ).toString();

    m_qsRegPosIPAddress = regHKLM.value( "AIF/Log/PosIPAddress", tr("<key not found>") ).toString();
    m_qsRegPosLogPort = regHKLM.value( "AIF/Log/PosLogPort", tr("<key not found>") ).toString();
    m_qsRegTablePosLogPort = regHKLM.value( "AIF/Log/TablePosLogPort", tr("<key not found>") ).toString();
    m_qsRegFootSwitchType = regHKLM.value( "AIF/FootswitchType", tr("<key not found>") ).toString();
    m_qsRegITUVersion = regHKLM.value( "AIF/ITUVersion", tr("<key not found>") ).toString();

    m_qsRegTableType = regHKLM.value( "Cycler/TableType", tr("<key not found>") ).toString();
    m_qsRegItuIP = regHKLM.value( "Cycler/TempData/itu ip", tr("<key not found>") ).toString();
}

void dlgManageReleases::_loadBackupSettings()
{
    QSettings   iniFile( "VTIAppManager.ini", QSettings::IniFormat );

    m_qsRegEngPC_IP = iniFile.value( "AIF/engPC_IP", tr("<key not found>") ).toString();
    m_qsRegDL_IP = iniFile.value( "AIF/DL_IP", tr("<key not found>") ).toString();

    m_qsRegAWHost = iniFile.value( "AW/host", tr("<key not found>") ).toString();

    m_qsRegKeypadPort = iniFile.value( "Keypad/KeypadPort", tr("<key not found>") ).toString();

    m_qsRegPosIPAddress = iniFile.value( "Log/PosIPAddress", tr("<key not found>") ).toString();
    m_qsRegPosLogPort = iniFile.value( "Log/PosLogPort", tr("<key not found>") ).toString();
    m_qsRegTablePosLogPort = iniFile.value( "Log/TablePosLogPort", tr("<key not found>") ).toString();
    m_qsRegFootSwitchType = iniFile.value( "AIF/FootswitchType", tr("<key not found>") ).toString();
    m_qsRegITUVersion = iniFile.value( "AIF/ITUVersion", tr("<key not found>") ).toString();

    m_qsRegTableType = iniFile.value( "Cycler/TableType", tr("<key not found>") ).toString();
    m_qsRegItuIP = iniFile.value( "Cycler/itu ip", tr("<key not found>") ).toString();
}

void dlgManageReleases::_backupRegistrySettings()
{
    QSettings   iniFile( "VTIAppManager.ini", QSettings::IniFormat );

    iniFile.setValue( "AIF/Version", m_qsRegAIFVersion );
    iniFile.setValue( "AIF/InnovaPrg", m_qsRegInnovaPrg );
    iniFile.setValue( "AIF/FPPrg", m_qsRegFPPrg );

    iniFile.setValue( "AIF/engPC_IP", m_qsRegEngPC_IP );
    iniFile.setValue( "AIF/DL_IP", m_qsRegDL_IP );

    iniFile.setValue( "AW/host", m_qsRegAWHost );

    iniFile.setValue( "Keypad/KeypadPort", m_qsRegKeypadPort );

    iniFile.setValue( "Log/PosIPAddress", m_qsRegPosIPAddress );
    iniFile.setValue( "Log/PosLogPort", m_qsRegPosLogPort );
    iniFile.setValue( "Log/TablePosLogPort", m_qsRegTablePosLogPort );
    iniFile.setValue( "AIF/FootswitchType", m_qsRegFootSwitchType );
    iniFile.setValue( "AIF/ITUVersion", m_qsRegITUVersion );

    iniFile.setValue( "Cycler/TableType", m_qsRegTableType );
    iniFile.setValue( "Cycler/itu ip", m_qsRegItuIP );
}

void dlgManageReleases::_restoreRegistrySettings()
{
    QSettings   regHKLM( "HKEY_LOCAL_MACHINE\\SOFTWARE\\GE Medical Systems\\Testinfra", QSettings::NativeFormat );

    regHKLM.setValue( "AIF/engPC_IP", m_qsRegEngPC_IP );
    regHKLM.setValue( "AIF/DL_IP", m_qsRegDL_IP );

    regHKLM.setValue( "AIF/AW/host", m_qsRegAWHost );

    regHKLM.setValue( "AIF/Keypad/KeypadPort", m_qsRegKeypadPort );

    regHKLM.setValue( "AIF/Log/PosIPAddress", m_qsRegPosIPAddress );
    regHKLM.setValue( "AIF/Log/PosLogPort", m_qsRegPosLogPort );
    regHKLM.setValue( "AIF/Log/TablePosLogPort", m_qsRegTablePosLogPort );
    regHKLM.setValue( "AIF/FootswitchType", m_qsRegFootSwitchType );
    regHKLM.setValue( "AIF/ITUVersion", m_qsRegITUVersion );

    regHKLM.setValue( "Cycler/TableType", m_qsRegTableType );
    regHKLM.setValue( "Cycler/TempData/itu ip", m_qsRegItuIP );
}
