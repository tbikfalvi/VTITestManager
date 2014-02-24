//============================================================================================================
//
//  VTI Test Manager application
//
//  File            : dlgpreferences.cpp
//  File version    : 1.0
//
//============================================================================================================

#include <QSettings>
#include <QMessageBox>
#include <QFileDialog>

//============================================================================================================

#include "dlgpreferences.h"
#include "ui_dlgpreferences.h"
#include "tmpreferences.h"

//============================================================================================================
//  dlgPreferences
//------------------------------------------------------------------------------------------------------------
//  Constructor of dialog class
//============================================================================================================
dlgPreferences::dlgPreferences(QWidget *parent) : QDialog(parent), ui(new Ui::dlgPreferences)
{
    ui->setupUi(this);

    setWindowTitle( QString("Manage application preferences") );

    bSettingsChanged = true;

    connect( ui->ledDirAIF, SIGNAL(textChanged(QString)), this, SLOT(slotSettingsChanged()) );
    connect( ui->ledDirAIFTestConfig, SIGNAL(textChanged(QString)), this, SLOT(slotSettingsChanged()) );
    connect( ui->ledDirLocalReleases, SIGNAL(textChanged(QString)), this, SLOT(slotSettingsChanged()) );
    connect( ui->ledDirRemoteReleases, SIGNAL(textChanged(QString)), this, SLOT(slotSettingsChanged()) );
    connect( ui->ledDirCycler, SIGNAL(textChanged(QString)), this, SLOT(slotSettingsChanged()) );
    connect( ui->ledDirCyclerTestConfig, SIGNAL(textChanged(QString)), this, SLOT(slotSettingsChanged()) );
    connect( ui->rbEnvCell, SIGNAL(clicked()), this, SLOT(slotEnvironmentChanged()) );
    connect( ui->rbEnvMinisystem, SIGNAL(clicked()), this, SLOT(slotEnvironmentChanged()) );
    connect( ui->rbEnvStandalone, SIGNAL(clicked()), this, SLOT(slotEnvironmentChanged()) );
    connect( ui->rbEnvDesktop, SIGNAL(clicked()), this, SLOT(slotEnvironmentChanged()) );
    connect( ui->ledAIFTestcaseTypes, SIGNAL(textChanged(QString)), this, SLOT(slotSettingsChanged()) );
    connect( ui->ledCyclerTestcaseTypes, SIGNAL(textChanged(QString)), this, SLOT(slotSettingsChanged()) );

    _loadSettings();

    bSettingsChanged = false;

    QSettings   iniFile( "VTIAppManager.ini", QSettings::IniFormat );
    int         dlgWidth    = iniFile.value( "Dialogs/Preferences_width", 526 ).toInt();
    int         dlgHeight   = iniFile.value( "Dialogs/Preferences_height", 467 ).toInt();
    QPoint      qpDlgSize   = QPoint( dlgWidth, dlgHeight );

    resize( qpDlgSize.x(), qpDlgSize.y() );
}

dlgPreferences::~dlgPreferences()
{
    QSettings   iniFile( "VTIAppManager.ini", QSettings::IniFormat );

    iniFile.setValue( "Dialogs/ProcessCommand_width", width() );
    iniFile.setValue( "Dialogs/ProcessCommand_height", height() );

    delete ui;
}

void dlgPreferences::slotSettingsChanged()
{
    if( bSettingsChanged )
        return;

    bSettingsChanged = true;
    ui->pbApply->setEnabled( true );
}

void dlgPreferences::slotEnvironmentChanged()
{
    QMessageBox::warning( this, tr("Warning"),
                          tr("Please change this setting only if you exactly know what are you doing.") );

    slotSettingsChanged();
}

void dlgPreferences::on_pbDirAIFSelect_clicked()
{
    QString qsDirectory = QFileDialog::getExistingDirectory(this, tr("Select the desired directory"), ui->ledDirAIF->text() );

    if (!qsDirectory.isEmpty())
    {
        ui->ledDirAIF->setText( qsDirectory );
    }
}

void dlgPreferences::on_pbDirAIFTestConfigSelect_clicked()
{
    QString qsDirectory = QFileDialog::getExistingDirectory(this, tr("Select the desired directory"), ui->ledDirAIFTestConfig->text() );

    if (!qsDirectory.isEmpty())
    {
        ui->ledDirAIFTestConfig->setText( qsDirectory );
    }
}

void dlgPreferences::on_pbDirLocalReleasesSelect_clicked()
{
    QString qsDirectory = QFileDialog::getExistingDirectory(this, tr("Select the desired directory"), ui->ledDirLocalReleases->text() );

    if (!qsDirectory.isEmpty())
    {
        ui->ledDirLocalReleases->setText( qsDirectory );
    }
}

void dlgPreferences::on_pbDirCyclerSelect_clicked()
{
    QString qsDirectory = QFileDialog::getExistingDirectory(this, tr("Select the desired directory"), ui->ledDirCycler->text() );

    if (!qsDirectory.isEmpty())
    {
        ui->ledDirCycler->setText( qsDirectory );
    }
}

void dlgPreferences::on_pbDirCyclerTestConfigSelect_clicked()
{
    QString qsStartDir = ( ui->ledDirCyclerTestConfig->text().contains("<CYCLER_VERSION>")?ui->ledDirCycler->text():ui->ledDirCyclerTestConfig->text() );
    QString qsDirectory = QFileDialog::getExistingDirectory(this, tr("Select the desired directory"), qsStartDir );

    if (!qsDirectory.isEmpty())
    {
        ui->ledDirCyclerTestConfig->setText( qsDirectory );
    }
}

void dlgPreferences::on_pbDirCyclerTestConfigDerive_clicked()
{
    ui->ledDirCyclerTestConfig->setText( QString("%1/<CYCLER_VERSION>/test/engine").arg(ui->ledDirCycler->text()) );
}

void dlgPreferences::on_pbApply_clicked()
{
    ui->pbApply->setEnabled( false );
    bSettingsChanged = false;
    _saveSettings();
}

void dlgPreferences::on_pbClose_clicked()
{
    if( bSettingsChanged )
    {
        if( QMessageBox::question( this, tr("Question"),
                                   tr("Some of the settings has been changed.\n"
                                      "Do you want to save changes?"),
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
        {
            on_pbApply_clicked();
        }
    }

    QDialog::accept();
}

void dlgPreferences::_loadSettings()
{
    QString qsDirAIF            = QString("C:/Program Files/GE Medical Systems/Testinfra/AutomationIF");
    QString qsDirConfAIF        = QString("%1/test2").arg(qsDirAIF);
    QString qsDirLocalReleases  = QString("D:/Users/Releases");
    QString qsDirRemoteReleases = QString("\\\\med.ge.com\\fs\\emea\\DI\\Budapest\\Eng\\Vasc\\vacas\\VTI\\releases\\");
    QString qsDirCycler         = QString("C:/cycler_liberty");
    QString qsDirConfCycler     = QString("%1/<CYCLER_VERSION>/test/engine").arg(qsDirCycler);

    QSettings   iniFile( "VTIAppManager.ini", QSettings::IniFormat );

    ui->ledDirAIF->setText( iniFile.value( "Test_Config/AIFDirectory", qsDirAIF ).toString() );
    ui->ledDirAIF->setToolTip( ui->ledDirAIF->text() );

    ui->ledDirAIFTestConfig->setText( iniFile.value( "Test_Config/AIFTestDirectory", qsDirConfAIF ).toString() );
    ui->ledDirAIFTestConfig->setToolTip( ui->ledDirAIFTestConfig->text() );

    ui->ledDirLocalReleases->setText( iniFile.value( "Releases/LocalDirectory", qsDirLocalReleases ).toString() );
    ui->ledDirLocalReleases->setToolTip( ui->ledDirLocalReleases->text() );
    ui->ledDirRemoteReleases->setText( iniFile.value( "Releases/RemoteDirectory", qsDirRemoteReleases ).toString() );

    ui->ledDirCycler->setText( iniFile.value( "Test_Config/CyclerDirectory", qsDirCycler ).toString() );
    ui->ledDirCycler->setToolTip( ui->ledDirCycler->text() );

    ui->ledDirCyclerTestConfig->setText( iniFile.value( "Test_Config/CyclerTestDirectory", qsDirConfCycler ).toString() );
    ui->ledDirCyclerTestConfig->setToolTip( ui->ledDirCyclerTestConfig->text() );

    if( tgPrefs::instance().isRunningOnDevelopmentPC() )
    {
        ui->rbEnvDesktop->setChecked( true );
    }
    else
    {
        QSettings   regHKLM_Testinfra( "HKEY_LOCAL_MACHINE\\SOFTWARE\\GE Medical Systems\\Testinfra", QSettings::NativeFormat );

        int nRunMode = regHKLM_Testinfra.value( "AIF/RunMode", tgPrefs::instance().ENV_MINISYSTEM ).toInt();

        if( nRunMode == tgPrefs::instance().ENV_CELL )
        {
            ui->rbEnvCell->setChecked( true );
        }
        else if( nRunMode == tgPrefs::instance().ENV_MINISYSTEM )
        {
            ui->rbEnvMinisystem->setChecked( true );
        }
        else if( nRunMode == tgPrefs::instance().ENV_STANDALONE )
        {
            ui->rbEnvStandalone->setChecked( true );
        }
    }

    ui->ledAIFTestcaseTypes->setText( tgPrefs::instance().testcaseTypes(tgPrefs::instance().TC_AIF).join(QChar('#')) );
    ui->ledCyclerTestcaseTypes->setText( tgPrefs::instance().testcaseTypes(tgPrefs::instance().TC_CYCLER).join(QChar('#')) );

    QStringList qslCommands = tgPrefs::instance().commands();

    for( int i=0; i<qslCommands.count(); i++ )
    {
        ui->listCommands->addItem( qslCommands.at(i) );
    }
}

void dlgPreferences::_saveSettings()
{
    if( ui->rbEnvDesktop->isChecked() )
        tgPrefs::instance().setRunningOnDevelopmentPC( true );
    else
        tgPrefs::instance().setRunningOnDevelopmentPC( false );

    tgPrefs::instance().setDirAIF( ui->ledDirAIF->text() );
    tgPrefs::instance().setDirAIFConf( ui->ledDirAIFTestConfig->text() );
    tgPrefs::instance().setDirCycler( ui->ledDirCycler->text() );
    tgPrefs::instance().setDirCyclerConf( ui->ledDirCyclerTestConfig->text() );
    tgPrefs::instance().setDirLocalReleases( ui->ledDirLocalReleases->text() );
    tgPrefs::instance().setDirRemoteReleases( ui->ledDirRemoteReleases->text() );

    tgPrefs::instance().setTestcaseType( tgPrefs::instance().TC_AIF, ui->ledAIFTestcaseTypes->text().split(QChar('#')) );
    tgPrefs::instance().setTestcaseType( tgPrefs::instance().TC_CYCLER, ui->ledCyclerTestcaseTypes->text().split(QChar('#')) );

    tgPrefs::instance().save();
}

void dlgPreferences::on_pbSave_clicked()
{
    if( ui->pbSave->text().compare(tr("Add")) == 0 )
    {
        tgPrefs::instance().addCommand( ui->ledCommand->text() );
        ui->listCommands->addItem( ui->ledCommand->text() );
    }
    else if( ui->pbSave->text().compare(tr("Modify")) == 0 )
    {
        int nPos = ui->listCommands->currentRow();

        ui->listCommands->takeItem( nPos );
        ui->listCommands->insertItem( nPos, ui->ledCommand->text() );
        tgPrefs::instance().deleteCommands();
        for( int i=0; i<ui->listCommands->count(); i++ )
        {
            tgPrefs::instance().addCommand( ui->listCommands->item(i)->text() );
        }
    }
    slotSettingsChanged();
}

void dlgPreferences::on_pbModify_clicked()
{
    ui->ledCommand->setText( ui->listCommands->item(ui->listCommands->currentRow())->text() );
    ui->pbSave->setText( tr("Modify") );
}

void dlgPreferences::on_pbDelete_clicked()
{
    ui->listCommands->takeItem( ui->listCommands->currentRow() );
    tgPrefs::instance().deleteCommands();
    for( int i=0; i<ui->listCommands->count(); i++ )
    {
        tgPrefs::instance().addCommand( ui->listCommands->item(i)->text() );
    }
    slotSettingsChanged();
}

void dlgPreferences::_enableCommandButtons(bool p_bEnable)
{
    ui->pbModify->setEnabled( p_bEnable );
    ui->pbDelete->setEnabled( p_bEnable );
}

void dlgPreferences::on_pbAdd_clicked()
{
    ui->pbSave->setText( tr("Add") );
    ui->ledCommand->setText( "" );
}

void dlgPreferences::on_listCommands_itemSelectionChanged()
{
    if( ui->listCommands->selectedItems().count() > 0 )
    {
        _enableCommandButtons( true );
    }
    else
    {
        _enableCommandButtons( false );
    }
}
