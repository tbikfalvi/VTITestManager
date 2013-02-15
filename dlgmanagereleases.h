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

private:
    Ui::dlgManageReleases *ui;

    QString                 m_qsLocalDirRelease;
    QString                 m_qsAIFFileRelease;
    QString                 m_qsCyclerFileRelease;
    QString                 m_qsCSSFileRelease;

    void                    _fillListAIFReleases();
    void                    _fillListCyclerReleases();
    void                    _fillListCSSReleases();
};

#endif // DLGMANAGERELEASES_H
