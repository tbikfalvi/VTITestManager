#ifndef DLGMANAGERELEASES_H
#define DLGMANAGERELEASES_H

#include <QDialog>
#include <QString>
#include <QModelIndex>

namespace Ui {
    class dlgManageReleases;
}

class dlgManageReleases : public QDialog
{
    Q_OBJECT

public:
    explicit dlgManageReleases(QWidget *parent = 0);
    ~dlgManageReleases();

private slots:
    void on_pbAIFChangeDir_clicked();
    void on_pbAIFInstall_clicked();
    void on_pbExit_clicked();
    void on_listAIFReleases_clicked(const QModelIndex &item);
    void on_pbAIFUninstall_clicked();

    void on_listCyclerReleases_clicked(const QModelIndex &index);

    void on_pbCyclerInstall_clicked();

    void on_listCSSReleases_clicked(const QModelIndex &index);

    void on_pbCSSInstall_clicked();

    void on_pbInstallAll_clicked();

private:
    Ui::dlgManageReleases *ui;

    QString m_qsLocalDirRelease;
    QString m_qsAIFFileRelease;
    QString m_qsCyclerFileRelease;
    QString m_qsCSSFileRelease;
    bool    m_bSilentInstall;
    QString m_qsRegAIFVersion;
    QString m_qsRegInnovaPrg;
    QString m_qsRegFPPrg;
    QString m_qsRegEngPC_IP;
    QString m_qsRegDL_IP;
    QString m_qsRegAWHost;
    QString m_qsRegKeypadPort;
    QString m_qsRegPosIPAddress;
    QString m_qsRegPosLogPort;
    QString m_qsRegTablePosLogPort;
    QString m_qsRegFootSwitchType;
    QString m_qsRegITUVersion;
    QString m_qsRegTableType;
    QString m_qsRegItuIP;

    void    _fillListAIFReleases();
    void    _fillListCyclerReleases();
    void    _fillListCSSReleases();
    void    _loadRegistrySettings();
    void    _loadBackupSettings();
    void    _backupRegistrySettings();
    void    _restoreRegistrySettings();
};

#endif // DLGMANAGERELEASES_H
