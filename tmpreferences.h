#ifndef TMPREFERENCES_H
#define TMPREFERENCES_H

#include <QStringList>
#include <QRect>

#include <preferences.h>
#include <singleton.h>

class cTMPreferences : public cPreferences
{
public:

    static const QString qsAppVersion() { return QString( "1.5.9.0" ); }

    enum actionList {
        ACT_NONE = 0,
        ACT_RUN_ENV
    };

    enum runningEnvironment {
        ENV_CELL = 1,
        ENV_MINISYSTEM = 5,
        ENV_STANDALONE = 2,
        ENV_DESKTOP = 99
    };

    enum tcSystem {
        TC_AIF = 1,
        TC_CYCLER
    };

    cTMPreferences();
    ~cTMPreferences();

    bool        actionNeeded();
    bool        actionNeeded( actionList p_nAction );
    void        actionProcessed( actionList p_nAction );

    QRect           screenSize() const;

    QString         dirAIF() const;
    QString         dirAIFConf() const;
    QString         dirLocalReleases() const;
    QString         dirRemoteReleases() const;
    QString         dirCycler() const;
    QString         dirCyclerConf() const;
    QString         dirCSS() const;
    QString         regAIFVersion() const;
    QString         regITUAddress() const;
    bool            isRunningOnDevelopmentPC();
    QStringList     testcaseTypes( tcSystem p_tcSystem ) const;
    QString         testerName() const;
    QString         testReportRevision() const;
    QString         testEnvironment() const;
    QString         buildAIFDrive() const;
    QString         buildAIFVersion() const;
    bool            buildAIFEngVersion();
    QString         buildAIFEngNumber();
    QString         buildCyclerDrive() const;
    QString         buildCyclerVersion() const;
    bool            buildCyclerEngVersion();
    QString         buildCyclerEngNumber();
    QString         buildCSSDrive() const;
    QString         buildCSSVersion() const;
    bool            buildCSSEngVersion();
    QString         buildCSSEngNumber();
    QString         processDrive() const;
    QString         processPath() const;
    QStringList     commands() const;

    void            setScreenSize( const QRect &p_qrScreenSize );
    void            setDirAIF( const QString &p_qsDirAIF );
    void            setDirAIFConf( const QString &p_qsDirAIFConf );
    void            setDirLocalReleases( const QString &p_qsDirLocalReleases );
    void            setDirRemoteReleases( const QString &p_qsDirRemoteReleases );
    void            setDirCycler( const QString &p_qsDirCycler );
    void            setDirCyclerConf( const QString &p_qsDirCyclerConf );
    void            setDirCSS( const QString &p_qsDirCSS );
    void            setRegAIFVersion( const QString &p_qsRegAIFVersion );
    void            setRegITUAddress( const QString &p_qsRegITUAddress );
    void            setRunningOnDevelopmentPC( bool p_bIsRunningOnDevelopmentPC );
    void            addTestcaseType( tcSystem p_tcSystem, QString p_qsTestcaseType );
    void            setTestcaseType( tcSystem p_tcSystem, const QStringList &p_qslTestcaseType );
    void            setTesterName( const QString p_qsTesterName, bool p_bIsSaved = false );
    void            setTestReportRevision( const QString p_qsTestReportRevision, bool p_bIsSaved = false );
    void            setTestEnvironment( const QString p_qsTestEnvironment, bool p_bIsSaved = false );
    void            setBuildAIFDrive( const QString &p_qsBuildAIFDrive );
    void            setBuildAIFVersion( const QString &p_qsBuildAIFVersion );
    void            setBuildAIFEngVersion( bool p_bBuildAIFEngVersion );
    void            setBuildAIFEngNumber( const QString &p_qsBuildAIFEngNumber );
    void            setBuildCyclerDrive( const QString &p_qsBuildCyclerDrive );
    void            setBuildCyclerVersion( const QString &p_qsBuildCyclerVersion );
    void            setBuildCyclerEngVersion( bool p_bBuildCyclerEngVersion );
    void            setBuildCyclerEngNumber( const QString &p_qsBuildCyclerEngNumber );
    void            setBuildCSSDrive( const QString &p_qsBuildCSSDrive );
    void            setBuildCSSVersion( const QString &p_qsBuildCSSVersion );
    void            setBuildCSSEngVersion( bool p_bBuildCSSEngVersion );
    void            setBuildCSSEngNumber( const QString &p_qsBuildCSSEngNumber );
    void            setProcessDrive( const QString &p_qsProcessDrive );
    void            setProcessPath( const QString &p_qsProcessPath );
    void            addCommand( const QString &p_qsCommand );
    void            deleteCommands();

private:

    QRect           m_qrScreenSize;
    bool            m_bActionNeeded;

    QStringList     m_qslActionsNeeded;

    QString         m_qsDirAIF;
    QString         m_qsDirAIFConf;
    QString         m_qsDirLocalReleases;
    QString         m_qsDirRemoteReleases;
    QString         m_qsDirCycler;
    QString         m_qsDirCyclerConf;
    QString         m_qsDirCSS;

    QString         m_qsRegAIFVersion;
    QString         m_qsRegITUAddress;

    QString         m_qsVersionAIFBuild;
    QString         m_qsNumberAIFENG;
    bool            m_bISAIFENGRelease;
    bool            m_bIsRunningOnDevelopmentPC;

    QStringList     m_qslAIFTestCaseTypes;
    QStringList     m_qslCyclerTestCaseTypes;

    QString         m_qsTesterName;
    QString         m_qsTestReportRevision;
    QString         m_qsTestEnvironment;

    QString         m_qsBuildAIFDrive;
    QString         m_qsBuildAIFVersion;
    bool            m_bBuildAIFEngVersion;
    QString         m_qsBuildAIFEngNumber;
    QString         m_qsBuildCyclerDrive;
    QString         m_qsBuildCyclerVersion;
    bool            m_bBuildCyclerEngVersion;
    QString         m_qsBuildCyclerEngNumber;
    QString         m_qsBuildCSSDrive;
    QString         m_qsBuildCSSVersion;
    bool            m_bBuildCSSEngVersion;
    QString         m_qsBuildCSSEngNumber;

    QString         m_qsProcessDrive;
    QString         m_qsProcessPath;
    QStringList     m_qslCommands;

protected:

    virtual void    readSettings( QSettings *m_poSettingsFile );
    virtual void    writeSettings( QSettings *m_poSettingsFile ) const;
};

typedef cSingleton<cTMPreferences> tgPrefs;

#endif // TMPREFERENCES_H
