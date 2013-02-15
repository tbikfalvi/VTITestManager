//============================================================================================================
//
//  VTI Test Manager application
//
//  File            : dlgtestinfraregistry.cpp
//  File version    : 1.0
//
//============================================================================================================

#include <QFileDialog>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QDateTime>
#include <QProcess>
#include <QDir>
#include <QDomDocument>

//============================================================================================================

#include "tracer.h"
#include "dlgconfigeditor.h"
#include "ui_dlgconfigeditor.h"

//============================================================================================================
// dlgConfigEditor
//------------------------------------------------------------------------------------------------------------
// Constructor of the config editor
//------------------------------------------------------------------------------------------------------------
// Parameters
//  parent          - handler of the caller window
//  p_qsSystem      - identifies which system is selected; value can be "AIF" or "Cycler"
//  p_qsDir         - contains the test home directory of selected system
//                    default value can be set in preferences dialog
//  p_qsDirConf     - contains the directory where the config file of the selected system can be found
//                    default value can be set in preferences dialog
//============================================================================================================
dlgConfigEditor::dlgConfigEditor(QWidget *parent, QString p_qsSystem, QString p_qsDir, QString p_qsDirConf ) : QDialog(parent), ui(new Ui::dlgConfigEditor)
{
    cTracer tracer("dlgConfigEditor");

    // Initialize the GUI controls
    ui->setupUi(this);

    // Initialize the progress dialog
    m_dlgProgress = new cDlgProgress( this );

    // Set the window title based on the selected system's name
    setWindowTitle( QString("Manage %1 ATP test").arg(p_qsSystem) );

    // Initialize controls and settings based on the selected system's name
    if( p_qsSystem.compare("AIF") == 0 )
    {
        setWindowIcon( QIcon(":/images/config_aif.png") );
        m_tcSystemType = tgPrefs::instance().TC_AIF;
        ui->chkOnlyTP->setEnabled( true );
        ui->ledReleaseVersion->setText( tgPrefs::instance().regAIFVersion() );
    }
    else
    {
        setWindowIcon( QIcon(":/images/config_cycler.png") );
        m_tcSystemType = tgPrefs::instance().TC_CYCLER;
        ui->chkOnlyTP->setEnabled( false );
        ui->ledReleaseVersion->setText( _getCyclerVersionFromPath(p_qsDirConf) );
    }

    // Set private members
    m_qsDir     = p_qsDir;
    m_qsDirConf = p_qsDirConf;

    m_qsFileName = QString( "%1/%2" ).arg(m_qsDirConf).arg(ui->ledFileName->text());

    ui->ledFileName->setToolTip( m_qsFileName );

    ui->ledTestReportRevision->setText( tgPrefs::instance().testReportRevision() );
    ui->ledTesterName->setText( tgPrefs::instance().testerName() );
    ui->ledEnvironmentName->setText( tgPrefs::instance().testEnvironment() );
    ui->ledITUAddress->setText( (tgPrefs::instance().regITUAddress().compare(tr("_not_defined_"))==0?"0":tgPrefs::instance().regITUAddress()) );
}
//============================================================================================================
// ~dlgConfigEditor
//------------------------------------------------------------------------------------------------------------
// Destructor
//============================================================================================================
dlgConfigEditor::~dlgConfigEditor()
{
    cTracer tracer("~dlgConfigEditor");

    tgPrefs::instance().save();

    delete m_dlgProgress;
    delete ui;
}
//============================================================================================================
// on_pbChangeFile_clicked
//------------------------------------------------------------------------------------------------------------
// Activated when Change filename button clicked
// Opens a QT File Open dialog. If user select a file and click on Open the newly selected file will be
// the target file
//============================================================================================================
void dlgConfigEditor::on_pbChangeFile_clicked()
{
    cTracer tracer("on_pbChangeFile_clicked");

    QString qsFileName = QFileDialog::getOpenFileName(this, tr("Open Config File"), m_qsDirConf, tr("All Files (*.*)"));

    if( qsFileName.length() > 0 )
    {
        ui->ledFileName->setText( QFileInfo(qsFileName).fileName() );
        ui->ledFileName->setToolTip( qsFileName );
        m_qsDirConf = QFileInfo(qsFileName).path();
    }
}
//============================================================================================================
// on_pbResetFileName_clicked
//------------------------------------------------------------------------------------------------------------
// Activated when Reset filename button clicked
// The target file set back to ATP.config
//============================================================================================================
void dlgConfigEditor::on_pbResetFileName_clicked()
{
    cTracer tracer("on_pbResetFileName_clicked");

    ui->ledFileName->setText( "ATP.config" );
}
//============================================================================================================
// on_pbProcessConfig_clicked
//------------------------------------------------------------------------------------------------------------
// Activated when Process config button clicked
// Read the target config file into memory, process the content of the file, fill the test item listbox
// based on the content of the target config file and finally fill the listbox contains the list of the
// comments (the comments comes from config file, but can be edited in the preferences dialog)
//============================================================================================================
void dlgConfigEditor::on_pbProcessConfig_clicked()
{
    cTracer tracer("on_pbProcessConfig_clicked");

    m_bSilentMode   = false;
    m_bOnlyTP       = false;

    m_dlgProgress->showProgress();
    _readConfigFile();
    _fillListView();
    _fillListTypes();
    m_dlgProgress->hideProgress();

    ui->chkSilent->setChecked( m_bSilentMode );
    ui->chkOnlyTP->setChecked( m_bOnlyTP );
}
//============================================================================================================
// on_listTestTypes_itemClicked
//------------------------------------------------------------------------------------------------------------
// Activated when user clicks on the listbox contains comments
// If the actual comment became selected, all of the test item marked with comment will be selected
// if the actual comment became deselected, all of the test item marked with comment will be deselected
//============================================================================================================
void dlgConfigEditor::on_listTestTypes_itemClicked(QListWidgetItem *item)
{
    cTracer tracer("on_listTestTypes_itemClicked");

    bool    bSelection = item->isSelected();
    QString qsItem = item->text();

    for( int i=0; i<ui->listTestElements->count(); i++ )
    {
        QString qsComment = ui->listTestElements->item(i)->text();

        if( qsComment.contains(QString("#")) )
        {
            qsComment = qsComment.right( qsComment.length() - qsComment.indexOf("#") );

            if( qsComment.contains(qsItem) )
            {
                ui->listTestElements->item(i)->setSelected( bSelection );
            }
        }
    }
}
//============================================================================================================
// on_pbExit_clicked
//------------------------------------------------------------------------------------------------------------
// Activated when Exit button clicked
// Closes the dialog without any further action
//============================================================================================================
void dlgConfigEditor::on_pbExit_clicked()
{
    cTracer tracer("on_pbExit_clicked");

    QDialog::close();
}
//============================================================================================================
// on_pbSave_clicked
//------------------------------------------------------------------------------------------------------------
// Activated when Save button clicked
// Save the test items status into the target config file
//============================================================================================================
void dlgConfigEditor::on_pbSave_clicked()
{
    cTracer tracer("on_pbSave_clicked");

    if( QMessageBox::question( this, tr("Question"),
                               tr("Are you sure you want to update the config file\n"
                                  "with the settings defined in the test items listbox?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        m_dlgProgress->showProgress();
        _saveConfigSettings();
        m_dlgProgress->hideProgress();
    }
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgConfigEditor::on_pbSelectAll_clicked()
{
    for( int i=0; i<ui->listTestTypes->count(); i++ )
    {
        ui->listTestTypes->item(i)->setSelected( true );
    }
    for( int i=0; i<ui->listTestElements->count(); i++ )
    {
        ui->listTestElements->item(i)->setSelected( true );
    }
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgConfigEditor::on_pbDeselectAll_clicked()
{
    for( int i=0; i<ui->listTestTypes->count(); i++ )
    {
        ui->listTestTypes->item(i)->setSelected( false );
    }
    for( int i=0; i<ui->listTestElements->count(); i++ )
    {
        ui->listTestElements->item(i)->setSelected( false );
    }
}
//============================================================================================================
// _readConfigFile
//------------------------------------------------------------------------------------------------------------
// Read target config file
//============================================================================================================
void dlgConfigEditor::_readConfigFile()
{
    cTracer tracer("_readConfigFile");

    m_qsFileName = QString( "%1/%2" ).arg(m_qsDirConf).arg(ui->ledFileName->text());
    ui->ledFileName->setToolTip( m_qsFileName );

    QFile   fileConfig( m_qsFileName );

    m_vTestCases.clear();

    if( !fileConfig.open(QIODevice::ReadOnly | QIODevice::Text) )
        return;

    QTextStream in(&fileConfig);
    QStringList qslTestCase;

    QString line = in.readLine();

    if( line.contains("<atp_config") )
    {
        m_bConfigXML = true;
        fileConfig.close();
        _readConfigXml( m_qsFileName );
    }
    else
    {
        m_bConfigXML = false;
        while( !in.atEnd() )
        {
            if( line.contains( "<testproc>" ) )
            {
                if( qslTestCase.count() > 0 )
                {
                    m_vTestCases.append( qslTestCase );
                    qslTestCase.clear();
                }
                qslTestCase << _addTestProcedure( line );
            }
            else if( line.contains( "<testcase>" ) )
            {
                qslTestCase << _addTestProcedure( line );
            }

            line = in.readLine();
        }

        if( qslTestCase.count() > 0 )
        {
            m_vTestCases.append( qslTestCase );
        }

        fileConfig.close();
    }
}
//============================================================================================================
// _fillListView
//------------------------------------------------------------------------------------------------------------
// Fills the listbox contains the test items base on the content of the target config file
//============================================================================================================
void dlgConfigEditor::_fillListView()
{
    cTracer tracer("_fillListView");

    ui->listTestElements->clear();

    for( int i=0; i<m_vTestCases.count(); i++ )
    {
        QStringList qslTest = m_vTestCases.at(i);
        QStringList qslTestCase = qslTest.at(0).split(QChar('|'));

        QString qsName      = qslTestCase.at(0);
        QString qsTitle     = ( qslTestCase.count()>3 ? QString(" (%1) ").arg(qslTestCase.at(3)) : "" );
        QString qsComment   = QString( qslTestCase.at(2).length()>0 ? QString("%1").arg(qslTestCase.at(2)) : "" ).remove(QChar('#')).trimmed();

        if( qsComment.length() > 0 )
        {
            if( qsComment.contains( "," ) )
            {
                QStringList qslTemp = qsComment.split( "," );
                for( int ci=0; ci<qslTemp.count(); ci++ )
                {
                    tgPrefs::instance().addTestcaseType( m_tcSystemType, qslTemp.at(ci) );
                }
            }
            else
            {
                tgPrefs::instance().addTestcaseType( m_tcSystemType, qsComment );
            }
        }

        ui->listTestElements->addItem( QString("%1%2%3").arg(qsName).arg(qsTitle).arg(qsComment.length()>0?QString(" #%1").arg(qsComment):"") );

        if( qslTestCase.at(1).compare("1") == 0 )
        {
            ui->listTestElements->item( ui->listTestElements->count()-1 )->setSelected( true );
        }

        for( int j=1; j<qslTest.count(); j++ )
        {
            qslTestCase = qslTest.at(j).split(QChar('|'));

            QString qsName      = qslTestCase.at(0);
            QString qsComment   = QString( qslTestCase.at(2).length()>0 ? QString("%1").arg(qslTestCase.at(2)) : "" ).remove(QChar('#')).trimmed();

            if( qsComment.length() > 0 )
            {
                tgPrefs::instance().addTestcaseType( m_tcSystemType, qsComment );
            }

            ui->listTestElements->addItem( QString("\t%1%2").arg(qsName).arg(qsComment.length()>0?QString(" #%1").arg(qsComment):"") );

            if( qslTestCase.at(1).compare("1") == 0 )
            {
                ui->listTestElements->item( ui->listTestElements->count()-1 )->setSelected( true );
            }
        }
    }
}
//============================================================================================================
// _fillListTypes
//------------------------------------------------------------------------------------------------------------
// Fills the listbox contains the comments base on the content of the target config file
//============================================================================================================
void dlgConfigEditor::_fillListTypes()
{
    cTracer tracer("_fillListTypes");

    ui->listTestTypes->clear();

    QStringList qslTestcaseTypes = tgPrefs::instance().testcaseTypes( m_tcSystemType );

    for( int i=0; i<qslTestcaseTypes.count(); i++ )
    {
        ui->listTestTypes->addItem( qslTestcaseTypes.at(i) );

        for( int j=0; j<ui->listTestElements->selectedItems().count(); j++ )
        {
            QString qsItem = ui->listTestElements->selectedItems().at(j)->text();

            if( qsItem.contains(qslTestcaseTypes.at(i)) )
            {
                ui->listTestTypes->item( ui->listTestTypes->count()-1 )->setSelected( true );
            }
        }
    }
}
//============================================================================================================
// _addTestProcedure
//------------------------------------------------------------------------------------------------------------
// Translate the test item format defined in config file to '|' separated string
//============================================================================================================
QString dlgConfigEditor::_addTestProcedure(const QString &p_qsLine)
{
    cTracer tracer("_addTestProcedure");

    QString     qsLine = p_qsLine.mid( p_qsLine.indexOf(">")+1, p_qsLine.length()-p_qsLine.indexOf(">")-1 ).trimmed();
    QString     qsName = qsLine.left( qsLine.indexOf(" ") );

    qsLine = qsLine.remove( qsName ).trimmed();

    QString     qsEnabled = qsLine.left( 1 );
    QString     qsComment = ( qsLine.indexOf("#")!=-1 ? qsLine.right(qsLine.length()-qsLine.indexOf("#")) : "" );

    return QString("%1|%2|%3").arg(qsName).arg(qsEnabled).arg(qsComment);
}
//============================================================================================================
// _saveConfigSettings
//------------------------------------------------------------------------------------------------------------
// Overwrite the target config file based on the selections in the test item listbox
//============================================================================================================
void dlgConfigEditor::_saveConfigSettings()
{
    cTracer tracer("_saveConfigSettings");

    tgPrefs::instance().setTestReportRevision( ui->ledTestReportRevision->text() );
    tgPrefs::instance().setTesterName( ui->ledTesterName->text() );
    tgPrefs::instance().setTestEnvironment( ui->ledEnvironmentName->text() );

    QFile   fileConfig( m_qsFileName );

    fileConfig.copy( QString( "%1/ATP_backup_%2.config" ).arg(m_qsDirConf).arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")) );

    fileConfig.open( QIODevice::WriteOnly );

    if( m_bConfigXML )
    {
        fileConfig.write( "<atp_config version=\"2.0\">\n\n    <header>\n" );
        fileConfig.write( QString("        <setting fail_silent=\"%1\" />\n").arg((ui->chkSilent->isChecked()?"1":"0")).toStdString().c_str() );
        if( m_tcSystemType == tgPrefs::instance().TC_AIF )
        {
            fileConfig.write( QString("        <setting only_tp=\"%1\" />\n").arg((ui->chkOnlyTP->isChecked()?"1":"0")).toStdString().c_str() );
        }
        fileConfig.write( QString("        <setting system_release=\"%1\" />\n").arg(ui->ledReleaseVersion->text()).toStdString().c_str() );
        fileConfig.write( QString("        <setting testreport_revision=\"%1\" />\n").arg(ui->ledTestReportRevision->text()).toStdString().c_str() );
        fileConfig.write( QString("        <setting tester_name=\"%1\" />\n").arg(ui->ledTesterName->text()).toStdString().c_str() );
        fileConfig.write( QString("        <setting test_environment=\"%1\" />\n").arg(ui->ledEnvironmentName->text()).toStdString().c_str() );
        fileConfig.write( QString("        <setting itu_address=\"%1\" />\n").arg(ui->ledITUAddress->text()).toStdString().c_str() );
        fileConfig.write( "    </header>\n\n    <testcases>\n" );

        bool    bTestItemStarted = false;

        for( int i=0; i<ui->listTestElements->count(); i++ )
        {
            QString qsLine      = ui->listTestElements->item(i)->text();
            //QStringList qslItem = ui->listTestElements->item(i)->text().split( QChar(' ') );
            QString qsEnabled   = ( ui->listTestElements->selectedItems().contains(ui->listTestElements->item(i)) ? "1" : "0" );

            QString qsName      = qsLine.left( qsLine.indexOf(QChar(' ')) ); //qslItem.at(0);
            QString qsTitle     = ( !qsName.contains("\t") ? qsLine.mid( qsLine.indexOf(QChar('('))+1, (qsLine.indexOf(QChar(')'))-qsLine.indexOf(QChar('('))-1) ) : "" );
            QString qsComment   = ( qsLine.contains(QChar('#')) ? qsLine.right( qsLine.length()-qsLine.indexOf(QChar('#')) ) : "" ); //QString( ( qslItem.count()>1 ? qslItem.at(1) : "" ) ).remove("#");

            if( qsName.contains("\t") )
            {
                qsLine = QString( "            <testproc name=\"%1\" execute=\"%2\" flags=\"%3\" />" ).arg(qsName.remove("\t")).arg(qsEnabled).arg(qsComment.remove("#"));
            }
            else
            {
                if( bTestItemStarted )
                {
                    fileConfig.write( "\n        </testitem>\n" );
                }
                qsLine = QString( "\n        <testitem name=\"%1\" filename=\"%2\" execute=\"%3\" flags=\"%4\">\n" ).arg(qsTitle).arg(qsName).arg(qsEnabled).arg(qsComment.remove("#"));
                bTestItemStarted = true;
            }

            fileConfig.write( QString("%1\n").arg(qsLine).toStdString().c_str() );
        }
        if( bTestItemStarted )
        {
            fileConfig.write( "\n        </testitem>\n" );
        }
        fileConfig.write( "\n    </testcases>\n\n</atp_config>\n" );
        fileConfig.write( QString("\n").toStdString().c_str() );
    }
    else
    {
        fileConfig.write( QString("<fail-silent> %1\n").arg((ui->chkSilent->isChecked()?"1":"0")).toStdString().c_str() );
        if( m_tcSystemType == tgPrefs::instance().TC_AIF )
        {
            fileConfig.write( QString("<onlyTP> %1\n").arg((ui->chkOnlyTP->isChecked()?"1":"0")).toStdString().c_str() );
        }

        for( int i=0; i<ui->listTestElements->count(); i++ )
        {
            QString qsLine;
            QStringList qslItem = ui->listTestElements->item(i)->text().split( QChar(' ') );

            if( qslItem.at(0).contains("\t") )
            {
                qsLine += QString("    <testcase> %1").arg( qslItem.at(0) ).remove("\t");
            }
            else
            {
                qsLine += QString("\n<testproc> %1").arg( qslItem.at(0) );
            }

            if( ui->listTestElements->selectedItems().contains( ui->listTestElements->item(i) ) )
            {
                qsLine += QString(" 1 ");
            }
            else
            {
                qsLine += QString(" 0 ");
            }

            if( qslItem.count() > 1 )
            {
                qsLine += QString("      %1").arg(qslItem.at(1));
            }

            fileConfig.write( QString("%1\n").arg(qsLine).toStdString().c_str() );
        }
        fileConfig.write( QString("\n").toStdString().c_str() );
    }

    fileConfig.close();
}
//============================================================================================================
// _readConfigXml
//------------------------------------------------------------------------------------------------------------
// Reads the target config file (the format of the file is XML)
//============================================================================================================
void dlgConfigEditor::_readConfigXml(QString p_qsFileName)
{
    cTracer tracer("_readConfigXml");

    QDomDocument doc( "atpconfig" );
    QFile file( p_qsFileName );

    if( !file.open(QIODevice::ReadOnly) )
        return;

    QString      qsErrorMsg  = "";
    int          inErrorLine = 0;
    file.seek( 0 );
    if( !doc.setContent( &file, &qsErrorMsg, &inErrorLine ) )
    {
        QMessageBox::warning( this, tr("Warning"), QString( "Parsing Batch file: %1 - Error in line %2: %3" ).arg( p_qsFileName ).arg( inErrorLine ).arg( qsErrorMsg ) );
        file.close();
        return;
    }
    file.close();

    QDomElement docRoot = doc.documentElement();

    QDomNodeList nodeList = docRoot.elementsByTagName("header").at(0).toElement().elementsByTagName("setting");

    for(int i = 0; i < nodeList.count(); i++ )
    {
        QString qsFailSilent( nodeList.at(i).toElement().attribute( "fail_silent", "" ) );
        if( qsFailSilent.length() )
        {
            m_bSilentMode = qsFailSilent.toInt();
            ui->chkSilent->setChecked( (qsFailSilent.compare("1")==0?true:false) );
        }
        QString qsOnlyTP( nodeList.at(i).toElement().attribute( "only_tp", "" ) );
        if( qsOnlyTP.length() )
        {
            m_bOnlyTP = qsOnlyTP.toInt();
            ui->chkOnlyTP->setChecked( (qsOnlyTP.compare("1")==0?true:false) );
        }
    }

    QStringList qslTestCase;

    QDomNodeList obTestItems = docRoot.elementsByTagName("testcases").at(0).toElement().elementsByTagName("testitem");

    for( int i = 0; i < obTestItems.count(); i++ )
    {
        QString qsName = obTestItems.at(i).toElement().attribute( "filename", "" );
        QString qsTitle = obTestItems.at(i).toElement().attribute( "name", "" );
        QString qsEnabled = obTestItems.at(i).toElement().attribute( "execute", "" );
        QString qsComment = obTestItems.at(i).toElement().attribute( "flags", "" );

        qslTestCase.clear();
        qslTestCase.append( QString("%1|%2|%3%4|%5").arg(qsName).arg(qsEnabled).arg((qsComment.length()>0?"#":"")).arg(qsComment).arg(qsTitle) );

        QDomNodeList obTestProcedures = obTestItems.at(i).toElement().elementsByTagName("testproc");

        for( int j=0; j < obTestProcedures.count(); j++ )
        {
            qsName = obTestProcedures.at(j).toElement().attribute("name", "");
            qsEnabled = obTestProcedures.at(j).toElement().attribute("execute", "");
            qsComment = obTestProcedures.at(j).toElement().attribute("flags", "");

            qslTestCase.append( QString("%1|%2|%3%4").arg(qsName).arg(qsEnabled).arg((qsComment.length()>0?"#":"")).arg(qsComment) );
        }

        m_vTestCases.append( qslTestCase );
    }
}
//============================================================================================================
//
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
QString dlgConfigEditor::_getCyclerVersionFromPath(const QString &p_qsPath) const
{
    QString qsRet = QString( p_qsPath ).remove( QString("/test/engine") );

    qsRet = qsRet.right( qsRet.length() - qsRet.lastIndexOf( "/" ) - 1 );

    return qsRet;
}
//============================================================================================================

