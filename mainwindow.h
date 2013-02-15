#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void closeEvent( QCloseEvent *p_poEvent );
    void on_action_ExitApplication_triggered();
    void on_action_Preferences_triggered();
    void on_action_Manage_registry_triggered();
    void on_action_AIF_TestConfiguration_triggered();
    void on_action_Cycler_TestConfiguration_triggered();
    void on_action_ManageReleases_triggered();
    void on_action_BuildVTIPackage_triggered();
    void on_action_ManageATP_AIF_triggered();
    void on_action_ManageATP_Cycler_triggered();
    void on_action_Process_Commands_triggered();

private:

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
