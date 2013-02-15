//============================================================================================================
//
//  VTI Test Manager application
//
//  File            : dlgmanageautotests.cpp
//  File version    : 1.0
//
//============================================================================================================

#include <QProcess>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>

//============================================================================================================

#include "dlgmanageautotests.h"
#include "ui_dlgmanageautotests.h"

//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
dlgManageAutoTests::dlgManageAutoTests(QWidget *parent, QString p_qsSystem, QString p_qsDir, QString p_qsDirConf) : QDialog(parent), ui(new Ui::dlgManageAutoTests)
{
    ui->setupUi(this);

    setWindowTitle( QString("Manage %1 ATP test").arg(p_qsSystem) );

    if( p_qsSystem.compare("AIF") == 0 )
    {
        setWindowIcon( QIcon(":/images/config_aif.png") );
        m_tcSystemType = tgPrefs::instance().TC_AIF;
    }
    else
    {
        setWindowIcon( QIcon(":/images/config_cycler.png") );
        m_tcSystemType = tgPrefs::instance().TC_CYCLER;
    }

    m_qsDir     = p_qsDir;
    m_qsDirConf = p_qsDirConf;

    m_dlgWarningBox = new cDlgWarningBox( this );
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
dlgManageAutoTests::~dlgManageAutoTests()
{
    delete m_dlgWarningBox;
    delete ui;
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgManageAutoTests::on_pbExit_clicked()
{
    if( ui->ptComment->toPlainText().length() > 0 )
    {
        if( QMessageBox::question( this, tr("Question"),
                                   tr("The comment field is not empty.\n"
                                      "Do you want to save the entered text\n"
                                      "into a text file?"),
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
        {
            QString qsFileName = QFileDialog::getSaveFileName( this, tr("Save comment to ...") );
            if( qsFileName.length() > 0 )
            {
                QFile   qfFile( qsFileName );

                if( qfFile.open( QIODevice::WriteOnly|QIODevice::Text ) )
                {
                    qfFile.write( ui->ptComment->toPlainText().toStdString().c_str() );
                    qfFile.close();
                }
            }
        }
    }
    m_dlgWarningBox->hideWarningBox();
    QDialog::close();
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgManageAutoTests::on_pbExecute_clicked()
{
    QString m_qsCurrentPath = QDir::currentPath();

    if( QMessageBox::question( this, tr("Question"),
                               tr( "Is the system is ready to process the %1 automated test?").arg(m_tcSystemType==tgPrefs::instance().TC_AIF?"AIF":"Cycler"),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        _executeProcess();
    }

    QDir::setCurrent( m_qsCurrentPath );
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgManageAutoTests::_executeProcess()
{
    QProcess *qpATP = new QProcess(this);
    QString qsATP = QString("itkwish30 %1").arg( m_tcSystemType==tgPrefs::instance().TC_AIF?"aifATPv2.0.tcl":"ATP.tcl" );

    QString qsDirATP = ( m_tcSystemType==tgPrefs::instance().TC_AIF ? m_qsDirConf : m_qsDirConf.remove("/test/engine") );

    QDir::setCurrent( qsDirATP );

    if( !qpATP->startDetached( qsATP ) )
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("Error occured when starting process:\n\n%1\n\nError code: %2\n"
                                 "0 > The process failed to start.\n"
                                 "1 > The process crashed some time after starting successfully.\n"
                                 "2 > The last waitFor...() function timed out.\n"
                                 "4 > An error occurred when attempting to write to the process.\n"
                                 "3 > An error occurred when attempting to read from the process.\n"
                                 "5 > An unknown error occurred.").arg(qsATP).arg(qpATP->error()) );
        m_dlgWarningBox->hideWarningBox();
    }
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgManageAutoTests::on_chkShowWarning_clicked(bool checked)
{
    cDlgWarningBox::showPosition    spPos = cDlgWarningBox::POS_DEFAULT;

    if( ui->rbShowWarningTop->isChecked() )
        spPos = cDlgWarningBox::POS_TOP;
    else if( ui->rbShowWarningBottom->isChecked() )
        spPos = cDlgWarningBox::POS_BOTTOM;

    if( checked )
    {
        m_dlgWarningBox->showWarningBox( spPos );
    }
    else
    {
        m_dlgWarningBox->hideWarningBox();
    }
}
//============================================================================================================
