//============================================================================================================
//
//  VTI Test Manager application
//
//  File            : dlgtestinfraregistry.h
//  File version    : 1.0
//
//============================================================================================================

#ifndef DLGTESTINFRAREGISTRY_H
#define DLGTESTINFRAREGISTRY_H

//============================================================================================================

#include <QDialog>

//============================================================================================================
namespace Ui
{
    class dlgTestinfraRegistry;
}
//============================================================================================================
class dlgTestinfraRegistry : public QDialog
{
    Q_OBJECT

public:
    explicit dlgTestinfraRegistry(QWidget *parent = 0);
    ~dlgTestinfraRegistry();

private slots:
    void slotEnableButtons();
    void on_pbReadFromRegistry_clicked();
    void on_pbSaveToIni_clicked();
    void on_pbWriteToRegistry_clicked();
    void on_rbReadSavedSettings_clicked();

    void on_pbCheckDifference_clicked();

    void on_chkSelectAll_clicked(bool checked);

private:
    void _loadRegistrySettings();
    void _loadBackupSettings();

    Ui::dlgTestinfraRegistry    *ui;
    QString                      m_qsRegAIFVersion;
    QString                      m_qsIniAIFVersion;
    QString                      m_qsRegInnovaPrg;
    QString                      m_qsIniInnovaPrg;
    QString                      m_qsRegFPPrg;
    QString                      m_qsIniFPPrg;
    QString                      m_qsRegEngPC_IP;
    QString                      m_qsIniEngPC_IP;
    QString                      m_qsRegDL_IP;
    QString                      m_qsIniDL_IP;
    QString                      m_qsRegAWHost;
    QString                      m_qsIniAWHost;
    QString                      m_qsRegKeypadPort;
    QString                      m_qsIniKeypadPort;
    QString                      m_qsRegPosIPAddress;
    QString                      m_qsIniPosIPAddress;
    QString                      m_qsRegPosLogPort;
    QString                      m_qsIniPosLogPort;
    QString                      m_qsRegTablePosLogPort;
    QString                      m_qsIniTablePosLogPort;
    QString                      m_qsRegFootSwitchType;
    QString                      m_qsIniFootSwitchType;
    QString                      m_qsRegITUVersion;
    QString                      m_qsIniITUVersion;
    QString                      m_qsRegTableType;
    QString                      m_qsIniTableType;
    QString                      m_qsRegItuIP;
    QString                      m_qsIniItuIP;
};
//============================================================================================================

#endif // DLGTESTINFRAREGISTRY_H

//============================================================================================================
