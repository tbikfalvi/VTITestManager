#ifndef DLGPROCESSCMD_H
#define DLGPROCESSCMD_H

#include <QDialog>
#include <QProcess>

namespace Ui {
    class dlgProcessCmd;
}

class dlgProcessCmd : public QDialog
{
    Q_OBJECT

public:
    explicit dlgProcessCmd(QWidget *parent = 0);
    ~dlgProcessCmd();

private slots:
    void slot_StdoutUpdated();
    void slot_ProcessFinished();
    void on_pbProcess_clicked();
    void on_pbEnterMsg_clicked();
    void on_pbExit_clicked();
    void on_pbExitDlg_clicked();
    void on_pbChangeDir_clicked();
    void on_cmbDrive_currentIndexChanged(int index);
    void on_cmbCommand_textChanged(const QString &arg1);

    void on_pbClearOutput_clicked();

private:
    Ui::dlgProcessCmd   *ui;
    QString              m_qsCurrentPath;
    QProcess            *m_qpCommand;
    QString              m_qsProcessPath;
    bool                 m_bProcessHalted;

    void                _setProcessPath();
    void                _enableCommands( bool p_bEnabled );
    void                _appendStdOut( const QString &p_qsText );
};

#endif // DLGPROCESSCMD_H
