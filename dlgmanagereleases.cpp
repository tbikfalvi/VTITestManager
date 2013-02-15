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

    ui->pbAIFInstall->setEnabled( false );
    ui->pbCyclerInstall->setEnabled( false );
    ui->pbCSSInstall->setEnabled( false );

    m_qsLocalDirRelease = tgPrefs::instance().dirLocalReleases();

    ui->ledCurrentDir->setText( m_qsLocalDirRelease );
    ui->ledCurrentDir->setToolTip( m_qsLocalDirRelease );

    QSettings   iniFile( "VTITestManager.ini", QSettings::IniFormat );

    ui->ledRegAIFVersion->setText( iniFile.value( "AIF/Version", tr("<key not found>") ).toString() );
    ui->ledRegInnovaPrg->setText( iniFile.value( "AIF/InnovaPrg", tr("<key not found>") ).toString() );
    ui->ledRegFPProgram->setText( iniFile.value( "AIF/FPPrg", tr("<key not found>") ).toString() );
    ui->ledRegIPEng->setText( iniFile.value( "AIF/engPC_IP", tr("<key not found>") ).toString() );
    ui->ledRegIPAW->setText( iniFile.value( "AW/host", tr("<key not found>") ).toString() );

    _fillListAIFReleases();
    _fillListCyclerReleases();
    _fillListCSSReleases();
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
dlgManageReleases::~dlgManageReleases()
{
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
        QFile   qfFile;

        qfFile.setFileName( qsRelease );
        qfFile.copy( QString("%1/CurrentAIFRelease.msi").arg(QDir::currentPath()) );

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
    }
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgManageReleases::on_pbAIFUninstall_clicked()
{
    QSettings   obReg( QString("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"), QSettings::NativeFormat );
    QStringList qslFolders  = obReg.childGroups();
    QString     qsProcess   = "";

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

    QProcess *qpAIFInstall = new QProcess(this);

    tgLogger::instance() << cSeverity::DEBUG
                         << "Uninstall process: \""
                         << qsProcess
                         << "\""
                         << cLogMessage::EOM;

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
        else
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
        else
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
