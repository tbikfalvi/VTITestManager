//============================================================================================================
//
//  VTI Test Manager application
//
//  File            : dlgprocesscmd.cpp
//  File version    : 1.0
//
//============================================================================================================

#include <QMessageBox>
#include <QDir>
#include <QByteArray>
#include <QScrollBar>
#include <QFileDialog>

//============================================================================================================

#include "tmpreferences.h"
#include "dlgprocesscmd.h"
#include "ui_dlgprocesscmd.h"

//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
dlgProcessCmd::dlgProcessCmd(QWidget *parent) : QDialog(parent), ui(new Ui::dlgProcessCmd)
{
    ui->setupUi(this);

    setWindowTitle( QString("Execute custom processes") );

    m_qsCurrentPath = QDir::currentPath();

    QFileInfoList roots = QDir::drives();

    for( int i=0; i<roots.count(); i++ )
    {
        ui->cmbDrive->addItem( roots.at(i).filePath().left(1) );
        if( ui->cmbDrive->itemText( ui->cmbDrive->count()-1 ).compare(tgPrefs::instance().processDrive()) == 0 )
        {
            ui->cmbDrive->setCurrentIndex( i );
        }
    }
    ui->ledProcessPath->setText( tgPrefs::instance().processPath() );
    _setProcessPath();

    QStringList qslCommands = tgPrefs::instance().commands();

    ui->cmbCommand->addItem("");
    for( int i=0; i<qslCommands.count(); i++ )
    {
        ui->cmbCommand->addItem( qslCommands.at(i) );
    }

    ui->cmbMessage->addItem( "" );

    QSettings   iniFile( "VTITestManager.ini", QSettings::IniFormat );
    int         dlgWidth    = iniFile.value( "Dialogs/ProcessCommand_width", 669 ).toInt();
    int         dlgHeight   = iniFile.value( "Dialogs/ProcessCommand_height", 563 ).toInt();
    QPoint      qpDlgSize   = QPoint( dlgWidth, dlgHeight );

    resize( qpDlgSize.x(), qpDlgSize.y() );
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
dlgProcessCmd::~dlgProcessCmd()
{
    QSettings   iniFile( "VTITestManager.ini", QSettings::IniFormat );

    iniFile.setValue( "Dialogs/ProcessCommand_width", width() );
    iniFile.setValue( "Dialogs/ProcessCommand_height", height() );

    if( m_qpCommand != NULL )
    {
        m_qpCommand->close();
        delete m_qpCommand;
        m_qpCommand = NULL;
    }

    QDir::setCurrent( m_qsCurrentPath );

    delete ui;
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgProcessCmd::on_cmbDrive_currentIndexChanged(int index)
{
    _setProcessPath();
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgProcessCmd::on_pbChangeDir_clicked()
{
    QString qsDirectory = QFileDialog::getExistingDirectory(this, tr("Select the desired directory"), m_qsProcessPath );

    if (!qsDirectory.isEmpty())
    {
        m_qsProcessPath = qsDirectory;
        ui->cmbDrive->setCurrentIndex( ui->cmbDrive->findText(m_qsProcessPath.left(1)) );
        ui->ledProcessPath->setText( m_qsProcessPath.right(m_qsProcessPath.length()-2) );
    }
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgProcessCmd::on_cmbCommand_textChanged(const QString &arg1)
{
    if( arg1.length() > 0 )
        ui->pbProcess->setEnabled( true );
    else
        ui->pbProcess->setEnabled( false );
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgProcessCmd::on_pbClearOutput_clicked()
{
    ui->ptStdOut->clear();
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgProcessCmd::on_pbProcess_clicked()
{
    if( ui->chkSavePath->isChecked() )
    {
        tgPrefs::instance().setProcessDrive( ui->cmbDrive->currentText() );
        tgPrefs::instance().setProcessPath( ui->ledProcessPath->text() );
        tgPrefs::instance().save();
    }

    if( ui->chkSaveProcess->isChecked() )
    {
        tgPrefs::instance().addCommand( ui->cmbCommand->currentText() );
        tgPrefs::instance().save();
    }

    if( m_qpCommand != NULL )
    {
        m_qpCommand->close();
        delete m_qpCommand;
        m_qpCommand = NULL;
    }

    m_qpCommand = new QProcess();

    if( QDir( m_qsProcessPath ).exists() )
    {
        m_qpCommand->setWorkingDirectory( m_qsProcessPath );
        m_qpCommand->setProcessChannelMode( QProcess::MergedChannels );
        connect( m_qpCommand, SIGNAL(readyReadStandardOutput()), this, SLOT(slot_StdoutUpdated()) );
        connect( m_qpCommand, SIGNAL(finished(int)), this, SLOT(slot_ProcessFinished()) );

        m_qpCommand->start( ui->cmbCommand->currentText(), QProcess::Unbuffered | QProcess::ReadWrite );

        _enableCommands( true );
    }
    else
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("The target path does not exists\n\n%1").arg(m_qsProcessPath) );
    }
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgProcessCmd::on_pbEnterMsg_clicked()
{
    m_qpCommand->write( QString("%1\n").arg(ui->cmbMessage->currentText()).toStdString().c_str() );
    if( ui->cmbMessage->findText( ui->cmbMessage->currentText(), Qt::MatchExactly ) == -1 )
    {
        ui->cmbMessage->addItem( ui->cmbMessage->currentText() );
    }
    ui->cmbMessage->setCurrentIndex( 0 );
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgProcessCmd::on_pbExit_clicked()
{
    m_bProcessHalted = true;
    m_qpCommand->close();
    delete m_qpCommand;
    m_qpCommand = NULL;
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgProcessCmd::on_pbExitDlg_clicked()
{
    QDialog::accept();
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgProcessCmd::slot_StdoutUpdated()
{
    QByteArray qbaData = m_qpCommand->readAllStandardOutput();

    _appendStdOut( QString( qbaData ) );
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgProcessCmd::slot_ProcessFinished()
{
    QString qsFinishMessage = QString( "Process %1" ).arg( m_bProcessHalted?"halted by user":"finished" );

    _appendStdOut( QString("\n----------------------------------------------------------\n"
                           ">> %1 <<\n"
                           "----------------------------------------------------------\n").arg(qsFinishMessage) );
    _enableCommands( false );
    m_bProcessHalted = false;
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgProcessCmd::_setProcessPath()
{
    m_qsProcessPath = QString( "%1:%2" ).arg(ui->cmbDrive->currentText()).arg(ui->ledProcessPath->text());
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgProcessCmd::_enableCommands(bool p_bEnabled)
{
    ui->cmbDrive->setEnabled( !p_bEnabled );
    ui->pbChangeDir->setEnabled( !p_bEnabled );
    ui->chkSavePath->setEnabled( !p_bEnabled );
    ui->chkSaveProcess->setEnabled( !p_bEnabled );
    ui->cmbCommand->setEnabled( !p_bEnabled );
    ui->pbProcess->setEnabled( !p_bEnabled );
    ui->pbExitDlg->setEnabled( !p_bEnabled );

    ui->cmbMessage->setEnabled( p_bEnabled );
    ui->pbEnterMsg->setEnabled( p_bEnabled );
    ui->pbExit->setEnabled( p_bEnabled );
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgProcessCmd::_appendStdOut(const QString &p_qsText)
{
    if( p_qsText.length() < 1 )
        return;

    QString     qsText  = ui->ptStdOut->toPlainText();

    qsText.append( p_qsText );
    ui->ptStdOut->clear();
    ui->ptStdOut->setPlainText( qsText );
    ui->ptStdOut->verticalScrollBar()->setValue( ui->ptStdOut->verticalScrollBar()->maximum() );
}
//============================================================================================================
