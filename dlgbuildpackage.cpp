//============================================================================================================
//
//  VTI Test Manager application
//
//  File            : dlgtestinfraregistry.cpp
//  File version    : 1.0
//
//============================================================================================================

#include <QMessageBox>
#include <QDir>
#include <QByteArray>
#include <QScrollBar>

//============================================================================================================

#include "tmpreferences.h"
#include "dlgbuildpackage.h"
#include "ui_dlgbuildpackage.h"

//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
dlgBuildPackage::dlgBuildPackage(QWidget *parent) : QDialog(parent), ui(new Ui::dlgBuildPackage)
{
    m_qsCurrentPath = QDir::currentPath();

    m_qpBuildAIF = NULL;
    m_bProcessAIFHalted = false;

    m_qpBuildCycler = NULL;
    m_bProcessCyclerHalted = false;

    m_qpBuildCSS = NULL;
    m_bProcessCSSHalted = false;

    ui->setupUi(this);

    connect( ui->ledAIFVersion, SIGNAL(textChanged(QString)), this, SLOT(slot_AIFLabelUpdate()) );
    connect( ui->chkAIFEngRelease, SIGNAL(clicked()), this, SLOT(slot_AIFLabelUpdate()) );
    connect( ui->ledAIFENGNumber, SIGNAL(textChanged(QString)), this, SLOT(slot_AIFLabelUpdate()) );
    connect( ui->ledCyclerVersion, SIGNAL(textChanged(QString)), this, SLOT(slot_CyclerLabelUpdate()) );
    connect( ui->chkCyclerEngRelease, SIGNAL(clicked()), this, SLOT(slot_CyclerLabelUpdate()) );
    connect( ui->ledCyclerENGNumber, SIGNAL(textChanged(QString)), this, SLOT(slot_CyclerLabelUpdate()) );
    connect( ui->ledCSSVersion, SIGNAL(textChanged(QString)), this, SLOT(slot_CSSLabelUpdate()) );
    connect( ui->chkCSSEngRelease, SIGNAL(clicked()), this, SLOT(slot_CSSLabelUpdate()) );
    connect( ui->ledCSSENGNumber, SIGNAL(textChanged(QString)), this, SLOT(slot_CSSLabelUpdate()) );

    QFileInfoList roots = QDir::drives();

    for( int i=0; i<roots.count(); i++ )
    {
        ui->cmbAIFBuildDrive->addItem( roots.at(i).filePath().left(1) );
        ui->cmbCyclerBuildDrive->addItem( roots.at(i).filePath().left(1) );
        ui->cmbCSSBuildDrive->addItem( roots.at(i).filePath().left(1) );
        if( ui->cmbAIFBuildDrive->itemText( ui->cmbAIFBuildDrive->count()-1 ).compare(tgPrefs::instance().buildAIFDrive()) == 0 )
        {
            ui->cmbAIFBuildDrive->setCurrentIndex( i );
        }
        if( ui->cmbCyclerBuildDrive->itemText( ui->cmbCyclerBuildDrive->count()-1 ).compare(tgPrefs::instance().buildCyclerDrive()) == 0 )
        {
            ui->cmbCyclerBuildDrive->setCurrentIndex( i );
        }
        if( ui->cmbCSSBuildDrive->itemText( ui->cmbCSSBuildDrive->count()-1 ).compare(tgPrefs::instance().buildCSSDrive()) == 0 )
        {
            ui->cmbCSSBuildDrive->setCurrentIndex( i );
        }
    }

    ui->ledAIFVersion->setText( tgPrefs::instance().buildAIFVersion() );
    ui->chkAIFEngRelease->setChecked( tgPrefs::instance().buildAIFEngVersion() );
    ui->ledAIFENGNumber->setEnabled( ui->chkAIFEngRelease->isChecked() );
    ui->ledAIFENGNumber->setText( tgPrefs::instance().buildAIFEngNumber() );
    ui->ledCyclerVersion->setText( tgPrefs::instance().buildCyclerVersion() );
    ui->chkCyclerEngRelease->setChecked( tgPrefs::instance().buildCyclerEngVersion() );
    ui->ledCyclerENGNumber->setEnabled( ui->chkCyclerEngRelease->isChecked() );
    ui->ledCyclerENGNumber->setText( tgPrefs::instance().buildCyclerEngNumber() );
    ui->ledCSSVersion->setText( tgPrefs::instance().buildCSSVersion() );
    ui->chkCSSEngRelease->setChecked( tgPrefs::instance().buildCSSEngVersion() );
    ui->ledCSSENGNumber->setEnabled( ui->chkCSSEngRelease->isChecked() );
    ui->ledCSSENGNumber->setText( tgPrefs::instance().buildCSSEngNumber() );
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
dlgBuildPackage::~dlgBuildPackage()
{
    if( m_qpBuildAIF != NULL )
    {
        m_qpBuildAIF->close();
        delete m_qpBuildAIF;
        m_qpBuildAIF = NULL;
    }

    if( m_qpBuildCycler != NULL )
    {
        m_qpBuildCycler->close();
        delete m_qpBuildCycler;
        m_qpBuildCycler = NULL;
    }

    if( m_qpBuildCSS != NULL )
    {
        m_qpBuildCSS->close();
        delete m_qpBuildCSS;
        m_qpBuildCSS = NULL;
    }

    QDir::setCurrent( m_qsCurrentPath );

    delete ui;
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgBuildPackage::on_chkAIFEngRelease_clicked()
{
    ui->ledAIFENGNumber->setEnabled( ui->chkAIFEngRelease->isChecked() );
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgBuildPackage::on_pbStartAIFBuild_clicked()
{
    QString qsCurrentPath = QDir::currentPath();

    ui->pbCopyAIFRelease->setEnabled( false );
    ui->ptAIFStdOut->clear();

    if( ui->chkSaveAIFBuildSettings )
    {
        _saveBuildSettings();
    }

    if( m_qpBuildAIF != NULL )
    {
        m_qpBuildAIF->close();
        delete m_qpBuildAIF;
        m_qpBuildAIF = NULL;
    }

    m_qpBuildAIF = new QProcess();

    if( QDir::setCurrent( QString("%1:\\testinfra\\build").arg(ui->cmbAIFBuildDrive->currentText()) ) )
    {
        QString     qsENG = (ui->chkAIFEngRelease->isChecked()?QString("_ENG%1").arg(ui->ledAIFENGNumber->text()):"");
        QString     qsBuildCommand = QString("tclsh80 TIPackBuild.tcl AIF_%1").arg(ui->ledAIFVersion->text());

        if( ui->chkAIFEngRelease->isChecked() )
            qsBuildCommand += qsENG;

        if( ui->rbAIFBuildAIF->isChecked() )
            qsBuildCommand += QString( " AIF" );
        else if( ui->rbAIFBuildAW->isChecked() )
            qsBuildCommand += QString( " AW" );
        else if( ui->rbAIFBuildCYC->isChecked() )
            qsBuildCommand += QString( " CYC" );
        else if( ui->rbAIFBuildHW->isChecked() )
            qsBuildCommand += QString( " HW" );
        else if( ui->rbAIFBuildMSI->isChecked() )
            qsBuildCommand += QString( " MSI" );

        if( ui->chkAIFSilent->isChecked() )
            qsBuildCommand += QString( " -no_usr_action" );

        m_qpBuildAIF->setProcessChannelMode( QProcess::MergedChannels );
        connect( m_qpBuildAIF, SIGNAL(readyReadStandardOutput()), this, SLOT(slot_AIFStdoutUpdated()) );
        connect( m_qpBuildAIF, SIGNAL(finished(int)), this, SLOT(slot_AIFProcessFinished()) );

        m_qpBuildAIF->start( qsBuildCommand, QProcess::Unbuffered | QProcess::ReadWrite );

        _enableAIFBuildSettings( false );
    }
    else
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("There is no valid AIF build directory at the given drive.\n"
                                 "Please select a different drive.") );
    }

    QDir::setCurrent( qsCurrentPath );
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgBuildPackage::on_pbAIFYes_clicked()
{
    m_qpBuildAIF->write( "yes\n" );
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgBuildPackage::on_pbAIFNo_clicked()
{
    m_qpBuildAIF->write( "no\n" );
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgBuildPackage::on_pbAIFHalt_clicked()
{
    m_bProcessAIFHalted = true;
    m_qpBuildAIF->close();
    delete m_qpBuildAIF;
    m_qpBuildAIF = NULL;
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgBuildPackage::on_pbCopyAIFRelease_clicked()
{
    ui->pbCopyAIFRelease->setEnabled( false );

    QString qsDir = QString("%1:\\testinfra\\build\\MSIPackage\\Release").arg(ui->cmbAIFBuildDrive->currentText());
    QString qsFileName = QString("Testinfra_AIF_%1").arg(ui->ledAIFVersion->text());

    if( ui->chkAIFEngRelease->isChecked() )
    {
        qsFileName += (ui->chkAIFEngRelease->isChecked()?QString("_ENG%1").arg(ui->ledAIFENGNumber->text()):"");
    }
    qsFileName += QString(".msi");

    _appendAIFStdOut( QString("\nCopy AIF release '%1'\n"
                              "    from '%2'\n"
                              "    to '%3'\n").arg(qsFileName).arg(qsDir).arg(tgPrefs::instance().dirLocalReleases()) );

    QString qsTemp;

    if( QDir::setCurrent( qsDir ) )
    {
        QFile   qfAIFRelease( QString("%1\\%2").arg(qsDir).arg(qsFileName) );
        QFile   qfDestRelease( QString("%1\\%2").arg(tgPrefs::instance().dirLocalReleases()).arg(qsFileName) );

        if( qfDestRelease.exists() )
        {
            if( QMessageBox::question( this, tr("Question"),
                                       tr("The install package on the destination folder already exists.\n"
                                          "Do you want to overwrite it?"),
                                       QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
            {
                qfDestRelease.remove();
            }
        }

        if( qfAIFRelease.copy( QString("%1\\%2").arg(tgPrefs::instance().dirLocalReleases()).arg(qsFileName) ) )
        {
            qsTemp = QString("\nSucceeded\n");
        }
        else
        {
            qsTemp = QString("\nFailed\n");
        }
    }
    else
    {
        qsTemp = QString("\nFailed\n");
    }
    _appendAIFStdOut( qsTemp );
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgBuildPackage::on_pbStartCyclerBuild_clicked()
{
    QString qsCurrentPath = QDir::currentPath();

    ui->pbCopyCyclerRelease->setEnabled( false );
    ui->ptCyclerStdOut->clear();

    if( ui->chkSaveCyclerBuildSettings )
    {
        _saveBuildSettings();
    }

    if( m_qpBuildCycler != NULL )
    {
        m_qpBuildCycler->close();
        delete m_qpBuildCycler;
        m_qpBuildCycler = NULL;
    }

    m_qpBuildCycler = new QProcess();

    if( QDir::setCurrent( QString("%1:\\sat\\tools").arg(ui->cmbCyclerBuildDrive->currentText()) ) )
    {
        QString     qsENG = (ui->chkCyclerEngRelease->isChecked()?QString("_ENG%1").arg(ui->ledCyclerENGNumber->text()):"");
        QString     qsBuildCommand = QString("tclsh80 cpcycler.tcl LBY_%1").arg(ui->ledCyclerVersion->text());

        if( ui->chkCyclerEngRelease->isChecked() )
            qsBuildCommand += qsENG;

        m_qpBuildCycler->setProcessChannelMode( QProcess::MergedChannels );
        connect( m_qpBuildCycler, SIGNAL(readyReadStandardOutput()), this, SLOT(slot_CyclerStdoutUpdated()) );
        connect( m_qpBuildCycler, SIGNAL(finished(int)), this, SLOT(slot_CyclerProcessFinished()) );

        m_qpBuildCycler->start( qsBuildCommand, QProcess::Unbuffered | QProcess::ReadWrite );

        _enableCyclerBuildSettings( false );
    }
    else
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("There is no valid Cycler build directory at the given drive.\n"
                                 "Please select a different drive.") );
    }

    QDir::setCurrent( qsCurrentPath );
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgBuildPackage::on_pbCyclerYes_clicked()
{
    m_qpBuildCycler->write( "yes\n" );
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgBuildPackage::on_pbCyclerNo_clicked()
{
    m_qpBuildCycler->write( "no\n" );
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgBuildPackage::on_pbCyclerHalt_clicked()
{
    m_bProcessCyclerHalted = true;
    m_qpBuildCycler->close();
    delete m_qpBuildCycler;
    m_qpBuildCycler = NULL;
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgBuildPackage::on_chkCyclerEngRelease_clicked()
{
    ui->ledCyclerENGNumber->setEnabled( ui->chkCyclerEngRelease->isChecked() );
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgBuildPackage::on_pbStartCSSBuild_clicked()
{
    QString qsCurrentPath = QDir::currentPath();

    ui->pbCopyCSSRelease->setEnabled( false );
    ui->ptCSSStdOut->clear();

    if( ui->chkSaveCSSBuildSettings )
    {
        _saveBuildSettings();
    }

    if( m_qpBuildCSS != NULL )
    {
        m_qpBuildCSS->close();
        delete m_qpBuildCSS;
        m_qpBuildCSS = NULL;
    }

    m_qpBuildCSS = new QProcess();

    if( QDir::setCurrent( QString("%1:\\sat\\CellStatusScenario").arg(ui->cmbCSSBuildDrive->currentText()) ) )
    {
        QString     qsENG = (ui->chkCSSEngRelease->isChecked()?QString("_ENG%1").arg(ui->ledCSSENGNumber->text()):"");
        QString     qsVersion = QString( "CSS_%1" ).arg( ui->ledCSSVersion->text() );

        if( ui->chkCSSEngRelease->isChecked() )
            qsVersion.append( qsENG );

        QFile       qfVersionFile( "version.txt" );

        if( qfVersionFile.exists() )
        {
            qfVersionFile.remove();
        }
        if( qfVersionFile.open( QIODevice::WriteOnly|QIODevice::Text ) )
        {
            qfVersionFile.write( qsVersion.toStdString().c_str() );
            qfVersionFile.close();
        }
        else
        {
            _appendCSSStdOut( tr( "Unable to create version file: %1/version.txt" ).arg( QDir::currentPath() ) );
            return;
        }

        QString     qsBuildCommand = QString("\"%1\\pkzip.exe\" -add %2.zip ").arg(qsCurrentPath).arg(qsVersion);

        qsBuildCommand.append( "CellSanityActionLib.tcl " );
        qsBuildCommand.append( "CellStatusActionLib.tcl " );
        qsBuildCommand.append( "ChkApp.tcl " );
        qsBuildCommand.append( "ChkGUI.tcl " );
        qsBuildCommand.append( "version.txt " );

        m_qpBuildCSS->setProcessChannelMode( QProcess::MergedChannels );
        connect( m_qpBuildCSS, SIGNAL(readyReadStandardOutput()), this, SLOT(slot_CSSStdoutUpdated()) );
        connect( m_qpBuildCSS, SIGNAL(finished(int)), this, SLOT(slot_CSSProcessFinished()) );

        m_qpBuildCSS->start( qsBuildCommand, QProcess::Unbuffered | QProcess::ReadWrite );

        _enableCSSBuildSettings( false );
    }
    else
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("There is no valid CSS build directory at the given drive.\n"
                                 "Please select a different drive.") );
    }

    QDir::setCurrent( qsCurrentPath );
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgBuildPackage::on_pbCSSYes_clicked()
{
    m_qpBuildCSS->write( "yes\n" );
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgBuildPackage::on_pbCSSNo_clicked()
{
    m_qpBuildCSS->write( "no\n" );
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgBuildPackage::on_pbCSSHalt_clicked()
{
    m_bProcessCSSHalted = true;
    m_qpBuildCSS->close();
    delete m_qpBuildCSS;
    m_qpBuildCSS = NULL;
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgBuildPackage::on_chkCSSEngRelease_clicked()
{
    ui->ledCSSENGNumber->setEnabled( ui->chkCSSEngRelease->isChecked() );
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgBuildPackage::on_pbExit_clicked()
{
    QDialog::accept();
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgBuildPackage::slot_AIFStdoutUpdated()
{
    QByteArray  qbaData = m_qpBuildAIF->readAllStandardOutput();

    _appendAIFStdOut( QString(qbaData) );
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgBuildPackage::slot_AIFProcessFinished()
{
    QString qsFinishMessage = QString( "Build process %1" ).arg( m_bProcessAIFHalted?"halted by user":"finished" );

    _appendAIFStdOut( QString("\n----------------------------------------------------------\n"
                              ">> %1 <<\n"
                              "----------------------------------------------------------\n").arg(qsFinishMessage) );

    if( !m_bProcessAIFHalted && ui->ptAIFStdOut->toPlainText().contains( "Test Infra package build finished" ) )
    {
        ui->pbCopyAIFRelease->setEnabled( true );
    }

    m_bProcessAIFHalted = false;
    _enableAIFBuildSettings( true );
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgBuildPackage::slot_CyclerStdoutUpdated()
{
    QByteArray qbaData = m_qpBuildCycler->readAllStandardOutput();

    _appendCyclerStdOut( QString( qbaData ) );
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgBuildPackage::slot_CyclerProcessFinished()
{
    QString qsFinishMessage = QString( "Build process %1" ).arg( m_bProcessCyclerHalted?"halted by user":"finished" );

    _appendCyclerStdOut( QString("\n----------------------------------------------------------\n"
                                 ">> %1 <<\n"
                                 "----------------------------------------------------------\n").arg(qsFinishMessage) );

    if( !m_bProcessCyclerHalted && ui->ptCyclerStdOut->toPlainText().contains( "Zip file created:" ) )
    {
        ui->pbCopyCyclerRelease->setEnabled( true );
    }

    m_bProcessCyclerHalted = false;
    _enableCyclerBuildSettings( true );
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgBuildPackage::slot_CSSStdoutUpdated()
{
    QByteArray qbaData = m_qpBuildCSS->readAllStandardOutput();

    _appendCSSStdOut( QString( qbaData ) );
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgBuildPackage::slot_CSSProcessFinished()
{
    QString qsFinishMessage = QString( "Build process %1" ).arg( m_bProcessCSSHalted?"halted by user":"finished" );

    _appendCSSStdOut( QString("\n----------------------------------------------------------\n"
                                 ">> %1 <<\n"
                                 "----------------------------------------------------------\n").arg(qsFinishMessage) );

    ui->pbCopyCSSRelease->setEnabled( true );

    m_bProcessCSSHalted = false;
    _enableCSSBuildSettings( true );
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgBuildPackage::_enableAIFBuildSettings(bool p_bEnabled)
{
    ui->pbExit->setEnabled( p_bEnabled );

    ui->cmbAIFBuildDrive->setEnabled( p_bEnabled );
    ui->ledAIFVersion->setEnabled( p_bEnabled );
    ui->chkAIFEngRelease->setEnabled( p_bEnabled );
    ui->ledAIFENGNumber->setEnabled( p_bEnabled );
    ui->pbStartAIFBuild->setEnabled( p_bEnabled );
    ui->rbAIFBuildALL->setEnabled( p_bEnabled );
    ui->rbAIFBuildHW->setEnabled( p_bEnabled );
    ui->rbAIFBuildAIF->setEnabled( p_bEnabled );
    ui->rbAIFBuildCYC->setEnabled( p_bEnabled );
    ui->rbAIFBuildAW->setEnabled( p_bEnabled );
    ui->rbAIFBuildMSI->setEnabled( p_bEnabled );
    ui->chkAIFSilent->setEnabled( p_bEnabled );
    ui->chkSaveAIFBuildSettings->setEnabled( p_bEnabled );

    ui->pbAIFYes->setEnabled( !p_bEnabled );
    ui->pbAIFNo->setEnabled( !p_bEnabled );
    ui->pbAIFHalt->setEnabled( !p_bEnabled );
}

//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgBuildPackage::_enableCyclerBuildSettings(bool p_bEnabled)
{
    ui->pbExit->setEnabled( p_bEnabled );

    ui->cmbCyclerBuildDrive->setEnabled( p_bEnabled );
    ui->ledCyclerVersion->setEnabled( p_bEnabled );
    ui->chkCyclerEngRelease->setEnabled( p_bEnabled );
    ui->ledCyclerENGNumber->setEnabled( p_bEnabled );
    ui->pbStartCyclerBuild->setEnabled( p_bEnabled );
    ui->chkSaveCyclerBuildSettings->setEnabled( p_bEnabled );

    ui->pbCyclerYes->setEnabled( !p_bEnabled );
    ui->pbCyclerNo->setEnabled( !p_bEnabled );
    ui->pbCyclerHalt->setEnabled( !p_bEnabled );
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgBuildPackage::_enableCSSBuildSettings(bool p_bEnabled)
{
    ui->pbExit->setEnabled( p_bEnabled );

    ui->cmbCSSBuildDrive->setEnabled( p_bEnabled );
    ui->ledCSSVersion->setEnabled( p_bEnabled );
    ui->chkCSSEngRelease->setEnabled( p_bEnabled );
    ui->ledCSSENGNumber->setEnabled( p_bEnabled );
    ui->pbStartCSSBuild->setEnabled( p_bEnabled );
    ui->chkSaveCSSBuildSettings->setEnabled( p_bEnabled );

    ui->pbCSSYes->setEnabled( !p_bEnabled );
    ui->pbCSSNo->setEnabled( !p_bEnabled );
    ui->pbCSSHalt->setEnabled( !p_bEnabled );
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgBuildPackage::_saveBuildSettings()
{
    if( ui->chkSaveAIFBuildSettings->isChecked() )
    {
        tgPrefs::instance().setBuildAIFDrive( ui->cmbAIFBuildDrive->currentText() );
        tgPrefs::instance().setBuildAIFVersion( ui->ledAIFVersion->text() );
        tgPrefs::instance().setBuildAIFEngVersion( ui->chkAIFEngRelease->isChecked() );
        tgPrefs::instance().setBuildAIFEngNumber( ui->ledAIFENGNumber->text() );
        tgPrefs::instance().save();
    }
    if( ui->chkSaveCyclerBuildSettings->isChecked() )
    {
        tgPrefs::instance().setBuildCyclerDrive( ui->cmbCyclerBuildDrive->currentText() );
        tgPrefs::instance().setBuildCyclerVersion( ui->ledCyclerVersion->text() );
        tgPrefs::instance().setBuildCyclerEngVersion( ui->chkCyclerEngRelease->isChecked() );
        tgPrefs::instance().setBuildCyclerEngNumber( ui->ledCyclerENGNumber->text() );
        tgPrefs::instance().save();
    }
    if( ui->chkSaveCSSBuildSettings->isChecked() )
    {
        tgPrefs::instance().setBuildCSSDrive( ui->cmbCSSBuildDrive->currentText() );
        tgPrefs::instance().setBuildCSSVersion( ui->ledCSSVersion->text() );
        tgPrefs::instance().setBuildCSSEngVersion( ui->chkCSSEngRelease->isChecked() );
        tgPrefs::instance().setBuildCSSEngNumber( ui->ledCSSENGNumber->text() );
        tgPrefs::instance().save();
    }
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgBuildPackage::on_pbCopyCyclerRelease_clicked()
{
    ui->pbCopyCyclerRelease->setEnabled( false );

    // Zip file created: D:\Documents and Settings\bikfata\tmp\cycler\LBY_test/LBY_test.zip
    int nPosStart = ui->ptCyclerStdOut->toPlainText().indexOf( QString("Zip file created: ") );
    int nPosStop  = ui->ptCyclerStdOut->toPlainText().indexOf( QString("\n"), nPosStart );

    QString qsDir = ui->ptCyclerStdOut->toPlainText().mid( nPosStart, nPosStop-nPosStart ).remove( QString("Zip file created: ") );

    qsDir.replace( QString("/"), QString("\\") );

    QString qsFileName  = qsDir.right( qsDir.length()-qsDir.lastIndexOf( QString("\\") ) );

    qsDir.remove( qsFileName );
    qsFileName.remove( "\\" );

    _appendCyclerStdOut( QString("\nCopy Cycler package '%1'\n"
                                 "    from '%2'\n"
                                 "    to '%3'\n").arg(qsFileName).arg(qsDir).arg(tgPrefs::instance().dirLocalReleases()) );

    QString qsTemp;

    if( QDir::setCurrent( qsDir ) )
    {
        QFile   qfCyclerRelease( QString("%1\\%2").arg(qsDir).arg(qsFileName) );
        QFile   qfDestRelease( QString("%1\\%2").arg(tgPrefs::instance().dirLocalReleases()).arg(qsFileName) );

        if( qfDestRelease.exists() )
        {
            if( QMessageBox::question( this, tr("Question"),
                                       tr("The install package on the destination folder already exists.\n"
                                          "Do you want to overwrite it?"),
                                       QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
            {
                qfDestRelease.remove();
            }
        }

        if( qfCyclerRelease.copy( QString("%1\\%2").arg(tgPrefs::instance().dirLocalReleases()).arg(qsFileName) ) )
        {
            qsTemp = QString("\nSucceeded\n");
        }
        else
        {
            qsTemp = QString("\nFailed\n");
        }
    }
    else
    {
        qsTemp = QString("\nFailed\n");
    }
    _appendCyclerStdOut( qsTemp );
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgBuildPackage::on_pbCopyCSSRelease_clicked()
{
    ui->pbCopyCSSRelease->setEnabled( false );

    QString     qsENG = (ui->chkCSSEngRelease->isChecked()?QString("_ENG%1").arg(ui->ledCSSENGNumber->text()):"");
    QString     qsFileName = QString( "CSS_%1" ).arg( ui->ledCSSVersion->text() );

    if( ui->chkCSSEngRelease->isChecked() )
        qsFileName.append( qsENG );

    qsFileName.append( ".zip" );

    QString qsDir = QString("%1:/sat/CellStatusScenario").arg(ui->cmbCSSBuildDrive->currentText());

    qsDir.replace( QString("/"), QString("\\") );
    _appendCSSStdOut( QString("\nCopy CSS package '%1'\n"
                                 "    from '%2'\n"
                                 "    to '%3'\n").arg(qsFileName).arg(qsDir).arg(tgPrefs::instance().dirLocalReleases()) );

    QString qsTemp;

    if( QDir::setCurrent( qsDir ) )
    {
        QFile   qfCSSRelease( QString("%1\\%2").arg(qsDir).arg(qsFileName) );
        QFile   qfDestRelease( QString("%1\\%2").arg(tgPrefs::instance().dirLocalReleases()).arg(qsFileName) );

        if( qfDestRelease.exists() )
        {
            if( QMessageBox::question( this, tr("Question"),
                                       tr("The install package on the destination folder already exists.\n"
                                          "Do you want to overwrite it?"),
                                       QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
            {
                qfDestRelease.remove();
            }
        }

        if( qfCSSRelease.copy( QString("%1\\%2").arg(tgPrefs::instance().dirLocalReleases()).arg(qsFileName) ) )
        {
            qsTemp = QString("\nSucceeded\n");
        }
        else
        {
            qsTemp = QString("\nFailed\n");
        }
    }
    else
    {
        qsTemp = QString("\nFailed\n");
    }
    _appendCSSStdOut( qsTemp );
}
//============================================================================================================
void dlgBuildPackage::_appendAIFStdOut(const QString &p_qsText)
{
    if( p_qsText.length() < 1 )
        return;

    QString     qsText  = ui->ptAIFStdOut->toPlainText();

    qsText.append( p_qsText );
    ui->ptAIFStdOut->clear();
    ui->ptAIFStdOut->setPlainText( qsText );
    ui->ptAIFStdOut->verticalScrollBar()->setValue( ui->ptAIFStdOut->verticalScrollBar()->maximum() );
}

void dlgBuildPackage::_appendCyclerStdOut(const QString &p_qsText)
{
    if( p_qsText.length() < 1 )
        return;

    QString     qsText  = ui->ptCyclerStdOut->toPlainText();

    qsText.append( p_qsText );
    ui->ptCyclerStdOut->clear();
    ui->ptCyclerStdOut->setPlainText( qsText );
    ui->ptCyclerStdOut->verticalScrollBar()->setValue( ui->ptCyclerStdOut->verticalScrollBar()->maximum() );
}

void dlgBuildPackage::_appendCSSStdOut(const QString &p_qsText)
{
    if( p_qsText.length() < 1 )
        return;

    QString     qsText  = ui->ptCSSStdOut->toPlainText();

    qsText.append( p_qsText );
    ui->ptCSSStdOut->clear();
    ui->ptCSSStdOut->setPlainText( qsText );
    ui->ptCSSStdOut->verticalScrollBar()->setValue( ui->ptCSSStdOut->verticalScrollBar()->maximum() );
}

void dlgBuildPackage::slot_AIFLabelUpdate()
{
    QString qsLabel = QString("=> AIF_%1").arg( ui->ledAIFVersion->text() );

    if( ui->chkAIFEngRelease->isChecked() )
        qsLabel += QString( "_ENG%1" ).arg( ui->ledAIFENGNumber->text() );

    ui->lblAIFLabel->setText( qsLabel );
}

void dlgBuildPackage::slot_CyclerLabelUpdate()
{
    QString qsLabel = QString("=> LBY_%1").arg( ui->ledCyclerVersion->text() );

    if( ui->chkCyclerEngRelease->isChecked() )
        qsLabel += QString( "_ENG%1" ).arg( ui->ledCyclerENGNumber->text() );

    ui->lblCyclerLabel->setText( qsLabel );
}

void dlgBuildPackage::slot_CSSLabelUpdate()
{
    QString qsLabel = QString("=> CSS_%1").arg( ui->ledCSSVersion->text() );

    if( ui->chkCSSEngRelease->isChecked() )
        qsLabel += QString( "_ENG%1" ).arg( ui->ledCSSENGNumber->text() );

    ui->lblCSSLabel->setText( qsLabel );
}
