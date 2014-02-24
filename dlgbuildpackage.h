#ifndef DLGBUILDPACKAGE_H
#define DLGBUILDPACKAGE_H

#include <QDialog>
#include <QProcess>
#include <QModelIndex>
#include <QTreeWidgetItem>

namespace Ui {
    class dlgBuildPackage;
}

class dlgBuildPackage : public QDialog
{
    Q_OBJECT

public:
    explicit dlgBuildPackage(QWidget *parent = 0);
    ~dlgBuildPackage();

private slots:
    void slot_PackageNameCreate();
    void slot_AIFStdoutUpdated();
    void slot_AIFProcessFinished();
    void slot_CyclerStdoutUpdated();
    void slot_CyclerProcessFinished();
    void slot_CSSStdoutUpdated();
    void slot_CSSProcessFinished();
    void slot_AIFLabelUpdate();
    void slot_CyclerLabelUpdate();
    void slot_CSSLabelUpdate();
    void on_pbStartAIFBuild_clicked();
    void on_pbAIFYes_clicked();
    void on_pbAIFNo_clicked();
    void on_pbAIFHalt_clicked();
    void on_pbExit_clicked();
    void on_chkAIFEngRelease_clicked();
    void on_pbStartCyclerBuild_clicked();
    void on_chkCyclerEngRelease_clicked();
    void on_pbStartCSSBuild_clicked();
    void on_chkCSSEngRelease_clicked();
    void on_pbCopyAIFRelease_clicked();
    void on_pbCopyCyclerRelease_clicked();
    void on_pbCyclerYes_clicked();
    void on_pbCyclerNo_clicked();
    void on_pbCyclerHalt_clicked();
    void on_pbCopyCSSRelease_clicked();
    void on_pbCSSYes_clicked();
    void on_pbCSSNo_clicked();
    void on_pbCSSHalt_clicked();
    void on_pbPublishAIFRelease_clicked();
    void on_pbPublishCylerRelease_clicked();
    void on_pbPublishCSSRelease_clicked();
    void on_treeSATReleases_doubleClicked(const QModelIndex &index);
    void on_pbSATNewProgram_clicked();
    void on_pbSATNewRelease_clicked();
    void on_pbSATModify_clicked();
    void on_pbSATDelete_clicked();
    void on_treeSATReleases_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void on_pbSATSave_clicked();
    void on_pbSATCancel_clicked();
    void on_pbCopySettings_clicked();
    void on_pbBuildSatPackage_clicked();

private:
    Ui::dlgBuildPackage *ui;

    QString              m_qsCurrentPath;
    QProcess            *m_qpBuildAIF;
    bool                 m_bProcessAIFHalted;
    QProcess            *m_qpBuildCycler;
    bool                 m_bProcessCyclerHalted;
    QProcess            *m_qpBuildCSS;
    bool                 m_bProcessCSSHalted;
    bool                 m_bModifySATRelease;
    bool                 m_bBuildCanStart;
    bool                 m_bIsReleaseCanBePublished;

    void                _fillSATTree();
    void                _saveSATTree();
    void                _enableAIFBuildSettings( bool p_bEnabled );
    void                _enableCyclerBuildSettings( bool p_bEnabled );
    void                _enableCSSBuildSettings( bool p_bEnabled );
    void                _saveBuildSettings();
    void                _appendAIFStdOut( const QString &p_qsText );
    void                _appendCyclerStdOut( const QString &p_qsText );
    void                _appendCSSStdOut( const QString &p_qsText );
};

#endif // DLGBUILDPACKAGE_H
