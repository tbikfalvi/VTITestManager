

#include "tmpreferences.h"
#include "tracer.h"

cTMPreferences::cTMPreferences() : cPreferences()
{
    cTracer tracer( "cTMPreferences::cTMPreferences" );

    setAppName( "VTITestManager" );

    m_bActionNeeded             = false;

    m_qsDirAIF                  = QString("C:/Program Files/GE Medical Systems/Testinfra/AutomationIF");
    m_qsDirAIFConf              = QString("%1/test2").arg(m_qsDirAIF);
    m_qsDirLocalReleases        = QString("D:/Users/Releases");
    m_qsDirRemoteReleases       = QString("");
    m_qsDirCycler               = QString("C:/cycler_liberty");
    m_qsDirCyclerConf           = QString("%1/<CYCLER_VERSION>/test/engine").arg(m_qsDirCycler);
    m_qsDirCSS                  = QString("C:/cycler_liberty/CellStatusScenario");
    m_qsRegAIFVersion           = QObject::tr("_not_defined_");
    m_qsRegITUAddress           = QObject::tr("_not_defined_");
    m_bIsRunningOnDevelopmentPC = false;
    m_qsTesterName              = QObject::tr("_not_defined_");
    m_qsTestReportRevision      = "1";
    m_qsTestEnvironment         = QObject::tr("_not_defined_");
    m_qsBuildAIFDrive           = QString("C");
    m_qsBuildAIFVersion         = QString("test");
    m_bBuildAIFEngVersion       = false;
    m_qsBuildAIFEngNumber       = QString("test");
    m_qsBuildCyclerDrive        = QString("C");
    m_qsBuildCyclerVersion      = QString("test");
    m_bBuildCyclerEngVersion    = false;
    m_qsBuildCyclerEngNumber    = QString("test");
    m_qsBuildCSSDrive           = QString("C");
    m_qsBuildCSSVersion         = QString("test");
    m_bBuildCSSEngVersion       = false;
    m_qsBuildCSSEngNumber       = QString("test");
    m_qsProcessDrive            = QString("C");
    m_qsProcessPath             = QString("\\");
}

cTMPreferences::~cTMPreferences()
{
}

void cTMPreferences::readSettings( QSettings *m_poSettingsFile )
{
    cTracer tracer( "cTMPreferences::readSettings" );

    QSettings   regHKLM_Testinfra( "HKEY_LOCAL_MACHINE\\SOFTWARE\\GE Medical Systems\\Testinfra", QSettings::NativeFormat );

    m_qsDirAIF = m_poSettingsFile->value( "Test_Config/AIFDirectory", m_qsDirAIF ).toString();
    m_qsDirAIFConf = m_poSettingsFile->value( "Test_Config/AIFTestDirectory", m_qsDirAIFConf ).toString();
    m_qsDirLocalReleases = m_poSettingsFile->value( "Releases/LocalDirectory", m_qsDirLocalReleases ).toString();
    m_qsDirRemoteReleases = m_poSettingsFile->value( "Releases/RemoteDirectory", m_qsDirRemoteReleases ).toString();
    m_qsDirCycler = m_poSettingsFile->value( "Test_Config/CyclerDirectory", m_qsDirCycler ).toString();
    m_qsDirCyclerConf = m_poSettingsFile->value( "Test_Config/CyclerTestDirectory", m_qsDirCyclerConf ).toString();
    m_qsDirCSS = m_poSettingsFile->value( "Test_Config/CSSDirectory", m_qsDirCSS ).toString();

    m_qsRegAIFVersion = regHKLM_Testinfra.value( "AIF/Version", QObject::tr("_not_defined_") ).toString();
    m_qsRegITUAddress = regHKLM_Testinfra.value( "Cycler/TempData/infoITU", QObject::tr("_not_defined_") ).toString();

    QSettings   regHKLM_TestManager( "HKEY_LOCAL_MACHINE\\SOFTWARE\\VTI Test Manager", QSettings::NativeFormat );

    QString qsTemp = regHKLM_TestManager.value( "RunningOnDevelopmentPc", QObject::tr("_not_defined_") ).toString();

    if( qsTemp.compare(QObject::tr("_not_defined_")) == 0 )
    {
        m_bActionNeeded = true;
        m_qslActionsNeeded << QString( cTMPreferences::ACT_RUN_ENV );
    }
    else
    {
        m_bIsRunningOnDevelopmentPC = regHKLM_TestManager.value( "RunningOnDevelopmentPc", false ).toBool();
    }

    m_qslAIFTestCaseTypes = QStringList() << m_poSettingsFile->value( "AIF/TestcaseTypes", "" ).toString().split(QChar('#'));
    if( m_qslAIFTestCaseTypes.count() == 1 && m_qslAIFTestCaseTypes.at(0).length() < 1 )
    {
        m_qslAIFTestCaseTypes.clear();
    }

    m_qslCyclerTestCaseTypes = QStringList() << m_poSettingsFile->value( "Cycler/TestcaseTypes", "" ).toString().split(QChar('#'));
    if( m_qslCyclerTestCaseTypes.count() == 1 && m_qslCyclerTestCaseTypes.at(0).length() < 1 )
    {
        m_qslCyclerTestCaseTypes.clear();
    }

    m_qsTesterName = m_poSettingsFile->value( "Test_Config/TesterName", QObject::tr("_not_defined_") ).toString();
    m_qsTestReportRevision = m_poSettingsFile->value( "Test_Config/TestReportRevision", "1" ).toString();
    m_qsTestEnvironment = m_poSettingsFile->value( "Test_Config/TestEnvironment", QObject::tr("_not_defined_") ).toString();

    m_qsBuildAIFDrive = m_poSettingsFile->value( "BuildAIF/Drive", QObject::tr("C") ).toString();
    m_qsBuildAIFVersion = m_poSettingsFile->value( "BuildAIF/Version", QObject::tr("test") ).toString();
    m_bBuildAIFEngVersion = m_poSettingsFile->value( "BuildAIF/ENGRelease", false ).toBool();
    m_qsBuildAIFEngNumber = m_poSettingsFile->value( "BuildAIF/ENGNumber", QObject::tr("test") ).toString();
    m_qsBuildCyclerDrive = m_poSettingsFile->value( "BuildCycler/Drive", QObject::tr("C") ).toString();
    m_qsBuildCyclerVersion = m_poSettingsFile->value( "BuildCycler/Version", QObject::tr("test") ).toString();
    m_bBuildCyclerEngVersion = m_poSettingsFile->value( "BuildCycler/ENGRelease", false ).toBool();
    m_qsBuildCyclerEngNumber = m_poSettingsFile->value( "BuildCycler/ENGNumber", QObject::tr("test") ).toString();
    m_qsBuildCSSDrive = m_poSettingsFile->value( "BuildCSS/Drive", QObject::tr("C") ).toString();
    m_qsBuildCSSVersion = m_poSettingsFile->value( "BuildCSS/Version", QObject::tr("test") ).toString();
    m_bBuildCSSEngVersion = m_poSettingsFile->value( "BuildCSS/ENGRelease", false ).toBool();
    m_qsBuildCSSEngNumber = m_poSettingsFile->value( "BuildCSS/ENGNumber", QObject::tr("test") ).toString();

    m_qsProcessDrive = m_poSettingsFile->value( "Process/Drive", QObject::tr("C") ).toString();
    m_qsProcessPath = m_poSettingsFile->value( "Process/Path", QObject::tr("\\") ).toString();

    m_qslCommands = QStringList() << m_poSettingsFile->value( "Process/Commands", "" ).toString().split(QString("###"));
    if( m_qslCommands.count() == 1 && m_qslCommands.at(0).length() < 1 )
    {
        m_qslCommands.clear();
    }
}

void cTMPreferences::writeSettings( QSettings *m_poSettingsFile ) const
{
    cTracer tracer( "cTMPreferences::writeSettings" );

    m_poSettingsFile->setValue( "Test_Config/AIFDirectory", m_qsDirAIF );
    m_poSettingsFile->setValue( "Test_Config/AIFTestDirectory", m_qsDirAIFConf );
    m_poSettingsFile->setValue( "Releases/LocalDirectory", m_qsDirLocalReleases );
    m_poSettingsFile->setValue( "Releases/RemoteDirectory", m_qsDirRemoteReleases );
    m_poSettingsFile->setValue( "Test_Config/CyclerDirectory", m_qsDirCycler );
    m_poSettingsFile->setValue( "Test_Config/CyclerTestDirectory", m_qsDirCyclerConf );
    m_poSettingsFile->setValue( "Test_Config/CSSDirectory", m_qsDirCSS );

    // m_qsRegAIFVersion - overwrite this registry value is forbidden
    // m_qsRegITUAddress - overwrite this registry value is forbidden

    m_poSettingsFile->setValue( "BuildAIF/Drive", m_qsBuildAIFDrive );
    m_poSettingsFile->setValue( "BuildAIF/Version", m_qsBuildAIFVersion );
    m_poSettingsFile->setValue( "BuildAIF/ENGRelease", m_bBuildAIFEngVersion );
    m_poSettingsFile->setValue( "BuildAIF/ENGNumber", m_qsBuildAIFEngNumber );
    m_poSettingsFile->setValue( "BuildCycler/Drive", m_qsBuildCyclerDrive );
    m_poSettingsFile->setValue( "BuildCycler/Version", m_qsBuildCyclerVersion );
    m_poSettingsFile->setValue( "BuildCycler/ENGRelease", m_bBuildCyclerEngVersion );
    m_poSettingsFile->setValue( "BuildCycler/ENGNumber", m_qsBuildCyclerEngNumber );
    m_poSettingsFile->setValue( "BuildCSS/Drive", m_qsBuildCSSDrive );
    m_poSettingsFile->setValue( "BuildCSS/Version", m_qsBuildCSSVersion );
    m_poSettingsFile->setValue( "BuildCSS/ENGRelease", m_bBuildCSSEngVersion );
    m_poSettingsFile->setValue( "BuildCSS/ENGNumber", m_qsBuildCSSEngNumber );

    QSettings   regHKLM_TestManager( "HKEY_LOCAL_MACHINE\\SOFTWARE\\VTI Test Manager", QSettings::NativeFormat );

    regHKLM_TestManager.setValue( "RunningOnDevelopmentPc", m_bIsRunningOnDevelopmentPC );

    m_poSettingsFile->setValue( "AIF/TestcaseTypes", m_qslAIFTestCaseTypes.join(QChar('#')) );
    m_poSettingsFile->setValue( "Cycler/TestcaseTypes", m_qslCyclerTestCaseTypes.join(QChar('#')) );

    m_poSettingsFile->setValue( "Test_Config/TesterName", m_qsTesterName );
    m_poSettingsFile->setValue( "Test_Config/TestReportRevision", m_qsTestReportRevision );
    m_poSettingsFile->setValue( "Test_Config/TestEnvironment", m_qsTestEnvironment );

    m_poSettingsFile->setValue( "Process/Drive", m_qsProcessDrive );
    m_poSettingsFile->setValue( "Process/Path", m_qsProcessPath );
    m_poSettingsFile->setValue( "Process/Commands", m_qslCommands.join(QString("###")) );
}

bool cTMPreferences::actionNeeded()
{
    bool bRet = false;

    if( m_bActionNeeded && m_qslActionsNeeded.count() > 0 )
    {
        bRet = true;
    }
    else if( m_bActionNeeded && m_qslActionsNeeded.count() < 1 )
    {
        m_bActionNeeded = false;
    }
    else if( !m_bActionNeeded && m_qslActionsNeeded.count() > 0 )
    {
        m_qslActionsNeeded.clear();
    }

    return bRet;
}

bool cTMPreferences::actionNeeded( actionList p_nAction )
{
    return m_qslActionsNeeded.contains(QString(p_nAction));
}

void cTMPreferences::actionProcessed( actionList p_nAction )
{
    if( m_qslActionsNeeded.contains(QString(p_nAction)) )
    {
        m_qslActionsNeeded.removeOne(QString(p_nAction));
    }
    save();
    actionNeeded();
}

QString cTMPreferences::dirAIF() const
{
    return m_qsDirAIF;
}

QString cTMPreferences::dirAIFConf() const
{
    return m_qsDirAIFConf;
}

QString cTMPreferences::dirLocalReleases() const
{
    return m_qsDirLocalReleases;
}

QString cTMPreferences::dirRemoteReleases() const
{
    return m_qsDirRemoteReleases;
}

QString cTMPreferences::dirCycler() const
{
    return m_qsDirCycler;
}

QString cTMPreferences::dirCyclerConf() const
{
    return m_qsDirCyclerConf;
}

QString cTMPreferences::dirCSS() const
{
    return m_qsDirCSS;
}

QString cTMPreferences::regAIFVersion() const
{
    return m_qsRegAIFVersion;
}

QString cTMPreferences::regITUAddress() const
{
    return m_qsRegITUAddress;
}

bool cTMPreferences::isRunningOnDevelopmentPC()
{
    return m_bIsRunningOnDevelopmentPC;
}

QStringList cTMPreferences::testcaseTypes(tcSystem p_tcSystem) const
{
    if( p_tcSystem == TC_CYCLER ) return m_qslCyclerTestCaseTypes;
    else return m_qslAIFTestCaseTypes;
}

QString cTMPreferences::testerName() const
{
    return m_qsTesterName;
}

QString cTMPreferences::testReportRevision() const
{
    return m_qsTestReportRevision;
}

QString cTMPreferences::testEnvironment() const
{
    return m_qsTestEnvironment;
}

QString cTMPreferences::buildAIFDrive() const
{
    return m_qsBuildAIFDrive;
}

QString cTMPreferences::buildAIFVersion() const
{
    return m_qsBuildAIFVersion;
}

bool cTMPreferences::buildAIFEngVersion()
{
    return m_bBuildAIFEngVersion;
}

QString cTMPreferences::buildAIFEngNumber()
{
    return m_qsBuildAIFEngNumber;
}

QString cTMPreferences::buildCyclerDrive() const
{
    return m_qsBuildCyclerDrive;
}

QString cTMPreferences::buildCyclerVersion() const
{
    return m_qsBuildCyclerVersion;
}

bool cTMPreferences::buildCyclerEngVersion()
{
    return m_bBuildCyclerEngVersion;
}

QString cTMPreferences::buildCyclerEngNumber()
{
    return m_qsBuildCyclerEngNumber;
}

QString cTMPreferences::buildCSSDrive() const
{
    return m_qsBuildCSSDrive;
}

QString cTMPreferences::buildCSSVersion() const
{
    return m_qsBuildCSSVersion;
}

bool cTMPreferences::buildCSSEngVersion()
{
    return m_bBuildCSSEngVersion;
}

QString cTMPreferences::buildCSSEngNumber()
{
    return m_qsBuildCSSEngNumber;
}

QString cTMPreferences::processDrive() const
{
    return m_qsProcessDrive;
}

QString cTMPreferences::processPath() const
{
    return m_qsProcessPath;
}

QStringList cTMPreferences::commands() const
{
    return m_qslCommands;
}

//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------

void cTMPreferences::setDirAIF( const QString &p_qsDirAIF )
{
    m_qsDirAIF = p_qsDirAIF;
}

void cTMPreferences::setDirAIFConf( const QString &p_qsDirAIFConf )
{
    m_qsDirAIFConf = p_qsDirAIFConf;
}

void cTMPreferences::setDirLocalReleases( const QString &p_qsDirLocalReleases )
{
    m_qsDirLocalReleases = p_qsDirLocalReleases;
}

void cTMPreferences::setDirRemoteReleases( const QString &p_qsDirRemoteReleases )
{
    m_qsDirRemoteReleases = p_qsDirRemoteReleases;
}

void cTMPreferences::setDirCycler( const QString &p_qsDirCycler )
{
    m_qsDirCycler = p_qsDirCycler;
}

void cTMPreferences::setDirCyclerConf( const QString &p_qsDirCyclerConf )
{
    m_qsDirCyclerConf = p_qsDirCyclerConf;
}

void cTMPreferences::setDirCSS( const QString &p_qsDirCSS )
{
    m_qsDirCSS = p_qsDirCSS;
}

void cTMPreferences::setRegAIFVersion( const QString &p_qsRegAIFVersion )
{
    m_qsRegAIFVersion = p_qsRegAIFVersion;
}

void cTMPreferences::setRegITUAddress( const QString &p_qsRegITUAddress )
{
    m_qsRegITUAddress = p_qsRegITUAddress;
}

void cTMPreferences::setRunningOnDevelopmentPC(bool p_bIsRunningOnDevelopmentPC)
{
    m_bIsRunningOnDevelopmentPC = p_bIsRunningOnDevelopmentPC;
}

void cTMPreferences::addTestcaseType(tcSystem p_tcSystem, QString p_qsTestcaseType)
{
    if( p_qsTestcaseType.length() < 0 )
        return;

    if( p_tcSystem == TC_AIF )
    {
        if( m_qslAIFTestCaseTypes.contains( p_qsTestcaseType.trimmed() ) )
            return;

        m_qslAIFTestCaseTypes << p_qsTestcaseType.trimmed();
    }
    else if( p_tcSystem == TC_CYCLER )
    {
        if( m_qslCyclerTestCaseTypes.contains( p_qsTestcaseType.trimmed() ) )
            return;

        m_qslCyclerTestCaseTypes << p_qsTestcaseType.trimmed();
    }
}

void cTMPreferences::setTestcaseType(tcSystem p_tcSystem, const QStringList &p_qslTestcaseType)
{
    if( p_tcSystem == TC_AIF )
    {
        m_qslAIFTestCaseTypes = p_qslTestcaseType;
        if( m_qslAIFTestCaseTypes.count() == 1 && m_qslAIFTestCaseTypes.at(0).length() < 1 )
        {
            m_qslAIFTestCaseTypes.clear();
        }
    }
    else if( p_tcSystem == TC_CYCLER )
    {
        m_qslCyclerTestCaseTypes = p_qslTestcaseType;
        if( m_qslCyclerTestCaseTypes.count() == 1 && m_qslCyclerTestCaseTypes.at(0).length() < 1 )
        {
            m_qslCyclerTestCaseTypes.clear();
        }
    }
}

void cTMPreferences::setTesterName(const QString p_qsTesterName, bool p_bIsSaved)
{
    m_qsTesterName = p_qsTesterName;

    if( p_bIsSaved )
    {
        QSettings obPrefFile( m_qsFileName, QSettings::IniFormat );

        obPrefFile.setValue( "Test_Config/TesterName", m_qsTesterName );
    }
}

void cTMPreferences::setTestReportRevision(const QString p_qsTestReportRevision, bool p_bIsSaved)
{
    m_qsTestReportRevision = p_qsTestReportRevision;

    if( p_bIsSaved )
    {
        QSettings obPrefFile( m_qsFileName, QSettings::IniFormat );

        obPrefFile.setValue( "Test_Config/TestReportRevision", m_qsTestReportRevision );
    }
}

void cTMPreferences::setTestEnvironment(const QString p_qsTestEnvironment, bool p_bIsSaved)
{
    m_qsTestEnvironment = p_qsTestEnvironment;

    if( p_bIsSaved )
    {
        QSettings obPrefFile( m_qsFileName, QSettings::IniFormat );

        obPrefFile.setValue( "Test_Config/TestEnvironment", m_qsTestEnvironment );
    }
}

void cTMPreferences::setBuildAIFDrive( const QString &p_qsBuildAIFDrive )
{
    m_qsBuildAIFDrive = p_qsBuildAIFDrive;
}

void cTMPreferences::setBuildAIFVersion( const QString &p_qsBuildAIFVersion )
{
    m_qsBuildAIFVersion = p_qsBuildAIFVersion;
}

void cTMPreferences::setBuildAIFEngVersion( bool p_bBuildAIFEngVersion )
{
    m_bBuildAIFEngVersion = p_bBuildAIFEngVersion;
}

void cTMPreferences::setBuildAIFEngNumber( const QString &p_qsBuildAIFEngNumber )
{
    m_qsBuildAIFEngNumber = p_qsBuildAIFEngNumber;
}

void cTMPreferences::setBuildCyclerDrive( const QString &p_qsBuildCyclerDrive )
{
    m_qsBuildCyclerDrive = p_qsBuildCyclerDrive;
}

void cTMPreferences::setBuildCyclerVersion( const QString &p_qsBuildCyclerVersion )
{
    m_qsBuildCyclerVersion = p_qsBuildCyclerVersion;
}

void cTMPreferences::setBuildCyclerEngVersion( bool p_bBuildCyclerEngVersion )
{
    m_bBuildCyclerEngVersion = p_bBuildCyclerEngVersion;
}

void cTMPreferences::setBuildCyclerEngNumber( const QString &p_qsBuildCyclerEngNumber )
{
    m_qsBuildCyclerEngNumber = p_qsBuildCyclerEngNumber;
}

void cTMPreferences::setBuildCSSDrive( const QString &p_qsBuildCSSDrive )
{
    m_qsBuildCSSDrive = p_qsBuildCSSDrive;
}

void cTMPreferences::setBuildCSSVersion( const QString &p_qsBuildCSSVersion )
{
    m_qsBuildCSSVersion = p_qsBuildCSSVersion;
}

void cTMPreferences::setBuildCSSEngVersion( bool p_bBuildCSSEngVersion )
{
    m_bBuildCSSEngVersion = p_bBuildCSSEngVersion;
}

void cTMPreferences::setBuildCSSEngNumber( const QString &p_qsBuildCSSEngNumber )
{
    m_qsBuildCSSEngNumber = p_qsBuildCSSEngNumber;
}

void cTMPreferences::setProcessDrive( const QString &p_qsProcessDrive )
{
    m_qsProcessDrive = p_qsProcessDrive;
}

void cTMPreferences::setProcessPath( const QString &p_qsProcessPath )
{
    m_qsProcessPath = p_qsProcessPath;
}

void cTMPreferences::addCommand(const QString &p_qsCommand)
{
    if( !m_qslCommands.contains( p_qsCommand ) && p_qsCommand.length() > 0 )
    {
        m_qslCommands << p_qsCommand;
    }
}

void cTMPreferences::deleteCommands()
{
    m_qslCommands.clear();
}

QRect cTMPreferences::screenSize() const
{
    return m_qrScreenSize;
}

void cTMPreferences::setScreenSize(const QRect &p_qrScreenSize)
{
    m_qrScreenSize = p_qrScreenSize;
}
