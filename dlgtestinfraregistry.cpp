//============================================================================================================
//
//  VTI Test Manager application
//
//  File            : dlgtestinfraregistry.cpp
//  File version    : 1.0
//
//============================================================================================================

#include <QSettings>
#include <QMessageBox>

//============================================================================================================

#include "dlgtestinfraregistry.h"
#include "ui_dlgtestinfraregistry.h"

//============================================================================================================
//  dlgTestinfraRegistry
//------------------------------------------------------------------------------------------------------------
//  Constructor of dialog class
//============================================================================================================
dlgTestinfraRegistry::dlgTestinfraRegistry(QWidget *parent) : QDialog(parent), ui(new Ui::dlgTestinfraRegistry)
{
    ui->setupUi(this);

    setMinimumSize( 472, 500 );
    setMaximumSize( 473, 500 );

    setWindowTitle( tr("Manage registry settings") );
    setWindowIcon( QIcon(":/images/regedit.png") );

    ui->pbSaveToIni->setEnabled( false );
    ui->pbWriteToRegistry->setEnabled( false );

    connect( ui->chkAwHostIp, SIGNAL(toggled(bool)), this, SLOT(slotEnableButtons()) );
    connect( ui->chkDlPcIp, SIGNAL(toggled(bool)), this, SLOT(slotEnableButtons()) );
    connect( ui->chkEngPcIp, SIGNAL(toggled(bool)), this, SLOT(slotEnableButtons()) );
    connect( ui->chkItuIp, SIGNAL(toggled(bool)), this, SLOT(slotEnableButtons()) );
    connect( ui->chkKeypadPort, SIGNAL(toggled(bool)), this, SLOT(slotEnableButtons()) );
    connect( ui->chkPosIp, SIGNAL(toggled(bool)), this, SLOT(slotEnableButtons()) );
    connect( ui->chkPosLogPort, SIGNAL(toggled(bool)), this, SLOT(slotEnableButtons()) );
    connect( ui->chkPosTableLogPort, SIGNAL(toggled(bool)), this, SLOT(slotEnableButtons()) );
    connect( ui->chkFootSwitchType, SIGNAL(toggled(bool)), this, SLOT(slotEnableButtons()) );
    connect( ui->chkITUVersion, SIGNAL(toggled(bool)), this, SLOT(slotEnableButtons()) );
    connect( ui->chkTableType, SIGNAL(toggled(bool)), this, SLOT(slotEnableButtons()) );

    _loadRegistrySettings();
    _loadBackupSettings();
}
//============================================================================================================
//  ~dlgTestinfraRegistry
//------------------------------------------------------------------------------------------------------------
//  Destructor of dialog class
//============================================================================================================
dlgTestinfraRegistry::~dlgTestinfraRegistry()
{
    delete ui;
}
//============================================================================================================
//  slotEnableButtons
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgTestinfraRegistry::slotEnableButtons()
{
    ui->pbSaveToIni->setEnabled( false );
    ui->pbWriteToRegistry->setEnabled( false );

    if( ui->chkAwHostIp->isChecked() ||
        ui->chkDlPcIp->isChecked() ||
        ui->chkEngPcIp->isChecked() ||
        ui->chkItuIp->isChecked() ||
        ui->chkKeypadPort->isChecked() ||
        ui->chkPosIp->isChecked() ||
        ui->chkPosLogPort->isChecked() ||
        ui->chkPosTableLogPort->isChecked() ||
        ui->chkFootSwitchType->isChecked() ||
        ui->chkITUVersion->isChecked() ||
        ui->chkTableType->isChecked() )
    {
        ui->pbSaveToIni->setEnabled( true );
        ui->pbWriteToRegistry->setEnabled( true );
    }
}
//============================================================================================================
//  on_pbReadFromRegistry_clicked
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgTestinfraRegistry::on_pbReadFromRegistry_clicked()
{
    _loadRegistrySettings();

    ui->ledAIFVersion->setText( m_qsRegAIFVersion );
    ui->ledInnovaProgram->setText( m_qsRegInnovaPrg );
    ui->ledFluoroPlus->setText( m_qsRegFPPrg );

    ui->ledEngPcIp->setText( m_qsRegEngPC_IP );
    ui->ledDlPcIp->setText( m_qsRegDL_IP );

    ui->ledAwHostIp->setText( m_qsRegAWHost );

    ui->ledKeypadPort->setText( m_qsRegKeypadPort );

    ui->ledPosIp->setText( m_qsRegPosIPAddress );
    ui->ledPosLogPort->setText( m_qsRegPosLogPort );
    ui->ledPosTableLogPort->setText( m_qsRegTablePosLogPort );
    ui->ledFootSwitchType->setText( m_qsRegFootSwitchType );
    ui->ledITUVersion->setText( m_qsRegITUVersion );

    ui->ledTableType->setText( m_qsRegTableType );
    ui->ledItuIp->setText( m_qsRegItuIP );
}
//============================================================================================================
//  on_rbReadSavedSettings_clicked
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgTestinfraRegistry::on_rbReadSavedSettings_clicked()
{
    _loadBackupSettings();

    ui->ledAIFVersion->setText( m_qsIniAIFVersion );
    ui->ledInnovaProgram->setText( m_qsIniInnovaPrg );
    ui->ledFluoroPlus->setText( m_qsIniFPPrg );

    ui->ledEngPcIp->setText( m_qsIniEngPC_IP );
    ui->ledDlPcIp->setText( m_qsIniDL_IP );

    ui->ledAwHostIp->setText( m_qsIniAWHost );

    ui->ledKeypadPort->setText( m_qsIniKeypadPort );

    ui->ledPosIp->setText( m_qsIniPosIPAddress );
    ui->ledPosLogPort->setText( m_qsIniPosLogPort );
    ui->ledPosTableLogPort->setText( m_qsIniTablePosLogPort );
    ui->ledFootSwitchType->setText( m_qsIniFootSwitchType );
    ui->ledITUVersion->setText( m_qsIniITUVersion );

    ui->ledTableType->setText( m_qsIniTableType );
    ui->ledItuIp->setText( m_qsIniItuIP );
}
//============================================================================================================
//  on_pbCheckDifference_clicked
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgTestinfraRegistry::on_pbCheckDifference_clicked()
{
    _loadRegistrySettings();
    _loadBackupSettings();

    if( m_qsRegAIFVersion.compare(m_qsIniAIFVersion) )
    {
        ui->lblAIFVersion->setStyleSheet( "QLabel {font: bold; color: blue;}" );
        ui->ledAIFVersion->setToolTip( tr("Registry: %1  Backup: %2").arg(m_qsRegAIFVersion).arg(m_qsIniAIFVersion) );
    }
    else
    {
        ui->lblAIFVersion->setStyleSheet( "QLabel {font: normal;}" );
        ui->ledAIFVersion->setToolTip( "" );
    }
    if( m_qsRegInnovaPrg.compare(m_qsIniInnovaPrg) )
    {
        ui->lblInnovaProgram->setStyleSheet( "QLabel {font: bold; color: blue;}" );
        ui->ledInnovaProgram->setToolTip( tr("Registry: %1  Backup: %2").arg(m_qsRegInnovaPrg).arg(m_qsIniInnovaPrg) );
    }
    else
    {
        ui->lblInnovaProgram->setStyleSheet( "QLabel {font: normal;}" );
        ui->ledInnovaProgram->setToolTip( "" );
    }
    if( m_qsRegFPPrg.compare(m_qsIniFPPrg) )
    {
        ui->lblFluoroPlus->setStyleSheet( "QLabel {font: bold; color: blue;}" );
        ui->ledFluoroPlus->setToolTip( tr("Registry: %1  Backup: %2").arg(m_qsRegFPPrg).arg(m_qsIniFPPrg) );
    }
    else
    {
        ui->lblFluoroPlus->setStyleSheet( "QLabel {font: normal;}" );
        ui->ledFluoroPlus->setToolTip( "" );
    }
    if( m_qsRegEngPC_IP.compare(m_qsIniEngPC_IP) )
    {
        ui->lblEngPcIp->setStyleSheet( "QLabel {font: bold; color: blue;}" );
        ui->ledEngPcIp->setToolTip( tr("Registry: %1  Backup: %2").arg(m_qsRegEngPC_IP).arg(m_qsIniEngPC_IP) );
        ui->chkEngPcIp->setChecked( true );
    }
    else
    {
        ui->lblEngPcIp->setStyleSheet( "QLabel {font: normal;}" );
        ui->ledEngPcIp->setToolTip( "" );
        ui->chkEngPcIp->setChecked( false );
    }
    if( m_qsRegDL_IP.compare(m_qsIniDL_IP) )
    {
        ui->lblDlPcIp->setStyleSheet( "QLabel {font: bold; color: blue;}" );
        ui->ledDlPcIp->setToolTip( tr("Registry: %1  Backup: %2").arg(m_qsRegDL_IP).arg(m_qsIniDL_IP) );
        ui->chkDlPcIp->setChecked( true );
    }
    else
    {
        ui->lblDlPcIp->setStyleSheet( "QLabel {font: normal;}" );
        ui->ledDlPcIp->setToolTip( "" );
        ui->chkDlPcIp->setChecked( false );
    }
    if( m_qsRegAWHost.compare(m_qsIniAWHost) )
    {
        ui->lblAwHostIp->setStyleSheet( "QLabel {font: bold; color: blue;}" );
        ui->ledAwHostIp->setToolTip( tr("Registry: %1  Backup: %2").arg(m_qsRegAWHost).arg(m_qsIniAWHost) );
        ui->chkAwHostIp->setChecked( true );
    }
    else
    {
        ui->lblAwHostIp->setStyleSheet( "QLabel {font: normal;}" );
        ui->ledAwHostIp->setToolTip( "" );
        ui->chkAwHostIp->setChecked( false );
    }
    if( m_qsRegKeypadPort.compare(m_qsIniKeypadPort) )
    {
        ui->lblKeypadPort->setStyleSheet( "QLabel {font: bold; color: blue;}" );
        ui->ledKeypadPort->setToolTip( tr("Registry: %1  Backup: %2").arg(m_qsRegKeypadPort).arg(m_qsIniKeypadPort) );
        ui->chkKeypadPort->setChecked( true );
    }
    else
    {
        ui->lblKeypadPort->setStyleSheet( "QLabel {font: normal;}" );
        ui->ledKeypadPort->setToolTip( "" );
        ui->chkKeypadPort->setChecked( false );
    }
    if( m_qsRegPosIPAddress.compare(m_qsIniPosIPAddress) )
    {
        ui->lblPosIp->setStyleSheet( "QLabel {font: bold; color: blue;}" );
        ui->ledPosIp->setToolTip( tr("Registry: %1  Backup: %2").arg(m_qsRegPosIPAddress).arg(m_qsIniPosIPAddress) );
        ui->chkPosIp->setChecked( true );
    }
    else
    {
        ui->lblPosIp->setStyleSheet( "QLabel {font: normal;}" );
        ui->ledPosIp->setToolTip( "" );
        ui->chkPosIp->setChecked( false );
    }
    if( m_qsRegPosLogPort.compare(m_qsIniPosLogPort) )
    {
        ui->lblPosLogPort->setStyleSheet( "QLabel {font: bold; color: blue;}" );
        ui->ledPosLogPort->setToolTip( tr("Registry: %1  Backup: %2").arg(m_qsRegPosLogPort).arg(m_qsIniPosLogPort) );
        ui->chkPosLogPort->setChecked( true );
    }
    else
    {
        ui->lblPosLogPort->setStyleSheet( "QLabel {font: normal;}" );
        ui->ledPosLogPort->setToolTip( "" );
        ui->chkPosLogPort->setChecked( false );
    }
    if( m_qsRegTablePosLogPort.compare(m_qsIniTablePosLogPort) )
    {
        ui->lblPosTableLogPort->setStyleSheet( "QLabel {font: bold; color: blue;}" );
        ui->ledPosTableLogPort->setToolTip( tr("Registry: %1  Backup: %2").arg(m_qsRegTablePosLogPort).arg(m_qsIniTablePosLogPort) );
        ui->chkPosTableLogPort->setChecked( true );
    }
    else
    {
        ui->lblPosTableLogPort->setStyleSheet( "QLabel {font: normal;}" );
        ui->ledPosTableLogPort->setToolTip( "" );
        ui->chkPosTableLogPort->setChecked( false );
    }
    if( m_qsRegFootSwitchType.compare(m_qsIniFootSwitchType) )
    {
        ui->lblFootSwitchType->setStyleSheet( "QLabel {font: bold; color: blue;}" );
        ui->ledFootSwitchType->setToolTip( tr("Registry: %1  Backup: %2").arg(m_qsRegFootSwitchType).arg(m_qsIniFootSwitchType) );
        ui->chkFootSwitchType->setChecked( true );
    }
    else
    {
        ui->lblFootSwitchType->setStyleSheet( "QLabel {font: normal;}" );
        ui->ledFootSwitchType->setToolTip( "" );
        ui->chkFootSwitchType->setChecked( false );
    }
    if( m_qsRegITUVersion.compare(m_qsIniITUVersion) )
    {
        ui->lblITUVersion->setStyleSheet( "QLabel {font: bold; color: blue;}" );
        ui->ledITUVersion->setToolTip( tr("Registry: %1  Backup: %2").arg(m_qsRegITUVersion).arg(m_qsIniITUVersion) );
        ui->chkITUVersion->setChecked( true );
    }
    else
    {
        ui->lblITUVersion->setStyleSheet( "QLabel {font: normal;}" );
        ui->ledITUVersion->setToolTip( "" );
        ui->chkITUVersion->setChecked( false );
    }
    if( m_qsRegTableType.compare(m_qsIniTableType) )
    {
        ui->lblTableType->setStyleSheet( "QLabel {font: bold; color: blue;}" );
        ui->ledTableType->setToolTip( tr("Registry: %1  Backup: %2").arg(m_qsRegTableType).arg(m_qsIniTableType) );
        ui->chkTableType->setChecked( true );
    }
    else
    {
        ui->lblTableType->setStyleSheet( "QLabel {font: normal;}" );
        ui->ledTableType->setToolTip( "" );
        ui->chkTableType->setChecked( false );
    }
    if( m_qsRegItuIP.compare(m_qsIniItuIP) )
    {
        ui->lblItuIp->setStyleSheet( "QLabel {font: bold; color: blue;}" );
        ui->ledItuIp->setToolTip( tr("Registry: %1  Backup: %2").arg(m_qsRegItuIP).arg(m_qsIniItuIP) );
        ui->chkItuIp->setChecked( true );
    }
    else
    {
        ui->lblItuIp->setStyleSheet( "QLabel {font: normal;}" );
        ui->ledItuIp->setToolTip( "" );
        ui->chkItuIp->setChecked( false );
    }
}
//============================================================================================================
//  on_pbSaveToIni_clicked
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgTestinfraRegistry::on_pbSaveToIni_clicked()
{
    QSettings   iniFile( "VTIAppManager.ini", QSettings::IniFormat );

    iniFile.setValue( "AIF/Version", ui->ledAIFVersion->text() );
    iniFile.setValue( "AIF/InnovaPrg", ui->ledInnovaProgram->text() );
    iniFile.setValue( "AIF/FPPrg", ui->ledFluoroPlus->text() );

    if( QMessageBox::question( this, tr("Question"),
                               tr("Are you sure you want to overwrite the selected settings\n"
                                  "in your previous backup with the selected values?"),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        if( ui->chkEngPcIp->isChecked() )           iniFile.setValue( "AIF/engPC_IP", ui->ledEngPcIp->text() );
        if( ui->chkDlPcIp->isChecked() )            iniFile.setValue( "AIF/DL_IP", ui->ledDlPcIp->text() );

        if( ui->chkAwHostIp->isChecked() )          iniFile.setValue( "AW/host", ui->ledAwHostIp->text() );

        if( ui->chkKeypadPort->isChecked() )        iniFile.setValue( "Keypad/KeypadPort", ui->ledKeypadPort->text() );

        if( ui->chkPosIp->isChecked() )             iniFile.setValue( "Log/PosIPAddress", ui->ledPosIp->text() );
        if( ui->chkPosLogPort->isChecked() )        iniFile.setValue( "Log/PosLogPort", ui->ledPosLogPort->text() );
        if( ui->chkPosTableLogPort->isChecked() )   iniFile.setValue( "Log/TablePosLogPort", ui->ledPosTableLogPort->text() );
        if( ui->chkFootSwitchType->isChecked() )    iniFile.setValue( "AIF/FootswitchType", ui->ledFootSwitchType->text() );
        if( ui->chkITUVersion->isChecked() )        iniFile.setValue( "AIF/ITUVersion", ui->ledITUVersion->text() );

        if( ui->chkTableType->isChecked() )         iniFile.setValue( "Cycler/TableType", ui->ledTableType->text() );
        if( ui->chkItuIp->isChecked() )             iniFile.setValue( "Cycler/itu ip", ui->ledItuIp->text() );
    }
    ui->chkEngPcIp->setChecked( false );
    ui->chkDlPcIp->setChecked( false );
    ui->chkAwHostIp->setChecked( false );
    ui->chkKeypadPort->setChecked( false );
    ui->chkPosIp->setChecked( false );
    ui->chkPosLogPort->setChecked( false );
    ui->chkPosTableLogPort->setChecked( false );
    ui->chkItuIp->setChecked( false );
    _loadBackupSettings();
    on_pbCheckDifference_clicked();
}
//============================================================================================================
//  on_pbWriteToRegistry_clicked
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgTestinfraRegistry::on_pbWriteToRegistry_clicked()
{
    QSettings   regHKLM( "HKEY_LOCAL_MACHINE\\SOFTWARE\\GE Medical Systems\\Testinfra", QSettings::NativeFormat );

    if( QMessageBox::question( this, tr("Question"),
                               tr("Are you sure you want to overwrite the selected settings\n"
                                  "in the registry with the selected values?"),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        if( ui->chkEngPcIp->isChecked() )           regHKLM.setValue( "AIF/engPC_IP", ui->ledEngPcIp->text() );
        if( ui->chkDlPcIp->isChecked() )            regHKLM.setValue( "AIF/DL_IP", ui->ledDlPcIp->text() );

        if( ui->chkAwHostIp->isChecked() )          regHKLM.setValue( "AIF/AW/host", ui->ledAwHostIp->text() );

        if( ui->chkKeypadPort->isChecked() )        regHKLM.setValue( "AIF/Keypad/KeypadPort", ui->ledKeypadPort->text() );

        if( ui->chkPosIp->isChecked() )             regHKLM.setValue( "AIF/Log/PosIPAddress", ui->ledPosIp->text() );
        if( ui->chkPosLogPort->isChecked() )        regHKLM.setValue( "AIF/Log/PosLogPort", ui->ledPosLogPort->text() );
        if( ui->chkPosTableLogPort->isChecked() )   regHKLM.setValue( "AIF/Log/TablePosLogPort", ui->ledPosTableLogPort->text() );
        if( ui->chkFootSwitchType->isChecked() )    regHKLM.setValue( "AIF/FootswitchType", ui->ledFootSwitchType->text() );
        if( ui->chkITUVersion->isChecked() )        regHKLM.setValue( "AIF/ITUVersion", ui->ledITUVersion->text() );

        if( ui->chkTableType->isChecked() )         regHKLM.setValue( "Cycler/TableType", ui->ledTableType->text() );
        if( ui->chkItuIp->isChecked() )             regHKLM.setValue( "Cycler/TempData/itu ip", ui->ledItuIp->text() );
    }
    ui->chkEngPcIp->setChecked( false );
    ui->chkDlPcIp->setChecked( false );
    ui->chkAwHostIp->setChecked( false );
    ui->chkKeypadPort->setChecked( false );
    ui->chkPosIp->setChecked( false );
    ui->chkPosLogPort->setChecked( false );
    ui->chkPosTableLogPort->setChecked( false );
    ui->chkItuIp->setChecked( false );
    _loadRegistrySettings();
    on_pbCheckDifference_clicked();
}
//============================================================================================================
//  on_pbWriteToRegistry_clicked
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgTestinfraRegistry::_loadRegistrySettings()
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
//============================================================================================================
//  on_pbWriteToRegistry_clicked
//------------------------------------------------------------------------------------------------------------
//
//============================================================================================================
void dlgTestinfraRegistry::_loadBackupSettings()
{
    QSettings   iniFile( "VTIAppManager.ini", QSettings::IniFormat );

    m_qsIniAIFVersion = iniFile.value( "AIF/Version", tr("<key not found>") ).toString();
    m_qsIniInnovaPrg = iniFile.value( "AIF/InnovaPrg", tr("<key not found>") ).toString();
    m_qsIniFPPrg = iniFile.value( "AIF/FPPrg", tr("<key not found>") ).toString();

    m_qsIniEngPC_IP = iniFile.value( "AIF/engPC_IP", tr("<key not found>") ).toString();
    m_qsIniDL_IP = iniFile.value( "AIF/DL_IP", tr("<key not found>") ).toString();

    m_qsIniAWHost = iniFile.value( "AW/host", tr("<key not found>") ).toString();

    m_qsIniKeypadPort = iniFile.value( "Keypad/KeypadPort", tr("<key not found>") ).toString();

    m_qsIniPosIPAddress = iniFile.value( "Log/PosIPAddress", tr("<key not found>") ).toString();
    m_qsIniPosLogPort = iniFile.value( "Log/PosLogPort", tr("<key not found>") ).toString();
    m_qsIniTablePosLogPort = iniFile.value( "Log/TablePosLogPort", tr("<key not found>") ).toString();
    m_qsIniFootSwitchType = iniFile.value( "AIF/FootswitchType", tr("<key not found>") ).toString();
    m_qsIniITUVersion = iniFile.value( "AIF/ITUVersion", tr("<key not found>") ).toString();

    m_qsIniTableType = iniFile.value( "Cycler/TableType", tr("<key not found>") ).toString();
    m_qsIniItuIP = iniFile.value( "Cycler/itu ip", tr("<key not found>") ).toString();
}
//============================================================================================================

void dlgTestinfraRegistry::on_chkSelectAll_clicked(bool checked)
{
    ui->chkAwHostIp->setChecked( checked );
    ui->chkDlPcIp->setChecked( checked );
    ui->chkEngPcIp->setChecked( checked );
    ui->chkFootSwitchType->setChecked( checked );
    ui->chkItuIp->setChecked( checked );
    ui->chkITUVersion->setChecked( checked );
    ui->chkKeypadPort->setChecked( checked );
    ui->chkPosIp->setChecked( checked );
    ui->chkPosLogPort->setChecked( checked );
    ui->chkPosTableLogPort->setChecked( checked );
    ui->chkTableType->setChecked( checked );
}
