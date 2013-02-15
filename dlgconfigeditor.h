#ifndef DLGCONFIGEDITOR_H
#define DLGCONFIGEDITOR_H

#include <QDialog>
#include <QVector>
#include <QStringList>
#include <QStandardItemModel>
#include <QListWidgetItem>
#include <QTextStream>

#include "dlgprogress.h"
#include "tmpreferences.h"

namespace Ui {
    class dlgConfigEditor;
}

class dlgConfigEditor : public QDialog
{
    Q_OBJECT

public:
    explicit dlgConfigEditor(QWidget *parent = 0, QString p_qsSystem = "", QString p_qsDir = "", QString p_qsDirConf = "" );
    ~dlgConfigEditor();

private slots:
    void on_pbProcessConfig_clicked();
    void on_pbChangeFile_clicked();
    void on_pbResetFileName_clicked();
    void on_listTestTypes_itemClicked(QListWidgetItem *item);
    void on_pbExit_clicked();
    void on_pbSave_clicked();

    void on_pbSelectAll_clicked();

    void on_pbDeselectAll_clicked();

private:
    Ui::dlgConfigEditor         *ui;
    cDlgProgress                *m_dlgProgress;

    cTMPreferences::tcSystem     m_tcSystemType;
    bool                         m_bConfigXML;

    QString                      m_qsDir;
    QString                      m_qsDirConf;
    QString                      m_qsFileName;
    QVector<QStringList>         m_vTestCases;
    bool                         m_bSilentMode;
    bool                         m_bOnlyTP;

    void                        _readConfigFile();
    void                        _fillListView();
    void                        _fillListTypes();
    QString                     _addTestProcedure( const QString &p_qsLine );
    void                        _saveConfigSettings();
    void                        _readConfigXml( QString p_qsFileName );
    QString                     _getCyclerVersionFromPath( const QString &p_qsPath ) const;
};

#endif // DLGCONFIGEDITOR_H
