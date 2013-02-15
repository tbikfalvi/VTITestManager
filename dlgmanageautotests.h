#ifndef DLGMANAGEAUTOTESTS_H
#define DLGMANAGEAUTOTESTS_H

#include <QDialog>

#include "tmpreferences.h"
#include "dlgwarningbox.h"

namespace Ui {
    class dlgManageAutoTests;
}

class dlgManageAutoTests : public QDialog
{
    Q_OBJECT

public:
    explicit dlgManageAutoTests(QWidget *parent = 0, QString p_qsSystem = "", QString p_qsDir = "", QString p_qsDirConf = "" );
    ~dlgManageAutoTests();

private slots:
    void on_pbExecute_clicked();
    void on_pbExit_clicked();
    void on_chkShowWarning_clicked(bool checked);

private:
    Ui::dlgManageAutoTests      *ui;
    cTMPreferences::tcSystem     m_tcSystemType;
    QString                      m_qsDir;
    QString                      m_qsDirConf;
    cDlgWarningBox              *m_dlgWarningBox;

    void                        _executeProcess();
};

#endif // DLGMANAGEAUTOTESTS_H
