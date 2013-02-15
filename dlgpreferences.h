#ifndef DLGPREFERENCES_H
#define DLGPREFERENCES_H

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
    class dlgPreferences;
}

class dlgPreferences : public QDialog
{
    Q_OBJECT

public:
    explicit dlgPreferences(QWidget *parent = 0);
    ~dlgPreferences();

private slots:
    void slotSettingsChanged();
    void slotEnvironmentChanged();
    void on_pbDirAIFSelect_clicked();
    void on_pbDirAIFTestConfigSelect_clicked();
    void on_pbDirCyclerSelect_clicked();
    void on_pbDirCyclerTestConfigSelect_clicked();
    void on_pbApply_clicked();
    void on_pbClose_clicked();
    void on_pbDirCyclerTestConfigDerive_clicked();
    void on_pbDirLocalReleasesSelect_clicked();
    void on_pbSave_clicked();
    void on_pbModify_clicked();
    void on_pbDelete_clicked();
    void on_pbAdd_clicked();
    void on_listCommands_itemSelectionChanged();

private:
    Ui::dlgPreferences *ui;
    bool                bSettingsChanged;

    void _loadSettings();
    void _saveSettings();
    void _enableCommandButtons( bool p_bEnable );
};

#endif // DLGPREFERENCES_H
